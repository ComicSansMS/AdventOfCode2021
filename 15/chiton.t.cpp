#include <chiton.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Transparent Origami")
{
    char const sample_input[] = "1163751742" "\n"
                                "1381373672" "\n"
                                "2136511328" "\n"
                                "3694931569" "\n"
                                "7463417111" "\n"
                                "1319128137" "\n"
                                "1359912421" "\n"
                                "3125421639" "\n"
                                "1293138521" "\n"
                                "2311944581" "\n";

    auto const map = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(map.width == 10);
        CHECK(map.height == 10);
        REQUIRE(map.m.size() == 100);

        CHECK(map.m[getIndex(map, 0, 0)] == 1);
        CHECK(map.m[getIndex(map, 1, 0)] == 1);
        CHECK(map.m[getIndex(map, 2, 0)] == 6);
        CHECK(map.m[getIndex(map, 3, 0)] == 3);
        CHECK(map.m[getIndex(map, 4, 0)] == 7);
        CHECK(map.m[getIndex(map, 5, 0)] == 5);
        CHECK(map.m[getIndex(map, 6, 0)] == 1);
        CHECK(map.m[getIndex(map, 7, 0)] == 7);
        CHECK(map.m[getIndex(map, 8, 0)] == 4);
        CHECK(map.m[getIndex(map, 9, 0)] == 2);

        CHECK(map.m[getIndex(map, 0, 9)] == 2);
        CHECK(map.m[getIndex(map, 1, 9)] == 3);
        CHECK(map.m[getIndex(map, 2, 9)] == 1);
        CHECK(map.m[getIndex(map, 3, 9)] == 1);
        CHECK(map.m[getIndex(map, 4, 9)] == 9);
        CHECK(map.m[getIndex(map, 5, 9)] == 4);
        CHECK(map.m[getIndex(map, 6, 9)] == 4);
        CHECK(map.m[getIndex(map, 7, 9)] == 5);
        CHECK(map.m[getIndex(map, 8, 9)] == 8);
        CHECK(map.m[getIndex(map, 9, 9)] == 1);
    }

    SECTION("Expand Map")
    {
        auto const e = expandMap(map);
        CHECK(e.width == 50);
        CHECK(e.height == 50);
        REQUIRE(e.m.size() == 2500);
        CHECK(std::all_of(begin(e.m), end(e.m), [](int c) { return (c > 0) && (c <= 9); }));
        CHECK(e.m[getIndex(e, 0, 0)] == 1);
        CHECK(e.m[getIndex(e, 1, 0)] == 1);
        CHECK(e.m[getIndex(e, 2, 0)] == 6);
        CHECK(e.m[getIndex(e, 3, 0)] == 3);

        CHECK(e.m[getIndex(e, 46, 49)] == 3);
        CHECK(e.m[getIndex(e, 47, 49)] == 4);
        CHECK(e.m[getIndex(e, 48, 49)] == 7);
        CHECK(e.m[getIndex(e, 49, 49)] == 9);
    }

    SECTION("Shortest Path")
    {
        CHECK(shortestPath(map) == 40);
        CHECK(shortestPath(expandMap(map)) == 315);
    }
    
    
}
