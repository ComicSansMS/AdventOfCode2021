#ifndef ADVENT_OF_CODE_20_TRENCH_MAP_INCLUDE_GUARD
#define ADVENT_OF_CODE_20_TRENCH_MAP_INCLUDE_GUARD

#include <fmt/format.h>

#include <bitset>
#include <cstdint>
#include <ostream>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Point {
    int x;
    int y;

    friend constexpr bool operator==(Point const&, Point const&) noexcept = default;
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


struct Image {
    std::bitset<512> lookup;
    Point min;
    Point max;
    std::unordered_set<Point, HashPoint> points;
};

template<>
struct fmt::formatter<Image>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Image const& i, FormatContext& ctx) {
        for (int iy = i.min.y; iy <= i.max.y; ++iy) {
            for (int ix = i.min.x; ix <= i.max.x; ++ix) {
                Point const p{ .x = ix, .y = iy };
                if (i.points.contains(p)) {
                    fmt::format_to(ctx.out(), "#");
                } else {
                    fmt::format_to(ctx.out(), ".");
                }
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

Image parseInput(std::string_view input);

int16_t neigbourhood2Number(Image const& input, Point const& p, bool out_of_bounds_are_0);

Image step(Image const& input_img, bool out_of_bounds_are_0);

int64_t enhance(Image const& input_img, int steps);

#endif
