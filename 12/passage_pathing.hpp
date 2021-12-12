#ifndef ADVENT_OF_CODE_12_PASSAGE_PATHING_INCLUDE_GUARD
#define ADVENT_OF_CODE_12_PASSAGE_PATHING_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

struct Cave {
    std::string name;
    bool isBig;
    std::vector<std::size_t> neighbours;

    friend bool operator==(Cave const&, Cave const&) = default;
};

struct Edge {
    std::size_t start_index;
    std::size_t destination_index;
    friend bool operator==(Edge const&, Edge const&) = default;
};

struct Graph {
    std::vector<Cave> caves;
    std::vector<Edge> edges;

    std::size_t startCaveIndex;
    std::size_t endCaveIndex;
};

Graph parseInput(std::string_view input);

struct Path {
    std::vector<std::size_t> nodes;
};

std::vector<Path> allPaths(Graph const& g);

std::vector<Path> allPaths2(Graph const& g);

#endif
