#ifndef ADVENT_OF_CODE_02_DIVE_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_02_DIVE_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <variant>
#include <vector>

struct Forward {
    int64_t n;
};

struct Down {
    int64_t n;
};

struct Up {
    int64_t n;
};

using Command = std::variant<Forward, Down, Up>;

std::vector<Command> parseInput(std::string_view input);

struct Position {
    int64_t x;
    int64_t y;

    friend bool operator==(Position const&, Position const&) = default;
};
Position plotCourse(std::vector<Command> const& commands);

struct Submarine {
    int64_t x;
    int64_t y;
    int64_t aim;
    friend bool operator==(Submarine const&, Submarine const&) = default;
};
Submarine plotCourse2(std::vector<Command> const& commands);

#endif
