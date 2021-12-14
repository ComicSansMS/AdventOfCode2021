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
        CHECK(formula.polymer == "NNCB");

        REQUIRE(formula.rules.size() == 16);
        CHECK(formula.rules[0] == Rule{ .first = 'B', .second = 'B', .insert = 'N' });
        CHECK(formula.rules[1] == Rule{ .first = 'B', .second = 'C', .insert = 'B' });
        CHECK(formula.rules[2] == Rule{ .first = 'B', .second = 'H', .insert = 'H' });
        CHECK(formula.rules[3] == Rule{ .first = 'B', .second = 'N', .insert = 'B' });
        CHECK(formula.rules[4] == Rule{ .first = 'C', .second = 'B', .insert = 'H' });
        CHECK(formula.rules[5] == Rule{ .first = 'C', .second = 'C', .insert = 'N' });
        CHECK(formula.rules[6] == Rule{ .first = 'C', .second = 'H', .insert = 'B' });
        CHECK(formula.rules[7] == Rule{ .first = 'C', .second = 'N', .insert = 'C' });
        CHECK(formula.rules[8] == Rule{ .first = 'H', .second = 'B', .insert = 'C' });
        CHECK(formula.rules[9] == Rule{ .first = 'H', .second = 'C', .insert = 'B' });
        CHECK(formula.rules[10] == Rule{ .first = 'H', .second = 'H', .insert = 'N' });
        CHECK(formula.rules[11] == Rule{ .first = 'H', .second = 'N', .insert = 'C' });
        CHECK(formula.rules[12] == Rule{ .first = 'N', .second = 'B', .insert = 'B' });
        CHECK(formula.rules[13] == Rule{ .first = 'N', .second = 'C', .insert = 'B' });
        CHECK(formula.rules[14] == Rule{ .first = 'N', .second = 'H', .insert = 'C' });
        CHECK(formula.rules[15] == Rule{ .first = 'N', .second = 'N', .insert = 'C' });
    }

    SECTION("Rule Progression")
    {
        auto rp = computeRuleProgression(formula);
        CHECK(rp[formula.rules[0]].r1 == formula.rules[3]);
        CHECK(rp[formula.rules[0]].r2 == formula.rules[12]);
    }

    SECTION("Initial rule count")
    {
        auto rc = computeInitialRuleCount(formula);
        CHECK(rc.size() == 3);
        CHECK(rc[formula.rules[4]] == 1);
        CHECK(rc[formula.rules[13]] == 1);
        CHECK(rc[formula.rules[15]] == 1);
    }

    SECTION("Step")
    {
        auto const rp = computeRuleProgression(formula);
        auto rc = computeInitialRuleCount(formula);
        rc = step(rc, rp);
        CHECK(rc.size() == 6);
        CHECK(rc[formula.rules[1]] == 1);
        CHECK(rc[formula.rules[6]] == 1);
        CHECK(rc[formula.rules[7]] == 1);
        CHECK(rc[formula.rules[8]] == 1);
        CHECK(rc[formula.rules[12]] == 1);
        CHECK(rc[formula.rules[13]] == 1);
        rc = step(rc, rp);
        CHECK(rc.size() == 8);
        CHECK(rc[formula.rules[0]] == 2);
        CHECK(rc[formula.rules[1]] == 2);
        CHECK(rc[formula.rules[2]] == 1);
        CHECK(rc[formula.rules[4]] == 2);
        CHECK(rc[formula.rules[5]] == 1);
        CHECK(rc[formula.rules[7]] == 1);
        CHECK(rc[formula.rules[9]] == 1);
        CHECK(rc[formula.rules[12]] == 2);
    }

    SECTION("Count Occurrences")
    {
        auto const rp = computeRuleProgression(formula);
        auto rc = computeInitialRuleCount(formula);
        auto counts = countOccurrences(rc);
        REQUIRE(counts.size() == 3);
        CHECK(counts[0].c == 'B');
        CHECK(counts[0].count == 1);
        CHECK(counts[1].c == 'C');
        CHECK(counts[1].count == 1);
        CHECK(counts[2].c == 'N');
        CHECK(counts[2].count == 2);

        rc = step(rc, rp);
        counts = countOccurrences(rc);
        REQUIRE(counts.size() == 4);
        CHECK(counts[0].c == 'H');
        CHECK(counts[0].count == 1);
        CHECK(counts[1].c == 'B');
        CHECK(counts[1].count == 2);
        CHECK(counts[2].c == 'C');
        CHECK(counts[2].count == 2);
        CHECK(counts[3].c == 'N');
        CHECK(counts[3].count == 2);

        rc = step(rc, rp);
        counts = countOccurrences(rc);
        REQUIRE(counts.size() == 4);
        CHECK(counts[0].c == 'H');
        CHECK(counts[0].count == 1);
        CHECK(counts[1].c == 'N');
        CHECK(counts[1].count == 2);
        CHECK(counts[2].c == 'C');
        CHECK(counts[2].count == 4);
        CHECK(counts[3].c == 'B');
        CHECK(counts[3].count == 6);

        rc = step(rc, rp);
        counts = countOccurrences(rc);
        REQUIRE(counts.size() == 4);
        CHECK(counts[0].c == 'H');
        CHECK(counts[0].count == 4);
        CHECK(counts[1].c == 'C');
        CHECK(counts[1].count == 5);
        CHECK(counts[2].c == 'N');
        CHECK(counts[2].count == 5);
        CHECK(counts[3].c == 'B');
        CHECK(counts[3].count == 11);

        rc = step(rc, rp);
        counts = countOccurrences(rc);
        REQUIRE(counts.size() == 4);
        CHECK(counts[0].c == 'H');
        CHECK(counts[0].count == 5);
        CHECK(counts[1].c == 'C');
        CHECK(counts[1].count == 10);
        CHECK(counts[2].c == 'N');
        CHECK(counts[2].count == 11);
        CHECK(counts[3].c == 'B');
        CHECK(counts[3].count == 23);

        for (int i = 0; i < 6; ++i) {
            rc = step(rc, rp);
        }
        counts = countOccurrences(rc);
        REQUIRE(counts.size() == 4);
        CHECK(counts[0].c == 'H');
        CHECK(counts[0].count == 161);
        CHECK(counts[1].c == 'C');
        CHECK(counts[1].count == 298);
        CHECK(counts[2].c == 'N');
        CHECK(counts[2].count == 865);
        CHECK(counts[3].c == 'B');
        CHECK(counts[3].count == 1749);

        for (int i = 0; i < 30; ++i) {
            rc = step(rc, rp);
        }
        counts = countOccurrences(rc);
        REQUIRE(counts.size() == 4);
        CHECK(counts[0].c == 'H');
        CHECK(counts[0].count == 3849876073);
        CHECK(counts[3].c == 'B');
        CHECK(counts[3].count == 2192039569602);
    }

    SECTION("Result 1")
    {
        CHECK(result1(formula) == 1588);
    }

    SECTION("Result 2")
    {
        CHECK(result2(formula) == 2188189693529);
    }

}
