#include <transparent_origami.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Transparent Origami")
{
    char const sample_input[] = "6,10"           "\n"
                                "0,14"           "\n"
                                "9,10"           "\n"
                                "0,3"            "\n"
                                "10,4"           "\n"
                                "4,11"           "\n"
                                "6,0"            "\n"
                                "6,12"           "\n"
                                "4,1"            "\n"
                                "0,13"           "\n"
                                "10,12"          "\n"
                                "3,4"            "\n"
                                "3,0"            "\n"
                                "8,4"            "\n"
                                "1,10"           "\n"
                                "2,14"           "\n"
                                "8,10"           "\n"
                                "9,0"            "\n"
                                ""               "\n"
                                "fold along y=7" "\n"
                                "fold along x=5" "\n";

    auto const transparency = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(transparency.max == Point{ .x = 10, .y = 14 });
        CHECK(fmt::format("{}", transparency) == "...#..#..#." "\n"
                                                 "....#......" "\n"
                                                 "..........." "\n"
                                                 "#.........." "\n"
                                                 "...#....#.#" "\n"
                                                 "..........." "\n"
                                                 "..........." "\n"
                                                 "..........." "\n"
                                                 "..........." "\n"
                                                 "..........." "\n"
                                                 ".#....#.##." "\n"
                                                 "....#......" "\n"
                                                 "......#...#" "\n"
                                                 "#.........." "\n"
                                                 "#.#........" "\n");

        REQUIRE(transparency.folds.size() == 2);
        CHECK(transparency.folds[0].axis == Fold::Axis::Y);
        CHECK(transparency.folds[0].coordinate == 7);
        CHECK(transparency.folds[1].axis == Fold::Axis::X);
        CHECK(transparency.folds[1].coordinate == 5);

        CHECK(!transparency.activeFold);
    }

    SECTION("Fold")
    {
        auto t = transparency;
        t.activeFold = 0;
        CHECK(t.points.size() == 18);
        CHECK(fmt::format("{}", t) == "...#..#..#." "\n"
                                      "....#......" "\n"
                                      "..........." "\n"
                                      "#.........." "\n"
                                      "...#....#.#" "\n"
                                      "..........." "\n"
                                      "..........." "\n"
                                      "-----------" "\n"
                                      "..........." "\n"
                                      "..........." "\n"
                                      ".#....#.##." "\n"
                                      "....#......" "\n"
                                      "......#...#" "\n"
                                      "#.........." "\n"
                                      "#.#........" "\n");
        t = fold(t);
        CHECK(!t.activeFold);
        CHECK(t.max == Point{ .x = 10, .y = 6 });
        CHECK(t.points.size() == 17);
        CHECK(fmt::format("{}", t) == "#.##..#..#." "\n"
                                      "#...#......" "\n"
                                      "......#...#" "\n"
                                      "#...#......" "\n"
                                      ".#.#..#.###" "\n"
                                      "..........." "\n"
                                      "..........." "\n");

        t.activeFold = 1;
        CHECK(fmt::format("{}", t) == "#.##.|#..#." "\n"
                                      "#...#|....." "\n"
                                      ".....|#...#" "\n"
                                      "#...#|....." "\n"
                                      ".#.#.|#.###" "\n"
                                      ".....|....." "\n"
                                      ".....|....." "\n");
        t = fold(t);
        CHECK(!t.activeFold);
        CHECK(t.max == Point{ .x = 4, .y = 6 });
        CHECK(t.points.size() == 16);
        CHECK(fmt::format("{}", t) == "#####" "\n"
                                      "#...#" "\n"
                                      "#...#" "\n"
                                      "#...#" "\n"
                                      "#####" "\n"
                                      "....." "\n"
                                      "....." "\n");
    }

    SECTION("Result 1")
    {
        CHECK(result1(transparency) == 17);
    }

    SECTION("Fold all")
    {
        CHECK(fmt::format("{}", foldAll(transparency)) == "#####" "\n"
                                                          "#...#" "\n"
                                                          "#...#" "\n"
                                                          "#...#" "\n"
                                                          "#####" "\n"
                                                          "....." "\n"
                                                          "....." "\n");
    }

}
