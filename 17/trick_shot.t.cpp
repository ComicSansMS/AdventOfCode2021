#include <trick_shot.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Trick Shot")
{
    char const sample_input[] = "target area: x=20..30, y=-10..-5\n";

    SECTION("Point")
    {
        std::stringstream sstr;
        sstr << Point{ .x = 42, .y = -512 };
        CHECK(sstr.str() == "{ .x = 42, .y = -512 }");
    }

    TargetArea const target_area = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(target_area.min == Point{ .x = 20, .y = -10 });
        CHECK(target_area.max == Point{ .x = 30, .y = -5 });
    }

    SECTION("Step")
    {
        Probe p{ .position = {.x = 0, .y = 0 }, .velocity = {.x = 7, .y = 2 } };
        p = step(p);
        CHECK(p.position == Point{ .x = 7, .y = 2 });
        CHECK(p.velocity == Vector{ .x = 6, .y = 1 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 13, .y = 3 });
        CHECK(p.velocity == Vector{ .x = 5, .y = 0 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 18, .y = 3 });
        CHECK(p.velocity == Vector{ .x = 4, .y = -1 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 22, .y = 2 });
        CHECK(p.velocity == Vector{ .x = 3, .y = -2 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 25, .y = 0 });
        CHECK(p.velocity == Vector{ .x = 2, .y = -3 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 27, .y = -3 });
        CHECK(p.velocity == Vector{ .x = 1, .y = -4 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 28, .y = -7 });
        CHECK(p.velocity == Vector{ .x = 0, .y = -5 });
        CHECK(probeIsInTarget(p, target_area));
        CHECK(!didOvershoot(p, target_area));
        p = step(p);
        CHECK(p.position == Point{ .x = 28, .y = -12 });
        CHECK(p.velocity == Vector{ .x = 0, .y = -6 });
        CHECK(!probeIsInTarget(p, target_area));
        CHECK(didOvershoot(p, target_area));
    }

    SECTION("Find Highest Height")
    {
        CHECK(findHighestHeight(target_area) == 45);
    }

    SECTION("Count Valid Velocities")
    {
        CHECK(countValidVelocities(target_area) == 112);
    }
}
