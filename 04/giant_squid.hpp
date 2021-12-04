#ifndef ADVENT_OF_CODE_04_GIANT_SQUID_INCLUDE_GUARD
#define ADVENT_OF_CODE_04_GIANT_SQUID_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <cstdint>
#include <optional>
#include <string_view>
#include <vector>

struct Board {
    std::array<int32_t, 5*5> board;
    std::array<bool, 5*5> hits;
};

struct Game {
    std::vector<int32_t> numbers;
    std::vector<Board> boards;
};

Game parseInput(std::string_view input);

bool isHit(Board const& b, int x, int y);

bool markNumbers(Board& b, int32_t number);

struct Bingo {
    enum class Kind {
        Row,
        Column
    } kind;
    int i;
};

std::optional<Bingo> isBingo(Board const& b);

int64_t sumUnmarked(Board const& b);

std::optional<Bingo> playBoard(Board& b, int32_t number);

struct Outcome {
    int board_index;
    int64_t score;
};

std::optional<Outcome> playGame(Game& g, int32_t number);

int64_t result1(Game g);

int64_t result2(Game g);

#endif
