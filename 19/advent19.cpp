#include <beacon_scanner.hpp>

#include <fmt/format.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

std::optional<std::string> readInput(char const* filename)
{
    std::ifstream fin(filename);
    if(!fin) {
        fmt::print(stderr, "Unable to open input file '{}' for reading.\n", filename);
        return std::nullopt;
    }

    std::stringstream sstr;
    sstr << fin.rdbuf();
    if(!fin) {
        fmt::print(stderr, "Unable to read input from file '{}'.\n", filename);
        return std::nullopt;
    }
    return sstr.str();
}

int main(int argc, char* argv[])
{
    char const* input_filename = "input";
    if(argc == 2) {
        input_filename = argv[1];
    }

    auto const input = readInput(input_filename);

    if(!input) {
        return 1;
    }

    auto const scans = parseInput(*input);
    //auto const t0 = std::chrono::steady_clock::now();
    auto const matched_scans = matchAllScans(scans);
    //auto const t1 = std::chrono::steady_clock::now();
    //fmt::print("Matching took {}ms.\n", std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
    fmt::print("First result is {}\n", countBeacons(matched_scans));
    fmt::print("Second result is {}\n", largestDistance(matched_scans));

    return 0;
}
