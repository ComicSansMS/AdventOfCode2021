#include <smoke_basin.hpp>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <unordered_set>

Heightmap parseInput(std::string_view input)
{
    Heightmap ret;
    ret.width = -1;
    for (auto const& rline : input | ranges::views::split('\n')) {
        for (char const c : rline) {
            assert((c >= '0') && (c <= '9'));
            ret.map.push_back(c - '0');
        }
        if (ret.width == -1) {
            ret.width = static_cast<int>(ret.map.size());
        }
        assert((ret.map.size() % ret.width) == 0);
    }
    ret.height = static_cast<int>(ret.map.size()) / ret.width;
    return ret;
}

int getCell(Heightmap const& map, int x, int y)
{
    assert((x >= 0) && (x < map.width) && (y >= 0) && (y < map.height));
    return map.map[y*map.width + x];
}

bool visit4Neighbourhood(Heightmap const& map, int x, int y, auto func)
{
    int const c = getCell(map, x, y);
    if (x > 0) {
        int const left = getCell(map, x-1, y);
        if (func(c, left)) { return false; }
    }
    if (x < map.width - 1) {
        int const right = getCell(map, x+1, y);
        if (func(c, right)) { return false; }
    }
    if (y > 0) {
        int const up = getCell(map, x, y-1);
        if (func(c, up)) { return false; }
    }
    if (y < map.height - 1) {
        int const down = getCell(map, x, y+1);
        if (func(c, down)) { return false; }
    }
    return true;
}

bool isLowerThan4Neighbourhood(Heightmap const& map, int x, int y)
{
    return visit4Neighbourhood(map, x, y, ranges::greater_equal{});
}

int getRiskLevel(Heightmap const& map, int x, int y)
{
    return getCell(map, x, y) + 1;
}

std::vector<Point> getLowPoints(Heightmap const& map)
{
    return 
        ranges::views::cartesian_product(ranges::views::iota(0, map.height),
                                         ranges::views::iota(0, map.width))
        | ranges::views::transform([](auto yx) -> Point {
                auto [y, x] = yx;
                return Point{ .x = x, .y = y };
            })
        | ranges::views::filter([&map](auto p) {
                return isLowerThan4Neighbourhood(map, p.x, p.y);
            })
        | ranges::to<std::vector<Point>>;
}

int result1(Heightmap const& map)
{
    auto const low_points = getLowPoints(map);
    return ranges::accumulate(
        low_points
        | ranges::views::transform([&map](Point const& p) -> int {
                return getRiskLevel(map, p.x, p.y);
            }), 0);
}

Basin floodFill(Heightmap const& map, Point const& p)
{
    std::unordered_set<Point, HashPoint> visited;
    Basin ret;
    ret.points.push_back(p);
    int processed = 0;
    auto const isPartOfBasin = [](int c, int p) {
        return (p < 9) && (c <= p);
    };
    while (processed < ret.points.size()) {
        auto const [x, y] = ret.points[processed];
        int const c = getCell(map, x, y);
        if (x > 0) {
            Point pleft = Point{ x-1, y };
            if (!visited.contains(pleft)) {
                int const left = getCell(map, pleft.x, pleft.y);
                if (isPartOfBasin(c, left)) {
                    ret.points.push_back(pleft);
                    visited.insert(pleft);
                }
            }
        }
        if (x < map.width - 1) {
            Point pright = Point{ x+1, y };
            if (!visited.contains(pright)) {
                int const right = getCell(map, pright.x, pright.y);
                if (isPartOfBasin(c, right)) {
                    ret.points.push_back(pright);
                    visited.insert(pright);
                }
            }
        }
        if (y > 0) {
            Point pup = Point{ x, y-1 };
            if (!visited.contains(pup)) {
                int const up = getCell(map, pup.x, pup.y);
                if (isPartOfBasin(c, up)) {
                    ret.points.push_back(pup);
                    visited.insert(pup);
                }
            }
        }
        if (y < map.height - 1) {
            Point pdown = Point{ x, y+1 };
            if (!visited.contains(pdown)) {
                int const down = getCell(map, pdown.x, pdown.y);
                if (isPartOfBasin(c, down)) {
                    ret.points.push_back(pdown);
                    visited.insert(pdown);
                }
            }
        }
        ++processed;
    }
    return ret;
}

std::vector<Basin> partitionBasins(Heightmap const& map)
{
    std::vector<Basin> ret;
    auto const low_points = getLowPoints(map);
    for (auto const& p : low_points) {
        Basin basin = floodFill(map, p);
        std::sort(begin(basin.points), end(basin.points));
        ret.emplace_back(std::move(basin));
    }
    return ret;
}

int64_t result2(Heightmap const& map)
{
    auto basins = partitionBasins(map);
    auto const max_basin = [](Basin const& lhs, Basin const& rhs) { return lhs.points.size() > rhs.points.size(); };
    std::partial_sort(begin(basins), begin(basins) + 3, end(basins), max_basin);
    return basins[0].points.size() * basins[1].points.size() * basins[2].points.size();
}
