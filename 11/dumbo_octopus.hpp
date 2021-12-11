#ifndef ADVENT_OF_CODE_11_DUMBO_OCTOPUS_INCLUDE_GUARD
#define ADVENT_OF_CODE_11_DUMBO_OCTOPUS_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <string_view>

struct Map {
    std::array<int, 100> a;
    int64_t flashes;
};

Map parseInput(std::string_view input);

template<>
struct fmt::formatter<Map>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Map const& map, FormatContext& ctx) {
        for (int iy = 0; iy < 10; ++iy) {
            for (int ix = 0; ix < 10; ++ix) {
                fmt::format_to(ctx.out(), "{}", map.a[iy*10 + ix]);
            }
            fmt::format_to(ctx.out(), "\n");
        }
        return ctx.out();
    }
};

Map step(Map const& m);

int64_t result1(Map const& m);

int64_t result2(Map const& m);

#endif
