#ifndef ADVENT_OF_CODE_05_HYDROTHERMAL_VENTURE_INCLUDE_GUARD
#define ADVENT_OF_CODE_05_HYDROTHERMAL_VENTURE_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <bitset>
#include <cstdint>
#include <limits>
#include <optional>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Point {
    int x;
    int y;
    friend constexpr bool operator==(Point const&, Point const&) noexcept = default;
};

inline std::ostream& operator<<(std::ostream& os, Point const& p) {
    return os << "{ x: " << p.x << " y: " << p.y << " }";
}

struct Line {
    Point start;
    Point end;

    friend constexpr bool operator==(Line const&, Line const&) noexcept = default;
};

inline std::ostream& operator<<(std::ostream& os, Line const& l) {
    return os << "{ start: " << l.start << " end: " << l.end << " }";
}

std::vector<Line> parseInput(std::string_view input);


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


struct Map {
    std::unordered_map<Point, int, HashPoint> m;
    Point min = { .x = std::numeric_limits<int>::max(), .y = std::numeric_limits<int>::max() };
    Point max = { .x = std::numeric_limits<int>::min(), .y = std::numeric_limits<int>::min() };
};

Map calculateMap(std::vector<Line> const& lines);

template<>
struct fmt::formatter<Map>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Map const& map, FormatContext& ctx) {
        for (int iy = map.min.y; iy <= map.max.y; ++iy) {
            for (int ix = map.min.x; ix <= map.max.x; ++ix) {
                auto const it = map.m.find(Point{ .x = ix, .y = iy });
                if ((it != end(map.m)) && (it->second != 0)) {
                    fmt::format_to(ctx.out(), "{}", it->second);
                } else {
                    fmt::format_to(ctx.out(), ".");
                }
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

int64_t result(Map const& m);

void addDiagonalLines(Map& m, std::vector<Line> const& lines);

#endif
