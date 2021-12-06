#ifndef ADVENT_OF_CODE_06_LANTERNFISH_INCLUDE_GUARD
#define ADVENT_OF_CODE_06_LANTERNFISH_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <cstdint>
#include <limits>
#include <optional>
#include <ostream>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Fishes {
    std::array<int64_t, 10> a;
};

Fishes parseInput(std::string_view input);

Fishes simulateDay(Fishes fishes);

int64_t result(Fishes fishes, int n_days);

#endif
