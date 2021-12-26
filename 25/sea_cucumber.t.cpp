#include <sea_cucumber.hpp>

#include <catch.hpp>

TEST_CASE("Sea Cucumber")
{
    char const sample_input[] =
        "v...>>.vv>" "\n"
        ".vv>>.vv.." "\n"
        ">>.>v>...v" "\n"
        ">>v>>.>.v." "\n"
        "v>v.vv.v.." "\n"
        ">.>>..v..." "\n"
        ".vv..>.>v." "\n"
        "v.v..>>v.v" "\n"
        "....v..v.>" "\n";

    auto const m = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(m.width == 10);
        CHECK(m.height == 9);
        CHECK(fmt::format("{}", m) == sample_input);
    }

    SECTION("Move East")
    {
        auto east = parseInput("...>>>>>..." "\n");
        std::size_t count = 0;
        east = moveEast(east, count);
        CHECK(fmt::format("{}", east) == "...>>>>.>.." "\n");
        CHECK(count == 1);
        count = 0;
        east = moveEast(east, count);
        CHECK(fmt::format("{}", east) == "...>>>.>.>." "\n");
        CHECK(count == 2);
        count = 0;
        east = moveEast(east, count);
        CHECK(fmt::format("{}", east) == "...>>.>.>.>" "\n");
        CHECK(count == 3);
        count = 0;
        east = moveEast(east, count);
        CHECK(fmt::format("{}", east) == ">..>.>.>.>." "\n");
        CHECK(count == 4);
    }

    SECTION("Move South")
    {
        auto south = parseInput("." "\n"
                               "." "\n"
                               "v" "\n"
                               "v" "\n"
                               "v" "\n"
                               "." "\n"
                               "." "\n");
        std::size_t count = 0;
        south = moveSouth(south, count);
        CHECK(fmt::format("{}", south) == "." "\n"
                                          "." "\n"
                                          "v" "\n"
                                          "v" "\n"
                                          "." "\n"
                                          "v" "\n"
                                          "." "\n");
        CHECK(count == 1);
        count = 0;
        south = moveSouth(south, count);
        CHECK(fmt::format("{}", south) == "." "\n"
                                          "." "\n"
                                          "v" "\n"
                                          "." "\n"
                                          "v" "\n"
                                          "." "\n"
                                          "v" "\n");
        CHECK(count == 2);
        count = 0;
        south = moveSouth(south, count);
        CHECK(fmt::format("{}", south) == "v" "\n"
                                          "." "\n"
                                          "." "\n"
                                          "v" "\n"
                                          "." "\n"
                                          "v" "\n"
                                          "." "\n");
        CHECK(count == 3);
    }

    SECTION("Step")
    {
        auto [s1, moved] = step(m);
        CHECK(fmt::format("{}", s1) == "....>.>v.>" "\n"
                                       "v.v>.>v.v." "\n"
                                       ">v>>..>v.." "\n"
                                       ">>v>v>.>.v" "\n"
                                       ".>v.v...v." "\n"
                                       "v>>.>vvv.." "\n"
                                       "..v...>>.." "\n"
                                       "vv...>>vv." "\n"
                                       ">.v.v..v.v" "\n");
        auto [s2, moved2] = step(s1);
        CHECK(fmt::format("{}", s2) == ">.v.v>>..v" "\n"
                                       "v.v.>>vv.." "\n"
                                       ">v>.>.>.v." "\n"
                                       ">>v>v.>v>." "\n"
                                       ".>..v....v" "\n"
                                       ".>v>>.v.v." "\n"
                                       "v....v>v>." "\n"
                                       ".vv..>>v.." "\n"
                                       "v>.....vv." "\n");
        auto [s3, moved3] = step(s2);
        CHECK(fmt::format("{}", s3) == "v>v.v>.>v." "\n"
                                       "v...>>.v.v" "\n"
                                       ">vv>.>v>.." "\n"
                                       ">>v>v.>.v>" "\n"
                                       "..>....v.." "\n"
                                       ".>.>v>v..v" "\n"
                                       "..v..v>vv>" "\n"
                                       "v.v..>>v.." "\n"
                                       ".v>....v.." "\n");
        auto [s4, moved4] = step(s3);
        CHECK(fmt::format("{}", s4) == "v>..v.>>.." "\n"
                                       "v.v.>.>.v." "\n"
                                       ">vv.>>.v>v" "\n"
                                       ">>.>..v>.>" "\n"
                                       "..v>v...v." "\n"
                                       "..>>.>vv.." "\n"
                                       ">.v.vv>v.v" "\n"
                                       ".....>>vv." "\n"
                                       "vvv>...v.." "\n");
        auto [s5, moved5] = step(s4);
        CHECK(fmt::format("{}", s5) == "vv>...>v>." "\n"
                                       "v.v.v>.>v." "\n"
                                       ">.v.>.>.>v" "\n"
                                       ">v>.>..v>>" "\n"
                                       "..v>v.v..." "\n"
                                       "..>.>>vvv." "\n"
                                       ".>...v>v.." "\n"
                                       "..v.v>>v.v" "\n"
                                       "v.v.>...v." "\n");
        Map s = std::move(s5);
        for (int i = 0; i < 25; ++i) {
            auto [si, mi] = step(s);
            s = std::move(si);
        }
        CHECK(fmt::format("{}", s) == ".vv.v..>>>" "\n"
                                      "v>...v...>" "\n"
                                      ">.v>.>vv.>" "\n"
                                      ">v>.>.>v.>" "\n"
                                      ".>..v.vv.." "\n"
                                      "..v>..>>v." "\n"
                                      "....v>..>v" "\n"
                                      "v.v...>vv>" "\n"
                                      "v.v...>vvv" "\n");
    }

    SECTION("Play until completion")
    {
        CHECK(play(m) == 58);
    }
}
