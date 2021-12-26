#ifndef ADVENT_OF_CODE_25_SEA_CUCUMBER_INCLUDE_GUARD
#define ADVENT_OF_CODE_25_SEA_CUCUMBER_INCLUDE_GUARD

#include <fmt/format.h>

#include <cassert>
#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>

enum class Field {
    Empty,
    East,
    South
};

struct Map {
    int width;
    int height;
    std::vector<Field> v;
};

template<>
struct fmt::formatter<Map>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Map const& map, FormatContext& ctx) {
        for (int iy = 0; iy < map.height; ++iy) {
            for (int ix = 0; ix < map.width; ++ix) {
                Field const f = map.v[iy * map.width + ix];
                if (f == Field::Empty) {
                    fmt::format_to(ctx.out(), ".");
                } else if (f == Field::East) {
                    fmt::format_to(ctx.out(), ">");
                } else {
                    assert(f == Field::South);
                    fmt::format_to(ctx.out(), "v");
                }
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

Map parseInput(std::string_view input);

Map moveEast(Map const& m, std::size_t& cucumbers_moved);

Map moveSouth(Map const& m, std::size_t& cucumbers_moved);

struct StepReturn {
    Map m;
    std::size_t cucumbers_moved;
};

StepReturn step(Map const& m);

int play(Map const& m);

#endif
