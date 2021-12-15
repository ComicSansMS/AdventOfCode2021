#ifndef ADVENT_OF_CODE_15_CHITON_INCLUDE_GUARD
#define ADVENT_OF_CODE_15_CHITON_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <string_view>
#include <vector>

struct Map {
    int width;
    int height;
    std::vector<int> m;
};

int getIndex(Map const& m, int x, int y);

Map parseInput(std::string_view input);

struct Point {
    int x;
    int y;
};

struct Path {
    std::vector<Point> p;
    int total_cost;
};

int shortestPath(Map const& m);

Map expandMap(Map const& m);

#endif
