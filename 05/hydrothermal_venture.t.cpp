#include <hydrothermal_venture.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Hydrothermal Venture")
{
    char const sample_input[] =
        "0,9 -> 5,9" "\n"
        "8,0 -> 0,8" "\n"
        "9,4 -> 3,4" "\n"
        "2,2 -> 2,1" "\n"
        "7,0 -> 7,4" "\n"
        "6,4 -> 2,0" "\n"
        "0,9 -> 2,9" "\n"
        "3,4 -> 1,4" "\n"
        "0,0 -> 8,8" "\n"
        "5,5 -> 8,2" "\n";

    std::vector<Line> lines = parseInput(sample_input);

    SECTION("Point")
    {
        std::stringstream sstr;
        sstr << Point{ 23, -42 };
        CHECK(sstr.str() == "{ x: 23 y: -42 }");
        CHECK(Point{ 1, 2 } == Point{ 1, 2 });
        CHECK(!(Point{ 1, 2 } == Point{ 1, 0 }));
        CHECK(!(Point{ 1, 2 } == Point{ 0, 2 }));
        CHECK(!(Point{ 1, 2 } == Point{ 0, 0 }));
    }

    SECTION("Line")
    {
        std::stringstream sstr;
        sstr << Line{ Point{ 23, -42 }, Point{ 55, 0 } };
        CHECK(sstr.str() == "{ start: { x: 23 y: -42 } end: { x: 55 y: 0 } }");
    }

    SECTION("Parse Input")
    {
        REQUIRE(lines.size() == 10);

        CHECK(lines[0] == Line{ Point{ 0, 9 }, Point{ 5, 9 } });
        CHECK(lines[1] == Line{ Point{ 8, 0 }, Point{ 0, 8 } });
        CHECK(lines[2] == Line{ Point{ 9, 4 }, Point{ 3, 4 } });
        CHECK(lines[3] == Line{ Point{ 2, 2 }, Point{ 2, 1 } });
        CHECK(lines[4] == Line{ Point{ 7, 0 }, Point{ 7, 4 } });
        CHECK(lines[5] == Line{ Point{ 6, 4 }, Point{ 2, 0 } });
        CHECK(lines[6] == Line{ Point{ 0, 9 }, Point{ 2, 9 } });
        CHECK(lines[7] == Line{ Point{ 3, 4 }, Point{ 1, 4 } });
        CHECK(lines[8] == Line{ Point{ 0, 0 }, Point{ 8, 8 } });
        CHECK(lines[9] == Line{ Point{ 5, 5 }, Point{ 8, 2 } });

        auto const lines2 = parseInput("110,99 -> 55,9" "\n"
                                       "821,912 -> 594,111" "\n");
        CHECK(lines2 == std::vector<Line>{
                Line{ Point{ 110, 99 }, Point{ 55, 9 } },
                Line{ Point{ 821, 912 }, Point{ 594, 111 } }
            });
    }

    SECTION("Calculate Map")
    {
        auto const m = calculateMap(lines);
        CHECK(m.min == Point{ 0, 0 });
        CHECK(m.max == Point{ 9, 9 });
        CHECK(fmt::format("{}", m) == 
            ".......1.." "\n"
            "..1....1.." "\n"
            "..1....1.." "\n"
            ".......1.." "\n"
            ".112111211" "\n"
            ".........." "\n"
            ".........." "\n"
            ".........." "\n"
            ".........." "\n"
            "222111...." "\n");
    }

    SECTION("Result 1")
    {
        CHECK(result(calculateMap(lines)) == 5);
    }

    SECTION("Add Diagonal Lines")
    {
        auto m = calculateMap(lines);
        addDiagonalLines(m, lines);
        CHECK(fmt::format("{}", m) ==
            "1.1....11." "\n"
            ".111...2.." "\n"
            "..2.1.111." "\n"
            "...1.2.2.." "\n"
            ".112313211" "\n"
            "...1.2...." "\n"
            "..1...1..." "\n"
            ".1.....1.." "\n"
            "1.......1." "\n"
            "222111...." "\n");
    }

    SECTION("Result 2")
    {
        auto m = calculateMap(lines);
        addDiagonalLines(m, lines);
        CHECK(result(m) == 12);
    }
}
