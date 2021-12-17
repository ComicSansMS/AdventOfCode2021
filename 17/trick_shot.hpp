#ifndef ADVENT_OF_CODE_17_TRICK_SHOT_INCLUDE_GUARD
#define ADVENT_OF_CODE_17_TRICK_SHOT_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <memory>
#include <ostream>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

struct Point {
    int x;
    int y;

    friend bool operator==(Point const&, Point const&) = default;
};

template<>
struct fmt::formatter<Point>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Point const& p, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "{{ .x = {}, .y = {} }}", p.x, p.y);
        return ctx.out();
    }
};

inline std::ostream& operator<<(std::ostream& os, Point const& p) {
    return os << fmt::format("{}", p);
}

struct TargetArea {
    Point min;
    Point max;
};

TargetArea parseInput(std::string_view input);

struct Vector {
    int x;
    int y;

    friend bool operator==(Vector const&, Vector const&) = default;
};

struct Probe {
    Point position;
    Vector velocity;
};

Probe step(Probe p);

bool probeIsInTarget(Probe const& p, TargetArea const& target);

bool didOvershoot(Probe const& p, TargetArea const& target);

int findHighestHeight(TargetArea const& target);

int countValidVelocities(TargetArea const& target);

#endif
