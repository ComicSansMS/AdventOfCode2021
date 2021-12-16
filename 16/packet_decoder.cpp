#include <packet_decoder.hpp>

#include <range/v3/algorithm/max.hpp>
#include <range/v3/algorithm/min.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>
#include <limits>

RawData parseInput(std::string_view input)
{
    auto decode_hex = [](char c) -> std::byte {
        if ((c >= '0') && (c <= '9')) {
            return static_cast<std::byte>(c - '0');
        } else {
            assert((c >= 'A') && (c <= 'F'));
            return static_cast<std::byte>((c - 'A') + 10);
        }
    };
    RawData ret{};
    for (std::size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '\n') {
            assert(i == input.size() - 1);
            break;
        }
        std::byte const upper_nibble = decode_hex(input[i]);
        ++i;
        assert(i < input.size());
        std::byte const lower_nibble = decode_hex(input[i]);
        std::byte const byte = (upper_nibble << 4) | lower_nibble;
        ret.d.push_back(byte);
    }
    return ret;
}

int32_t RawData::extractBits(std::size_t n)
{
    int32_t ret = 0;
    auto const all_ones = [](std::size_t n) -> std::byte {
        return static_cast<std::byte>((1 << (n)) - 1);
    };
    while (n > 0) {
        assert(n <= 32);
        assert((offset + n) <= (d.size() * 8));
        std::size_t byte_offset = offset / 8;
        std::byte const current_byte = d[byte_offset];
        std::size_t bit_offset = offset % 8;
        std::size_t bits_in_current_byte = 8 - bit_offset;
        std::size_t bits_to_extract = std::min(n, bits_in_current_byte);
        std::size_t bits_remaining = (bits_in_current_byte - bits_to_extract);

        std::byte const mask = all_ones(bits_to_extract);
        std::byte const extracted_bits = (current_byte >> bits_remaining) & mask;
        ret <<= bits_to_extract;
        ret |= static_cast<int32_t>(extracted_bits);
        offset += bits_to_extract;
        n -= bits_to_extract;
    }
    return ret;
}

Header decodeHeader(RawData& raw_data)
{
    Header h{};
    h.version = static_cast<uint8_t>(raw_data.extractBits(3));
    h.type_id = static_cast<Header::Type>(raw_data.extractBits(3));
    return h;
}

Literal decodeLiteral(RawData& raw_data)
{
    Literal ret{};
    for (;;) {
        int32_t const n = raw_data.extractBits(5);
        ret.n = (ret.n << 4) | (n & 0xf);
        if ((n & 0x10) == 0) { break; }
    }
    return ret;
}

void skipTrailingZeroes(RawData& raw_data)
{
    std::size_t const bits_to_skip = 8 - (raw_data.offset % 8);
    if (bits_to_skip < 8) {
        int32_t const skipped = raw_data.extractBits(bits_to_skip);
        assert(skipped == 0);
    }
    assert((raw_data.offset % 8) == 0);
}

std::vector<Packet> decodeSubPackets(RawData& raw_data);

Packet decodePacket(RawData& raw_data)
{
    Header const h = decodeHeader(raw_data);
    if (h.type_id == Header::Type::Literal) {
        Literal const l = decodeLiteral(raw_data);
        return Packet{ .header = h, .data = l };
    } else {
        std::vector<Packet> sub_packets = decodeSubPackets(raw_data);
        return Packet{ .header = h, .data = Operator{ .sub_packets = std::move(sub_packets) } };
    }
}

std::vector<Packet> decodeSubPackets(RawData& raw_data)
{
    std::vector<Packet> ret;
    int32_t const length_type_id = raw_data.extractBits(1);
    if (length_type_id == 0) {
        int32_t const sub_packet_length = raw_data.extractBits(15);
        std::size_t const target_offset = raw_data.offset + sub_packet_length;
        while (raw_data.offset < target_offset) {
            ret.push_back(decodePacket(raw_data));
        }
    } else {
        assert(length_type_id == 1);
        int32_t const number_sub_packets = raw_data.extractBits(11);
        for (int32_t i = 0; i < number_sub_packets; ++i) {
            ret.push_back(decodePacket(raw_data));
        }
    }
    return ret;
}

Packet decode(RawData raw_data)
{
    Packet const ret = decodePacket(raw_data);
    skipTrailingZeroes(raw_data);
    assert(raw_data.offset == (raw_data.d.size() * 8));
    return ret;
}

struct AddVersionNumbersPacketVisitor {
    int32_t operator()(Literal const&) const { return 0; }

    int32_t operator()(Operator const& op) const;
};

int32_t addVersionNumbers_rec(Packet const& p)
{
    return p.header.version + std::visit(AddVersionNumbersPacketVisitor{}, p.data);
}

int32_t AddVersionNumbersPacketVisitor::operator()(Operator const& op) const
{
    int32_t acc = 0;
    for (auto const& p : op.sub_packets)
    {
        acc += addVersionNumbers_rec(p);
    }
    return acc;
}

int32_t addVersionNumbers(Packet const& p)
{
    return addVersionNumbers_rec(p);
}

class EvaluateVisitor {
private:
    Header::Type m_type;
public:
    EvaluateVisitor(Header::Type type) : m_type(type) {}

    int64_t operator()(Literal const&) const;

    int64_t operator()(Operator const& op) const;
};

int64_t evaluate_rec(Packet const& p)
{
    return std::visit(EvaluateVisitor{ p.header.type_id }, p.data);
}

int64_t EvaluateVisitor::operator()(Literal const& l) const
{
    assert(m_type == Header::Type::Literal);
    return l.n;
}

int64_t EvaluateVisitor::operator()(Operator const& op) const
{
    std::vector<int64_t> args;
    args.reserve(op.sub_packets.size());
    for (auto const sp : op.sub_packets) {
        args.push_back(evaluate_rec(sp));
    }

    if (m_type == Header::Type::OpAdd) {
        return ranges::accumulate(args, static_cast<int64_t>(0));
    } else if (m_type == Header::Type::OpMul) {
        return ranges::accumulate(args, static_cast<int64_t>(1), ranges::multiplies{});
    } else if (m_type == Header::Type::OpMin) {
        return ranges::min(args);
    } else if (m_type == Header::Type::OpMax) {
        return ranges::max(args);
    } else if (m_type == Header::Type::OpGT) {
        assert(args.size() == 2);
        return (args[0] > args[1]) ? 1 : 0;
    } else if (m_type == Header::Type::OpLT) {
        assert(args.size() == 2);
        return (args[0] < args[1]) ? 1 : 0;
    } else {
        assert(m_type == Header::Type::OpEq);
        assert(args.size() == 2);
        return (args[0] == args[1]) ? 1 : 0;
    }
}


int64_t evaluate(Packet const& p)
{
    return evaluate_rec(p);
}
