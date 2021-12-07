#ifndef ADVENT_OF_CODE_07_TREACHERY_OF_WHALES_INCLUDE_GUARD
#define ADVENT_OF_CODE_07_TREACHERY_OF_WHALES_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <cstdint>
#include <limits>
#include <optional>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Crabs {
    std::vector<int64_t> positions;
    int64_t min_position;
    int64_t max_position;
};

Crabs parseInput(std::string_view input);

int64_t costMovingTo(Crabs const& c, int64_t target);

int64_t result1(Crabs const& c);

int64_t costMovingTo2(Crabs const& c, int64_t target);

int64_t result2(Crabs const& c);


#endif
