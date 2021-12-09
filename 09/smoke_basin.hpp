#ifndef ADVENT_OF_CODE_09_SMOKE_BASIN_INCLUDE_GUARD
#define ADVENT_OF_CODE_09_SMOKE_BASIN_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <string_view>
#include <vector>

struct Heightmap {
    int width;
    int height;
    std::vector<int> map;
};

Heightmap parseInput(std::string_view input);

int getCell(Heightmap const& map, int x, int y);

template<>
struct fmt::formatter<Heightmap>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Heightmap const& map, FormatContext& ctx) {
        for (int iy = 0; iy < map.height; ++iy) {
            for (int ix = 0; ix < map.width; ++ix) {
                fmt::format_to(ctx.out(), "{}", getCell(map, ix, iy));
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

bool isLowerThan4Neighbourhood(Heightmap const& map, int x, int y);

int getRiskLevel(Heightmap const& map, int x, int y);

struct Point {
    int x;
    int y;
    friend bool operator<(Point const& lhs, Point const& rhs) {
        return std::tie(lhs.y, lhs.x) < std::tie(rhs.y, rhs.x);
    }
    friend bool operator==(Point const&, Point const&) = default;
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashPoint {
    std::size_t operator()(Point const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        return seed;
    }
};

std::vector<Point> getLowPoints(Heightmap const& map);

int result1(Heightmap const& map);

struct Basin {
    std::vector<Point> points;
};

std::vector<Basin> partitionBasins(Heightmap const& map);

int64_t result2(Heightmap const& map);

#endif
