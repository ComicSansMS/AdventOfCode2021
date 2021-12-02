#include <dive.hpp>

#include <range/v3/view/subrange.hpp>
#include <range/v3/range/conversion.hpp>

#include <catch.hpp>

TEST_CASE("Dive!")
{
    char const sample_input[] =
        "forward 5" "\n"
        "down 5"    "\n"
        "forward 8" "\n"
        "up 3"      "\n"
        "down 8"    "\n"
        "forward 2" "\n";

    std::vector<Command> const commands = parseInput(sample_input);
    SECTION("Parse Input")
    {
        REQUIRE(commands.size() == 6);

        REQUIRE(std::holds_alternative<Forward>(commands[0]));
        CHECK(std::get<Forward>(commands[0]).n == 5);
        REQUIRE(std::holds_alternative<Down>(commands[1]));
        CHECK(std::get<Down>(commands[1]).n == 5);
        REQUIRE(std::holds_alternative<Forward>(commands[2]));
        CHECK(std::get<Forward>(commands[2]).n == 8);
        REQUIRE(std::holds_alternative<Up>(commands[3]));
        CHECK(std::get<Up>(commands[3]).n == 3);
        REQUIRE(std::holds_alternative<Down>(commands[4]));
        CHECK(std::get<Down>(commands[4]).n == 8);
        REQUIRE(std::holds_alternative<Forward>(commands[5]));
        CHECK(std::get<Forward>(commands[5]).n == 2);
    }

    SECTION("Plot Course")
    {
        CHECK(plotCourse({}) == Position{ .x = 0, .y = 0 });
        CHECK(plotCourse({ Forward{42} }) == Position{ .x = 42, .y = 0 });
        CHECK(plotCourse({ Down{23} }) == Position{ .x = 0, .y = 23 });
        CHECK(plotCourse({ Up{15} }) == Position{ .x = 0, .y = -15 });
        CHECK(plotCourse(commands) == Position{ .x = 15, .y = 10 });
    }

    SECTION("Plot Course 2")
    {
        CHECK(plotCourse2({}) == Submarine{ .x = 0, .y = 0, .aim = 0 });
        CHECK(plotCourse2({ Down{42} }) == Submarine{ .x = 0, .y = 0, .aim = 42 });
        CHECK(plotCourse2({ Up{23} }) == Submarine{ .x = 0, .y = 0, .aim = -23 });

        CHECK(plotCourse2(ranges::to<std::vector<Command>>(ranges::subrange(begin(commands), begin(commands) + 1))) == Submarine{ .x = 5, .y = 0, .aim = 0 });
        CHECK(plotCourse2(ranges::to<std::vector<Command>>(ranges::subrange(begin(commands), begin(commands) + 2))) == Submarine{ .x = 5, .y = 0, .aim = 5 });
        CHECK(plotCourse2(ranges::to<std::vector<Command>>(ranges::subrange(begin(commands), begin(commands) + 3))) == Submarine{ .x = 13, .y = 40, .aim = 5 });
        CHECK(plotCourse2(ranges::to<std::vector<Command>>(ranges::subrange(begin(commands), begin(commands) + 4))) == Submarine{ .x = 13, .y = 40, .aim = 2 });
        CHECK(plotCourse2(ranges::to<std::vector<Command>>(ranges::subrange(begin(commands), begin(commands) + 5))) == Submarine{ .x = 13, .y = 40, .aim = 10 });
        CHECK(plotCourse2(ranges::to<std::vector<Command>>(ranges::subrange(begin(commands), begin(commands) + 6))) == Submarine{ .x = 15, .y = 60, .aim = 10 });
    }

}
