#include <binary_diagnostic.hpp>

#include <catch.hpp>

TEST_CASE("Binary Diagnostic")
{
    char const sample_input[] =
        "00100" "\n"
        "11110" "\n"
        "10110" "\n"
        "10111" "\n"
        "10101" "\n"
        "01111" "\n"
        "00111" "\n"
        "11100" "\n"
        "10000" "\n"
        "11001" "\n"
        "00010" "\n"
        "01010" "\n";

    auto const [report, entry_length] = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(report.size() == 12);
        REQUIRE(entry_length == 5);
        CHECK(report[0].to_ulong() == 0b00100u);
        CHECK(report[1].to_ulong() == 0b11110u);
        CHECK(report[2].to_ulong() == 0b10110u);
        CHECK(report[3].to_ulong() == 0b10111u);
        CHECK(report[4].to_ulong() == 0b10101u);
        CHECK(report[5].to_ulong() == 0b01111u);
        CHECK(report[6].to_ulong() == 0b00111u);
        CHECK(report[7].to_ulong() == 0b11100u);
        CHECK(report[8].to_ulong() == 0b10000u);
        CHECK(report[9].to_ulong() == 0b11001u);
        CHECK(report[10].to_ulong() == 0b00010u);
        CHECK(report[11].to_ulong() == 0b01010u);
    }

    SECTION("Pincount")
    {
        auto const acc = pincountInPosition(report);
        CHECK(acc[5] == 0);
        CHECK(acc[4] == 7);
        CHECK(acc[3] == 5);
        CHECK(acc[2] == 8);
        CHECK(acc[1] == 7);
        CHECK(acc[0] == 5);
    }

    SECTION("Gamma Bits")
    {
        CHECK(gammaBits(report).to_ulong() == 0b10110u);
    }

    SECTION("Result 1")
    {
        CHECK(result1(report, entry_length) == 198);
    }

    SECTION("Filter O2")
    {
        auto candidates = report;
        filterO2Candidate(candidates, 4);
        REQUIRE(candidates.size() == 7);
        CHECK(candidates[0].to_ullong() == 0b11110u);
        CHECK(candidates[1].to_ullong() == 0b10110u);
        CHECK(candidates[2].to_ullong() == 0b10111u);
        CHECK(candidates[3].to_ullong() == 0b10101u);
        CHECK(candidates[4].to_ullong() == 0b11100u);
        CHECK(candidates[5].to_ullong() == 0b10000u);
        CHECK(candidates[6].to_ullong() == 0b11001u);
        filterO2Candidate(candidates, 3);
        REQUIRE(candidates.size() == 4);
        CHECK(candidates[0].to_ullong() == 0b10110u);
        CHECK(candidates[1].to_ullong() == 0b10111u);
        CHECK(candidates[2].to_ullong() == 0b10101u);
        CHECK(candidates[3].to_ullong() == 0b10000u);
        filterO2Candidate(candidates, 2);
        REQUIRE(candidates.size() == 3);
        CHECK(candidates[0].to_ullong() == 0b10110u);
        CHECK(candidates[1].to_ullong() == 0b10111u);
        CHECK(candidates[2].to_ullong() == 0b10101u);
        filterO2Candidate(candidates, 1);
        REQUIRE(candidates.size() == 2);
        CHECK(candidates[0].to_ullong() == 0b10110u);
        CHECK(candidates[1].to_ullong() == 0b10111u);
        filterO2Candidate(candidates, 0);
        REQUIRE(candidates.size() == 1);
        CHECK(candidates[0].to_ullong() == 0b10111u);
    }

    SECTION("Filter CO2")
    {
        auto candidates = report;
        filterCO2Candidate(candidates, 4);
        REQUIRE(candidates.size() == 5);
        CHECK(candidates[0].to_ullong() == 0b00100u);
        CHECK(candidates[1].to_ullong() == 0b01111u);
        CHECK(candidates[2].to_ullong() == 0b00111u);
        CHECK(candidates[3].to_ullong() == 0b00010u);
        CHECK(candidates[4].to_ullong() == 0b01010u);
        filterCO2Candidate(candidates, 3);
        REQUIRE(candidates.size() == 2);
        CHECK(candidates[0].to_ullong() == 0b01111u);
        CHECK(candidates[1].to_ullong() == 0b01010u);
        filterCO2Candidate(candidates, 2);
        REQUIRE(candidates.size() == 1);
        CHECK(candidates[0].to_ullong() == 0b01010u);
        filterCO2Candidate(candidates, 1);
        REQUIRE(candidates.size() == 1);
        CHECK(candidates[0].to_ullong() == 0b01010u);
        filterCO2Candidate(candidates, 0);
        REQUIRE(candidates.size() == 1);
        CHECK(candidates[0].to_ullong() == 0b01010u);
    }

    SECTION("Result 2")
    {
        CHECK(result2(report, entry_length) == 230);
    }
}
