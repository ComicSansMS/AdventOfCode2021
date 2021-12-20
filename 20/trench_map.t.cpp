#include <trench_map.hpp>

#include <catch.hpp>

TEST_CASE("Trench Map")
{
    char const sample_input[] = 
        "..#.#..#####.#.#.#.###.##.....###.##.#..###.####..#####..#....#..#..##..##"
        "#..######.###...####..#..#####..##..#.#####...##.#.#..#.##..#.#......#.###"
        ".######.###.####...#.##.##..#..#..#####.....#.#....###..#.##......#.....#."
        ".#..#..##..#...##.######.####.####.#.#...#.......#..#.#.#...####.##.#....."
        ".#..#...##.#.##..#...##.#.##..###.#......#.#.......#.#.#.####.###.##...#.."
        "...####.#..#..#.##.#....##..#.####....##...##..#...#......#.#.......#....."
        "..##..####..#...#.#.#...##..#.#..###..#####........#..####......#..#" "\n"
        "\n"
        "#..#." "\n"
        "#...." "\n"
        "##..#" "\n"
        "..#.." "\n"
        "..###" "\n";

    auto const img = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(img.min == Point{ .x = 0, .y = 0 });
        CHECK(img.max == Point{ .x = 4, .y = 4 });
        CHECK(img.lookup.to_string().substr(512 - 64) == "0100001001111100111101110010110111000001101110101010111110010100");
        CHECK(fmt::format("{}", img) == "#..#." "\n"
                                        "#...." "\n"
                                        "##..#" "\n"
                                        "..#.." "\n"
                                        "..###" "\n");
    }

    SECTION("Neighbourhood to Number")
    {
        CHECK(neigbourhood2Number(img, Point{ .x = 2, .y = 2 }, true) == 34);
        CHECK(neigbourhood2Number(img, Point{ .x = 2, .y = 2 }, false) == 34);
        // XXX
        // X#.
        // X#.
        CHECK(neigbourhood2Number(img, Point{ .x = 0, .y = 0 }, true) == 0b0'0001'0010);
        CHECK(neigbourhood2Number(img, Point{ .x = 0, .y = 0 }, false) == 0b1'1111'0110);
        
    }

    SECTION("Step")
    {
        Image s = img;
        CHECK(fmt::format("{}", s) == "#..#." "\n"
                                      "#...." "\n"
                                      "##..#" "\n"
                                      "..#.." "\n"
                                      "..###" "\n");
                                      
        s = step(s, true);
        CHECK(fmt::format("{}", s) == ".##.##." "\n"
                                      "#..#.#." "\n"
                                      "##.#..#" "\n"
                                      "####..#" "\n"
                                      ".#..##." "\n"
                                      "..##..#" "\n"
                                      "...#.#." "\n");

        s = step(s, true);
        CHECK(fmt::format("{}", s) == ".......#." "\n"
                                      ".#..#.#.." "\n"
                                      "#.#...###" "\n"
                                      "#...##.#." "\n"
                                      "#.....#.#" "\n"
                                      ".#.#####." "\n"
                                      "..#.#####" "\n"
                                      "...##.##." "\n"
                                      "....###.." "\n");

        for (int i = 2; i < 50; ++i) {
            s = step(s, true);
        }
        CHECK(s.points.size() == 3351);
    }

    SECTION("Enhance")
    {
        CHECK(enhance(img, 2) == 31);
    }
}
