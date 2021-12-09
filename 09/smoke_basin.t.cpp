#include <smoke_basin.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Smoke Basin")
{
    char const sample_input[] = "2199943210" "\n"
                                "3987894921" "\n"
                                "9856789892" "\n"
                                "8767896789" "\n"
                                "9899965678" "\n";

    Heightmap const map = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(map.width == 10);
        CHECK(map.height == 5);
        CHECK(map.map.size() == 50);
        CHECK(fmt::format("{}", map) == sample_input);
    }

    SECTION("Lowest in Neighbourhood")
    {
        CHECK(!isLowerThan4Neighbourhood(map, 0, 0));
        CHECK(isLowerThan4Neighbourhood(map, 1, 0));
        CHECK(isLowerThan4Neighbourhood(map, 9, 0));
        CHECK(!isLowerThan4Neighbourhood(map, 0, 4));
        CHECK(!isLowerThan4Neighbourhood(map, 9, 4));
    }

    SECTION("Get Low Points")
    {
        auto points = getLowPoints(map);
        REQUIRE(points.size() == 4);
        CHECK(points[0] == Point{ .x = 1, .y = 0 });
        CHECK(points[1] == Point{ .x = 9, .y = 0 });
        CHECK(points[2] == Point{ .x = 2, .y = 2 });
        CHECK(points[3] == Point{ .x = 6, .y = 4 });
    }

    SECTION("Result 1")
    {
        CHECK(result1(map) == 15);
    }

    SECTION("Partition Basins")
    {
        auto basins = partitionBasins(map);
        REQUIRE(basins.size() == 4);
        CHECK(basins[0].points == std::vector{
                Point{ .x = 0, .y = 0 },
                Point{ .x = 1, .y = 0 },
                Point{ .x = 0, .y = 1 },
            });

        CHECK(basins[1].points == std::vector{
                Point{.x = 5, .y = 0 },
                Point{.x = 6, .y = 0 },
                Point{.x = 7, .y = 0 },
                Point{.x = 8, .y = 0 },
                Point{.x = 9, .y = 0 },
                Point{.x = 6, .y = 1 },
                Point{.x = 8, .y = 1 },
                Point{.x = 9, .y = 1 },
                Point{.x = 9, .y = 2 },
            });
        CHECK(basins[2].points == std::vector{
                Point{.x = 2, .y = 1 },
                Point{.x = 3, .y = 1 },
                Point{.x = 4, .y = 1 },
                Point{.x = 1, .y = 2 },
                Point{.x = 2, .y = 2 },
                Point{.x = 3, .y = 2 },
                Point{.x = 4, .y = 2 },
                Point{.x = 5, .y = 2 },
                Point{.x = 0, .y = 3 },
                Point{.x = 1, .y = 3 },
                Point{.x = 2, .y = 3 },
                Point{.x = 3, .y = 3 },
                Point{.x = 4, .y = 3 },
                Point{.x = 1, .y = 4 },
            });
        CHECK(basins[3].points == std::vector{
                Point{.x = 7, .y = 2 },
                Point{.x = 6, .y = 3 },
                Point{.x = 7, .y = 3 },
                Point{.x = 8, .y = 3 },
                Point{.x = 5, .y = 4 },
                Point{.x = 6, .y = 4 },
                Point{.x = 7, .y = 4 },
                Point{.x = 8, .y = 4 },
                Point{.x = 9, .y = 4 },
            });
    }

    SECTION("Result 2")
    {
        CHECK(result2(map) == 1134);
    }
}
