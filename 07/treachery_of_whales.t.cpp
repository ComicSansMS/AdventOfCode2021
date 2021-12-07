#include <treachery_of_whales.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Treachury of Whales")
{
    char const sample_input[] = "16,1,2,0,4,2,7,1,2,14\n";

    Crabs crabs = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(crabs.positions == std::vector<int64_t>{ 16, 1, 2, 0, 4, 2, 7, 1, 2, 14 });
        CHECK(crabs.min_position == 0);
        CHECK(crabs.max_position == 16);
    }

    SECTION("Cost")
    {
        CHECK(costMovingTo(crabs, 2) == 37);
        CHECK(costMovingTo(crabs, 1) == 41);
        CHECK(costMovingTo(crabs, 3) == 39);
        CHECK(costMovingTo(crabs, 10) == 71);
    }

    SECTION("Result 1")
    {
        CHECK(result1(crabs) == 37);
    }

    SECTION("Cost2")
    {
        CHECK(costMovingTo2(crabs, 2) == 206);
        CHECK(costMovingTo2(crabs, 5) == 168);
    }

    SECTION("Result 2")
    {
        CHECK(result2(crabs) == 168);
    }
}
