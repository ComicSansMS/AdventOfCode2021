#include <extended_polymerization.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Transparent Origami")
{
    char const sample_input[] = "NNCB"  "\n"
                                ""      "\n"
                                "CH -> B" "\n"
                                "HH -> N" "\n"
                                "CB -> H" "\n"
                                "NH -> C" "\n"
                                "HB -> C" "\n"
                                "HC -> B" "\n"
                                "HN -> C" "\n"
                                "NN -> C" "\n"
                                "BH -> H" "\n"
                                "NC -> B" "\n"
                                "NB -> B" "\n"
                                "BN -> B" "\n"
                                "BB -> N" "\n"
                                "BC -> B" "\n"
                                "CC -> N" "\n"
                                "CN -> C" "\n";

    auto formula = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(static_cast<std::string>(formula.polymer) == "NNCB");

        CHECK(formula.insertion_rules('C', 'H') == 'B');
        CHECK(formula.insertion_rules('H', 'H') == 'N');
        CHECK(formula.insertion_rules('C', 'B') == 'H');
        CHECK(formula.insertion_rules('N', 'H') == 'C');
        CHECK(formula.insertion_rules('H', 'B') == 'C');
        CHECK(formula.insertion_rules('H', 'C') == 'B');
        CHECK(formula.insertion_rules('H', 'N') == 'C');
        CHECK(formula.insertion_rules('N', 'N') == 'C');
        CHECK(formula.insertion_rules('B', 'H') == 'H');
        CHECK(formula.insertion_rules('N', 'C') == 'B');
        CHECK(formula.insertion_rules('N', 'B') == 'B');
        CHECK(formula.insertion_rules('B', 'N') == 'B');
        CHECK(formula.insertion_rules('B', 'B') == 'N');
        CHECK(formula.insertion_rules('B', 'C') == 'B');
        CHECK(formula.insertion_rules('C', 'C') == 'N');
        CHECK(formula.insertion_rules('C', 'N') == 'C');
        
        CHECK(formula.insertion_rules('A', 'A') == '?');
    }

    SECTION("Polymerize")
    {
        CHECK(static_cast<std::string>(formula.polymer) == "NNCB");
        polymerize(formula);
        CHECK(static_cast<std::string>(formula.polymer) == "NCNBCHB");
        polymerize(formula);
        CHECK(static_cast<std::string>(formula.polymer) == "NBCCNBBBCBHCB");
        polymerize(formula);
        CHECK(static_cast<std::string>(formula.polymer) == "NBBBCNCCNBBNBNBBCHBHHBCHB");
        polymerize(formula);
        CHECK(static_cast<std::string>(formula.polymer) == "NBBNBNBBCCNBCNCCNBBNBBNBBBNBBNBBCBHCBHHNHCBBCBHCB");
        polymerize(formula);
        CHECK(formula.polymer.chain.size() == 97);
        for (int i = 0; i < 5; ++i) {
            polymerize(formula);
        }
        CHECK(formula.polymer.chain.size() == 3073);
    }

    SECTION("Occurrence Count")
    {
        for (int i = 0; i < 10; ++i) {
            polymerize(formula);
        }
        CHECK(formula.polymer.chain.size() == 3073);
        auto const oc = countOccurrences(formula.polymer);
        REQUIRE(oc.size() == 4);
        CHECK(oc[0].c == 'H');
        CHECK(oc[0].count == 161);
        CHECK(oc[1].c == 'C');
        CHECK(oc[1].count == 298);
        CHECK(oc[2].c == 'N');
        CHECK(oc[2].count == 865);
        CHECK(oc[3].c == 'B');
        CHECK(oc[3].count == 1749);
    }

    SECTION("Result 1")
    {
        CHECK(result1(formula) == 1588);
    }
}
