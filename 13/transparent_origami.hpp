#ifndef ADVENT_OF_CODE_13_TRANSPARENT_ORIGAMI_INCLUDE_GUARD
#define ADVENT_OF_CODE_13_TRANSPARENT_ORIGAMI_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

struct Point {
    int x;
    int y;

    friend bool operator==(Point const&, Point const&) = default;   // LCOV_EXCL_LINE
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

struct Fold {
    int coordinate;
    enum class Axis {
        X,
        Y
    } axis;
};

struct Transparency {
    std::unordered_set<Point, HashPoint> points;
    std::vector<Fold> folds;

    std::optional<std::size_t> activeFold;
    Point max;
};

template<>
struct fmt::formatter<Transparency>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Transparency const& tp, FormatContext& ctx) {
        auto const fold = (tp.activeFold) ? (std::optional<Fold>(tp.folds[*tp.activeFold])) : std::nullopt;
        for (int iy = 0; iy <= tp.max.y; ++iy) {
            for (int ix = 0; ix <= tp.max.x; ++ix) {
                if (fold && (((fold->axis == Fold::Axis::X) && (fold->coordinate == ix)) ||
                             ((fold->axis == Fold::Axis::Y) && (fold->coordinate == iy))))
                {
                    fmt::format_to(ctx.out(), ((fold->axis == Fold::Axis::X) ? "|" : "-"));
                } else if (tp.points.contains(Point{ .x = ix, .y = iy })) {
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


Transparency parseInput(std::string_view input);

Transparency fold(Transparency const& t);

std::size_t result1(Transparency t);

Transparency foldAll(Transparency const& t);

#endif
