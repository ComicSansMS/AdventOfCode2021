#include <seven_segment_search.hpp>

#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/min.hpp>
#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <regex>
#include <string>
#include <tuple>


std::vector<Display> parseInput(std::string_view input)
{
    std::vector<Display> ret;
    auto parse_signal = [](auto rng) {
        Segment ret = { 0 };
        auto it = ranges::begin(rng);
        auto const it_end = ranges::end(rng);
        for (; it != it_end; ++it) {
            char const c = *it;
            if ((c >= 'a') && (c <= 'g')) {
                assert(!ret[6 - (c - 'a')]);
                ret[6 - (c - 'a')] = true;
            } else {
                assert((c == ' ') || (c == '\n'));
                ++it;
                break;
            }
        }
        return std::make_tuple(ranges::subrange(it, it_end), ret);
    };
    for (auto const& rline : input | ranges::views::split('\n')) {
        auto it_rng = ranges::subrange(ranges::begin(rline), ranges::end(rline));
        ret.emplace_back();
        for (int i = 0; i < 10; ++i) {
            auto const [new_rng, segment] = parse_signal(it_rng);
            it_rng = new_rng;
            ret.back().patterns[i] = segment;
        }
        {
            auto it = ranges::begin(it_rng);
            assert(*it == '|');
            ++it;
            assert(*it == ' ');
            ++it;
            it_rng = ranges::subrange(it, ranges::end(it_rng));
        }
        for (int i = 0; i < 4; ++i) {
            auto const [new_rng, segment] = parse_signal(it_rng);
            it_rng = new_rng;
            ret.back().code[i] = segment;
        }
        assert(ranges::empty(it_rng));
    }
    return ret;
}

bool isUnique(Segment const& s)
{
    std::size_t const c = s.count();
    return (c == 2) || (c == 3) || (c == 4) || (c == 7);
}

int64_t result1(std::vector<Display> const& displays)
{
    return ranges::accumulate(displays, (int64_t)0, ranges::plus{}, [](Display const& d) -> int64_t {
            return ranges::count_if(d.code, isUnique);
        });
}
