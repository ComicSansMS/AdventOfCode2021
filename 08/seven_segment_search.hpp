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
    std::array<int, 7> map;
};

std::vector<Display> parseInput(std::string_view input);

int64_t result1(std::vector<Display> const& displays);

bool isValidMapping(Mapping const& m);
bool mapsTo0(Mapping const& m, Segment const& s);
bool mapsTo1(Mapping const& m, Segment const& s);
bool mapsTo2(Mapping const& m, Segment const& s);
bool mapsTo3(Mapping const& m, Segment const& s);
bool mapsTo4(Mapping const& m, Segment const& s);
bool mapsTo5(Mapping const& m, Segment const& s);
bool mapsTo6(Mapping const& m, Segment const& s);
bool mapsTo7(Mapping const& m, Segment const& s);
bool mapsTo8(Mapping const& m, Segment const& s);
bool mapsTo9(Mapping const& m, Segment const& s);

Mapping determineValidMapping(Display const& d);

int64_t getCode(Mapping const& m, Display const& d);

int64_t result2(std::vector<Display> const& displays);

#endif
