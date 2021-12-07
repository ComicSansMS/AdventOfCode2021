#include <treachery_of_whales.hpp>

#include <range/v3/algorithm/min.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <regex>
#include <string>


Crabs parseInput(std::string_view input)
{
    assert(input.back() == '\n');
    Crabs ret = {};
    ret.min_position = std::numeric_limits<int64_t>::max();
    ret.max_position = std::numeric_limits<int64_t>::min();
    for (auto const& str : input | ranges::views::split(',')) {
        ret.positions.push_back(std::stoi(ranges::to<std::string>(str)));
        ret.min_position = std::min(ret.min_position, ret.positions.back());
        ret.max_position = std::max(ret.max_position, ret.positions.back());
    }
    return ret;
}

int64_t costMovingTo(Crabs const& c, int64_t target)
{
    return ranges::accumulate(c.positions, (int64_t)0, ranges::plus{},
                              [target](int64_t pos) { return std::abs(target - pos); });
}

int64_t result(Crabs const& c, int64_t (*func)(Crabs const&, int64_t))
{
    return ranges::min(ranges::views::ints(c.min_position, c.max_position + 1)
         | ranges::views::transform([&c, func](int64_t i) { return func(c, i); }));
}

int64_t result1(Crabs const& c)
{
    return result(c, costMovingTo);
}

int64_t costMovingTo2(Crabs const& c, int64_t target)
{
    auto const gauss = [](int64_t n) -> int64_t { return (n*(n+1)) / 2; };
    return ranges::accumulate(c.positions, (int64_t)0, ranges::plus{},
                              [target, gauss](int64_t pos) { return gauss(std::abs(target - pos)); });
}

int64_t result2(Crabs const& c)
{
    return result(c, costMovingTo2);
}
