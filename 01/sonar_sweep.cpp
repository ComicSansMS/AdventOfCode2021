#include <sonar_sweep.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>
#include <range/v3/view/adjacent_filter.hpp>

#include <cassert>
#include <optional>
#include <string>

std::vector<int> parseInput(std::string_view input)
{
    return ranges::to<std::vector<int>>(input | ranges::views::split('\n') |
        ranges::views::transform([](auto sv) {
            return std::stoi(ranges::to<std::string>(sv));
        }));
}

int64_t countDepthIncrease(std::vector<int> const& scan)
{
    return ranges::distance(scan | ranges::views::adjacent_filter(std::less{})) - 1;
}

int64_t threeMeasurement(std::vector<int> const& scan)
{
    assert(scan.size() > 3);
    std::array<int, 3> window{ scan[0], scan[1], scan[2] };
    int window_sum = ranges::accumulate(window, 0);
    int last_sum = 0;
    int ret = 0;
    for (std::size_t i = 3, s = scan.size(); i < s; ++i) {
        int const window_index = i % 3;
        last_sum = window_sum;
        window_sum += (-window[window_index]) + scan[i];
        window[window_index] = scan[i];
        if (window_sum > last_sum) {
            ++ret;
        }
    }
    return ret;
}
