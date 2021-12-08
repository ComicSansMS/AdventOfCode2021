#ifndef ADVENT_OF_CODE_08_SEVEN_SEGMENT_SEARCH_INCLUDE_GUARD
#define ADVENT_OF_CODE_08_SEVEN_SEGMENT_SEARCH_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <cstdint>
#include <limits>
#include <optional>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <vector>

using Segment = std::bitset<7>;

struct Display {
    std::array<Segment, 10> patterns;
    std::array<Segment, 4> code;
};

struct Mapping {
    std::array<std::vector<int>, 7> map;
};

std::vector<Display> parseInput(std::string_view input);

int64_t result1(std::vector<Display> const& displays);

#endif
