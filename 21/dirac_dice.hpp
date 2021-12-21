#ifndef ADVENT_OF_CODE_21_DIRAC_DICE_INCLUDE_GUARD
#define ADVENT_OF_CODE_21_DIRAC_DICE_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <string_view>
#include <unordered_map>
#include <vector>

struct State {
    int p1_pos;
    int p2_pos;
    int p1_score;
    int p2_score;

    friend constexpr bool operator==(State const&, State const&) noexcept = default;        // LCOV_EXCL_LINE
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashState {
    std::size_t operator()(State const& s) const {
        std::size_t seed = 0;
        hash_combine(seed, s.p1_pos);
        hash_combine(seed, s.p2_pos);
        hash_combine(seed, s.p1_score);
        hash_combine(seed, s.p2_score);
        return seed;
    }
};

struct Board {
    int player1;
    int player2;
};

Board parseInput(std::string_view input);

int movePlayer(int current_position, int die1, int die2, int die3);

int64_t result1(Board b);

int64_t result2(Board b);

#endif
