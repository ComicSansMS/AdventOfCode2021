#ifndef ADVENT_OF_CODE_16_PACKET_DECODER_INCLUDE_GUARD
#define ADVENT_OF_CODE_16_PACKET_DECODER_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>

struct RawData {
    std::vector<std::byte> d;
    std::size_t offset;

    int32_t extractBits(std::size_t n);
};

RawData parseInput(std::string_view input);

struct Header {
    uint8_t version;
    enum class Type : uint8_t {
        OpAdd = 0,
        OpMul = 1,
        OpMin = 2,
        OpMax = 3,
        Literal   = 4,
        OpGT = 5,
        OpLT = 6,
        OpEq = 7,
    } type_id;
};

struct Literal {
    int64_t n;
};

struct Packet;

struct Operator {
    std::vector<Packet> sub_packets;
};

using PacketData = std::variant<Literal, Operator>;

struct Packet {
    Header header;
    PacketData data;
};

Packet decode(RawData raw_data);

int32_t addVersionNumbers(Packet const& p);

int64_t evaluate(Packet const& p);

#endif
