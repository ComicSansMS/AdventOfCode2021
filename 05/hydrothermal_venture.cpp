#include <hydrothermal_venture.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <regex>
#include <string>

std::vector<Line> parseInput(std::string_view input)
{
    std::vector<Line> ret;
    std::regex const rx_line(R"(^(\d+),(\d+) -> (\d+),(\d+)$)");
    std::smatch smatch;
    for (auto const& line : input | ranges::views::split('\n')) {
        std::string const l = ranges::to<std::string>(line);
        bool const does_match = std::regex_match(l, smatch, rx_line);
        assert(does_match);
        ret.emplace_back();
        ret.back().start = Point{ .x = std::stoi(smatch[1]),
                                  .y = std::stoi(smatch[2]) };
        ret.back().end = Point{ .x = std::stoi(smatch[3]),
                                .y = std::stoi(smatch[4]) };
    }
    return ret;
}

Map calculateMap(std::vector<Line> const& lines)
{
    Map ret;
    for (auto const& l : lines) {
        ret.min.x = std::min(ret.min.x, l.start.x);
        ret.min.y = std::min(ret.min.y, l.start.y);
        ret.min.x = std::min(ret.min.x, l.end.x);
        ret.min.y = std::min(ret.min.y, l.end.y);
        ret.max.x = std::max(ret.max.x, l.start.x);
        ret.max.y = std::max(ret.max.y, l.start.y);
        ret.max.x = std::max(ret.max.x, l.end.x);
        ret.max.y = std::max(ret.max.y, l.end.y);

        if (l.start.x == l.end.x) {
            // vertical line
            for (int iy = std::min(l.start.y, l.end.y), iy_max = std::max(l.start.y, l.end.y); iy <= iy_max; ++iy) {
                Point const p{ l.start.x, iy };
                ++ret.m[p];
            }
        } else if (l.start.y == l.end.y) {
            // horizontal line
            for (int ix = std::min(l.start.x, l.end.x), ix_max = std::max(l.start.x, l.end.x); ix <= ix_max; ++ix) {
                Point const p{ ix,  l.start.y };
                ++ret.m[p];
            }
        }
    }
    return ret;
}

int64_t result(Map const& map)
{
    return std::count_if(begin(map.m), end(map.m), 
                         [](auto const& p) { return p.second >= 2; });
}

void addDiagonalLines(Map& m, std::vector<Line> const& lines)
{
    for (auto const& l : lines) {
        if ((l.start.x != l.end.x) && (l.start.y != l.end.y)) {
            // diagonal line
            assert(std::abs(l.end.x - l.start.x) == std::abs(l.end.y - l.start.y));
            int const n_steps = std::abs(l.end.x - l.start.x);
            Point p{ l.start.x, l.start.y };
            for (int i = 0; i <= n_steps; ++i) {
                ++m.m[p];
                p.x += (l.end.x > l.start.x) ? (+1) : (-1);
                p.y += (l.end.y > l.start.y) ? (+1) : (-1);
            }
        }
    }
}
