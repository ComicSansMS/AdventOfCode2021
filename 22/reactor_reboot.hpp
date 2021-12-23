#ifndef ADVENT_OF_CODE_22_REACTOR_REBOOT_INCLUDE_GUARD
#define ADVENT_OF_CODE_22_REACTOR_REBOOT_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <string_view>
#include <vector>

struct Point3 {
    int32_t x;
    int32_t y;
    int32_t z;

    friend bool operator==(Point3 const&, Point3 const&) = default;
};

struct AABB3 {
    Point3 min;
    Point3 max;

    friend bool operator==(AABB3 const&, AABB3 const&) = default;
};

struct RebootStep {
    enum class Turn {
        On,
        Off
    } turn;
    AABB3 region;

    friend bool operator==(RebootStep const&, RebootStep const&) = default;
};

std::vector<RebootStep> parseInput(std::string_view input);

void addStepToPartitioning(std::vector<RebootStep>& p, RebootStep const& step);

std::vector<RebootStep> partitionSteps(std::vector<RebootStep> const& s);

int64_t countPartitionedVolume(std::vector<RebootStep> const& p);

int64_t result1(std::vector<RebootStep> const& steps);

int64_t result2(std::vector<RebootStep> const& steps);

#endif
