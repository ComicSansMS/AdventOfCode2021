#ifndef ADVENT_OF_CODE_01_SONAR_SWEEP_HPP_INCLUDE_GUARD
#define ADVENT_OF_CODE_01_SONAR_SWEEP_HPP_INCLUDE_GUARD

#include <cstdint>
#include <string_view>
#include <vector>

std::vector<int> parseInput(std::string_view input);

int64_t countDepthIncrease(std::vector<int> const& scan);

int64_t threeMeasurement(std::vector<int> const& scan);

#endif
