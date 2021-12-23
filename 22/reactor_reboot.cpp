#include <reactor_reboot.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <regex>

std::vector<RebootStep> parseInput(std::string_view input)
{
    std::regex rx_line(R"((on|off) x=(\-?\d+)\.\.(\-?\d+),y=(\-?\d+)\.\.(\-?\d+),z=(\-?\d+)\.\.(\-?\d+))");

    using regex_it = std::regex_iterator<std::string_view::iterator>;
    auto const it_begin = regex_it(begin(input), end(input), rx_line);
    auto const it_end = regex_it();

    std::vector<RebootStep> ret{};
    std::transform(it_begin, it_end, std::back_inserter(ret),
        [](std::match_results<std::string_view::iterator> const& match) -> RebootStep {
            RebootStep s;
            s.turn = (match[1] == "on") ? RebootStep::Turn::On : RebootStep::Turn::Off;
            s.region.min = Point3{ .x = std::stoi(match[2]), .y = std::stoi(match[4]), .z = std::stoi(match[6]) };
            s.region.max = Point3{ .x = std::stoi(match[3]), .y = std::stoi(match[5]), .z = std::stoi(match[7]) };
            assert((s.region.min.x <= s.region.max.x) && (s.region.min.y <= s.region.max.y) && (s.region.min.z <= s.region.max.z));
            return s;
        });
    return ret;
}

inline constexpr AABB3 intersect(AABB3 const& b1, AABB3 const& b2) noexcept {
    return AABB3{ .min = Point3{ .x = std::max(b1.min.x, b2.min.x), .y = std::max(b1.min.y, b2.min.y), .z = std::max(b1.min.z, b2.min.z) },
                  .max = Point3{ .x = std::min(b1.max.x, b2.max.x), .y = std::min(b1.max.y, b2.max.y), .z = std::min(b1.max.z, b2.max.z) } };
}

inline bool intersects(AABB3 const& b1, AABB3 const& b2)
{
    // intersects only if there is an overlap on all axes
    if ((b1.max.x < b2.min.x) || (b1.min.x > b2.max.x) ||
        (b1.max.y < b2.min.y) || (b1.min.y > b2.max.y) ||
        (b1.max.z < b2.min.z) || (b1.min.z > b2.max.z))
    {
        return false;
    }
    return true;
}

void addStepToPartitioning(std::vector<RebootStep>& p, RebootStep const& step)
{
    if (step.turn == RebootStep::Turn::On) { p.push_back(step); }
    if (p.size() < 2) { return; }
    std::size_t const i_end = (step.turn == RebootStep::Turn::On) ? (p.size() - 1) : p.size();
    for (std::size_t i = 0; i != i_end; ++i) {
        RebootStep const& s = p[i];
        if (intersects(s.region, step.region)) {
            auto const intersection = intersect(s.region, step.region);
            if ((s.turn == RebootStep::Turn::On) && (step.turn == RebootStep::Turn::On)) {
                p.push_back(RebootStep{ .turn = RebootStep::Turn::Off, .region = intersection });
            } else if ((s.turn == RebootStep::Turn::On) && (step.turn == RebootStep::Turn::Off)) {
                p.push_back(RebootStep{ .turn = RebootStep::Turn::Off, .region = intersection });
            } else if ((s.turn == RebootStep::Turn::Off) && (step.turn == RebootStep::Turn::On)) {
                p.push_back(RebootStep{ .turn = RebootStep::Turn::On, .region = intersection });
            } else {
                assert((s.turn == RebootStep::Turn::Off) && (step.turn == RebootStep::Turn::Off));
                p.push_back(RebootStep{ .turn = RebootStep::Turn::On, .region = intersection });
            }
        }
    }
}

std::vector<RebootStep> partitionSteps(std::vector<RebootStep> const& s)
{
    std::vector<RebootStep> ret;
    for (auto const step : s) {
        addStepToPartitioning(ret, step);
    }
    return ret;
}

int64_t countPartitionedVolume(std::vector<RebootStep> const& p)
{
    int64_t ret = 0;
    for (auto const& s : p) {
        int64_t const len_x = ((s.region.max.x + 1) - s.region.min.x);
        int64_t const len_y = ((s.region.max.y + 1) - s.region.min.y);
        int64_t const len_z = ((s.region.max.z + 1) - s.region.min.z);
        int64_t const vol = len_x * len_y * len_z;
        if (s.turn == RebootStep::Turn::On) {
            ret += vol;
        } else {
            ret -= vol;
        }
    }
    return ret;
}

int64_t result1(std::vector<RebootStep> const& steps)
{
    std::vector<RebootStep> ps;
    for (auto const& s : steps) {
        if ((s.region.min.x >= -50) && (s.region.max.x <= 50)) {
            addStepToPartitioning(ps, s);
        }
    }
    return countPartitionedVolume(ps);
}

int64_t result2(std::vector<RebootStep> const& steps)
{
    return countPartitionedVolume(partitionSteps(steps));
}
