#include <packet_decoder.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Packet Decoder")
{
    char const sample_input2[] = "EE00D40C823060";
    char const sample_input3[] = "8A004A801A8002F478";
    char const sample_input4[] = "620080001611562C8802118E34";
    char const sample_input5[] = "C0015000016115A2E0802F182340";
    char const sample_input6[] = "A0016C880162017C3686B18A3D4780";

    
    auto raw_data2 = parseInput(sample_input2);
    auto raw_data3 = parseInput(sample_input3);
    auto raw_data4 = parseInput(sample_input4);
    auto raw_data5 = parseInput(sample_input5);
    auto raw_data6 = parseInput(sample_input6);


    SECTION("Parse Input")
    {
        auto raw_data = parseInput("38006F45291200");
        CHECK(raw_data.offset == 0);
        REQUIRE(raw_data.d.size() == 7);
        CHECK(raw_data.d[0] == static_cast<std::byte>(0x38));
        CHECK(raw_data.d[1] == static_cast<std::byte>(0x00));
        CHECK(raw_data.d[2] == static_cast<std::byte>(0x6F));
        CHECK(raw_data.d[3] == static_cast<std::byte>(0x45));
        CHECK(raw_data.d[4] == static_cast<std::byte>(0x29));
        CHECK(raw_data.d[5] == static_cast<std::byte>(0x12));
        CHECK(raw_data.d[6] == static_cast<std::byte>(0x00));
    }

    SECTION("Extract Bits")
    {
        auto raw_data = parseInput("38006F45291200");
        // 001 110 00000000000 1101111010 0010100101 00 1 0 0 0 1 0010 00000000
        CHECK(raw_data.extractBits(3) == 0b001);
        CHECK(raw_data.extractBits(3) == 0b110);
        CHECK(raw_data.extractBits(11) == 0);
        CHECK(raw_data.extractBits(10) == 0b1101111010);
        CHECK(raw_data.extractBits(10) == 0b0010100101);
        CHECK(raw_data.extractBits(2) == 0);
        CHECK(raw_data.extractBits(1) == 0b1);
        CHECK(raw_data.extractBits(1) == 0b0);
        CHECK(raw_data.extractBits(1) == 0b0);
        CHECK(raw_data.extractBits(1) == 0b0);
        CHECK(raw_data.extractBits(1) == 0b1);
        CHECK(raw_data.extractBits(4) == 0b0010);
        CHECK(raw_data.extractBits(8) == 0);
    }

    auto const checkLiteral = [](Packet const& p, uint8_t expected_version, int32_t expected_n)
    {
        REQUIRE(p.header.type_id == Header::Type::Literal);
        CHECK(p.header.version == expected_version);
        CHECK(std::get<Literal>(p.data).n == expected_n);
    };

    SECTION("Decode Packet")
    {
        {
            auto const packet = decode(parseInput("D2FE28"));
            CHECK(packet.header.version == 6);
            CHECK(packet.header.type_id == Header::Type::Literal);
            CHECK(std::get<Literal>(packet.data).n == 0b0111'1110'0101);
        }
        {
            auto const packet = decode(parseInput("38006F45291200"));
            CHECK(packet.header.version == 1);
            CHECK(packet.header.type_id == Header::Type::OpLT);
            auto const& sub_packets = std::get<Operator>(packet.data).sub_packets;
            REQUIRE(sub_packets.size() == 2);
            checkLiteral(sub_packets[0], 6, 10);
            checkLiteral(sub_packets[1], 2, 20);
        }
        {
            auto const packet = decode(parseInput("EE00D40C823060"));
            CHECK(packet.header.version == 7);
            CHECK(packet.header.type_id == Header::Type::OpMax);
            auto const& sub_packets = std::get<Operator>(packet.data).sub_packets;
            REQUIRE(sub_packets.size() == 3);
            checkLiteral(sub_packets[0], 2, 1);
            checkLiteral(sub_packets[1], 4, 2);
            checkLiteral(sub_packets[2], 1, 3);
        }
        {
            auto const packet = decode(parseInput("8A004A801A8002F478"));
            CHECK(packet.header.version == 4);
            CHECK(packet.header.type_id == Header::Type::OpMin);
            auto const& sub_packets = std::get<Operator>(packet.data).sub_packets;
            REQUIRE(sub_packets.size() == 1);
            CHECK(sub_packets[0].header.version == 1);
            CHECK(sub_packets[0].header.type_id == Header::Type::OpMin);
            auto const& sub_sub_packets = std::get<Operator>(sub_packets[0].data).sub_packets;
            REQUIRE(sub_sub_packets.size() == 1);
            CHECK(sub_sub_packets[0].header.version == 5);
            CHECK(sub_sub_packets[0].header.type_id == Header::Type::OpMin);
            auto const& sub_sub_sub_packets = std::get<Operator>(sub_sub_packets[0].data).sub_packets;
            REQUIRE(sub_sub_sub_packets.size() == 1);
            checkLiteral(sub_sub_sub_packets[0], 6, 15);
        }
    }

    SECTION("Add Version Numbers")
    {
        CHECK(addVersionNumbers(decode(parseInput("8A004A801A8002F478"))) == 16);
        CHECK(addVersionNumbers(decode(parseInput("620080001611562C8802118E34"))) == 12);
        CHECK(addVersionNumbers(decode(parseInput("C0015000016115A2E0802F182340"))) == 23);
        CHECK(addVersionNumbers(decode(parseInput("A0016C880162017C3686B18A3D4780"))) == 31);
    }

    SECTION("Evaluate")
    {
        CHECK(evaluate(decode(parseInput("C200B40A82"))) == 3);
        CHECK(evaluate(decode(parseInput("04005AC33890"))) == 54);
        CHECK(evaluate(decode(parseInput("880086C3E88112"))) == 7);
        CHECK(evaluate(decode(parseInput("CE00C43D881120"))) == 9);
        CHECK(evaluate(decode(parseInput("D8005AC2A8F0"))) == 1);
        CHECK(evaluate(decode(parseInput("F600BC2D8F"))) == 0);
        CHECK(evaluate(decode(parseInput("9C005AC2F8F0"))) == 0);
        CHECK(evaluate(decode(parseInput("9C0141080250320F1802104A08"))) == 1);
    }
}
