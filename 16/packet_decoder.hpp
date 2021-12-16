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
        Operator0 = 0,
        Operator1 = 1,
        Operator2 = 2,
        Operator3 = 3,
        Literal   = 4,
        Operator5 = 5,
        Operator6 = 6,
        Operator7 = 7,
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

#endif
