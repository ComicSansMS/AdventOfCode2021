#include <sonar_sweep.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/sliding.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/adjacent_filter.hpp>

#include <cassert>
#include <optional>
#include <string>

std::vector<int> parseInput(std::string_view input)
{
    return input
        | ranges::views::split('\n')
        | ranges::views::transform([](auto sv) {
                return std::stoi(ranges::to<std::string>(sv));
            })
        | ranges::to<std::vector>;
}

int64_t countIncreases(ranges::forward_range auto const& rng)
{
    return ranges::distance(rng | ranges::views::adjacent_filter(std::less{})) - 1;
}

int64_t countDepthIncrease(std::vector<int> const& scan)
{
    return countIncreases(scan);
}

int64_t threeMeasurement(std::vector<int> const& scan)
{
    assert(scan.size() > 3);
    return countIncreases(
        scan
        | ranges::views::sliding(3)
        | ranges::views::transform([](auto const& window) { return ranges::accumulate(window, 0); }));
}
