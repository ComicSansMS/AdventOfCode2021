#include <trick_shot.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <regex>

TargetArea parseInput(std::string_view input)
{
    std::regex const rx_input(R"(^target area: x=([-]?\d+)\.\.([-]?\d+), y=([-]?\d+)\.\.([-]?\d+)[\n]?$)");
    std::match_results<std::string_view::iterator> smatch;
    bool const does_match = std::regex_match(begin(input), end(input), smatch, rx_input);
    assert(does_match);

    TargetArea ret{};
    ret.min.x = std::stoi(smatch[1].str());
    ret.max.x = std::stoi(smatch[2].str());
    ret.min.y = std::stoi(smatch[3].str());
    ret.max.y = std::stoi(smatch[4].str());
    assert(ret.min.x < ret.max.x);
    assert(ret.min.y < ret.max.y);
    return ret;
}

Probe step(Probe p)
{
    p.position.x += p.velocity.x;
    p.position.y += p.velocity.y;
    if (p.velocity.x > 0) { --p.velocity.x; } else if (p.velocity.x < 0) { ++p.velocity.x; }
    --p.velocity.y;
    return p;
}

bool probeIsInTarget(Probe const& p, TargetArea const& target)
{
    return (p.position.x >= target.min.x) && (p.position.x <= target.max.x) &&
           (p.position.y >= target.min.y) && (p.position.y <= target.max.y);
}

bool didOvershoot(Probe const& p, TargetArea const& target)
{
    return (p.velocity.y < 0) && (p.position.y < target.min.y);
}

int determineMinInitialVelocityX(TargetArea const& target)
{
    int const max_vx = target.max.x;
    int max_x_for_ix = 0;
    int ix = 1;
    for (; ix <= max_vx; ++ix) {
        max_x_for_ix += ix;
        if (max_x_for_ix >= target.min.x) { break; }
    }
    return ix;
}

int findHighestHeight(TargetArea const& target)
{
    int const max_vx = target.max.x;
    int const min_vx = determineMinInitialVelocityX(target);
    int const min_vy = target.min.y;
    int const max_vy = -target.min.y;

    int max_y = 0;
    for (int ix = min_vx; ix <= max_vx; ++ix) {
        for (int iy = min_vy; iy <= max_vy; ++iy) {
            Vector const v{ .x = ix, .y = iy };
            Probe p{ .position = {.x = 0, .y = 0 }, .velocity = v };
            int max_y_for_v = 0;
            bool did_hit = false;
            while (!didOvershoot(p, target) && !did_hit) {
                p = step(p);
                max_y_for_v = std::max(max_y_for_v, p.position.y);
                did_hit = probeIsInTarget(p, target);
            }
            if (did_hit) {
                max_y = std::max(max_y, max_y_for_v);
            }
        }
    }
    return max_y;
}

int countValidVelocities(TargetArea const& target)
{
    int const max_vx = target.max.x;
    int const min_vx = determineMinInitialVelocityX(target);
    int const min_vy = target.min.y;
    int const max_vy = -target.min.y;

    int count = 0;
    for (int ix = min_vx; ix <= max_vx; ++ix) {
        for (int iy = min_vy; iy <= max_vy; ++iy) {
            Vector const v{ .x = ix, .y = iy };
            Probe p{ .position = {.x = 0, .y = 0 }, .velocity = v };
            bool did_overshoot = false;
            bool did_hit = false;
            for (int istep = 0; !did_overshoot && !did_hit; ++istep) {
                p = step(p);
                did_hit = probeIsInTarget(p, target);
                did_overshoot = didOvershoot(p, target);
            }
            if (did_hit) {
                ++count;
            }
        }
    }
    return count;
}
