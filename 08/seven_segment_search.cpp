#include <seven_segment_search.hpp>

#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/min.hpp>
#include <range/v3/algorithm/permutation.hpp>
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
    return ranges::accumulate(displays, static_cast<int64_t>(0), ranges::plus{}, [](Display const& d) -> int64_t {
            return ranges::count_if(d.code, isUnique);
        });
}

bool isValidMapping(Mapping const& m)
{
    return ranges::is_permutation(m.map, std::array<int, 7>{ 0, 1, 2, 3, 4, 5, 6 });
}

bool mapsTo0(Mapping const& m, Segment const& s)
{
    //  000
    // 1   2
    // 1   2
    //  ***
    // 4   5
    // 4   5
    //  666
    return s[m.map[0]] &&
           s[m.map[1]] &&
           s[m.map[2]] &&
           !s[m.map[3]] &&
           s[m.map[4]] &&
           s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsTo1(Mapping const& m, Segment const& s)
{
    //  ***
    // *   2
    // *   2
    //  ***
    // *   5
    // *   5
    //  ***
    return !s[m.map[0]] &&
           !s[m.map[1]] &&
           s[m.map[2]] &&
           !s[m.map[3]] &&
           !s[m.map[4]] &&
           s[m.map[5]] &&
           !s[m.map[6]];
}

bool mapsTo2(Mapping const& m, Segment const& s)
{
    //  000
    // *   2
    // *   2
    //  333
    // 4   *
    // 4   *
    //  666
    return s[m.map[0]] &&
           !s[m.map[1]] &&
           s[m.map[2]] &&
           s[m.map[3]] &&
           s[m.map[4]] &&
           !s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsTo3(Mapping const& m, Segment const& s)
{
    //  000
    // *   2
    // *   2
    //  333
    // *   5
    // *   5
    //  666
    return s[m.map[0]] &&
           !s[m.map[1]] &&
           s[m.map[2]] &&
           s[m.map[3]] &&
           !s[m.map[4]] &&
           s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsTo4(Mapping const& m, Segment const& s)
{
    //  ***
    // 1   2
    // 1   2
    //  333
    // *   5
    // *   5
    //  ***
    return !s[m.map[0]] &&
           s[m.map[1]] &&
           s[m.map[2]] &&
           s[m.map[3]] &&
           !s[m.map[4]] &&
           s[m.map[5]] &&
           !s[m.map[6]];
}

bool mapsTo5(Mapping const& m, Segment const& s)

{
    //  000
    // 1   *
    // 1   *
    //  333
    // *   5
    // *   5
    //  666
    return s[m.map[0]] &&
           s[m.map[1]] &&
           !s[m.map[2]] &&
           s[m.map[3]] &&
           !s[m.map[4]] &&
           s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsTo6(Mapping const& m, Segment const& s)

{
    //  000
    // 1   *
    // 1   *
    //  333
    // 4   5
    // 4   5
    //  666
    return s[m.map[0]] &&
           s[m.map[1]] &&
           !s[m.map[2]] &&
           s[m.map[3]] &&
           s[m.map[4]] &&
           s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsTo7(Mapping const& m, Segment const& s)
{
    //  000
    // *   2
    // *   2
    //  ***
    // *   5
    // *   5
    //  ***
    return s[m.map[0]] &&
           !s[m.map[1]] &&
           s[m.map[2]] &&
           !s[m.map[3]] &&
           !s[m.map[4]] &&
           s[m.map[5]] &&
           !s[m.map[6]];
}

bool mapsTo8(Mapping const& m, Segment const& s)
{
    //  000
    // 1   2
    // 1   2
    //  333
    // 4   5
    // 4   5
    //  666
    return s[m.map[0]] &&
           s[m.map[1]] &&
           s[m.map[2]] &&
           s[m.map[3]] &&
           s[m.map[4]] &&
           s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsTo9(Mapping const& m, Segment const& s)
{
    //  000
    // 1   2
    // 1   2
    //  333
    // *   5
    // *   5
    //  666
    return s[m.map[0]] &&
           s[m.map[1]] &&
           s[m.map[2]] &&
           s[m.map[3]] &&
           !s[m.map[4]] &&
           s[m.map[5]] &&
           s[m.map[6]];
}

bool mapsToAnyNumber(Mapping const& m, Segment const& s)
{
    return mapsTo0(m, s) ||
           mapsTo1(m, s) ||
           mapsTo2(m, s) ||
           mapsTo3(m, s) ||
           mapsTo4(m, s) ||
           mapsTo5(m, s) ||
           mapsTo6(m, s) ||
           mapsTo7(m, s) ||
           mapsTo8(m, s) ||
           mapsTo9(m, s);
}

Mapping determineValidMapping(Display const& d)
{
    Mapping m{ .map = { 0, 1, 2, 3, 4, 5, 6 } };
    auto const does_map = [&m](Segment const& s) { return mapsToAnyNumber(m, s); };
    while (ranges::next_permutation(m.map)) {
        if (ranges::all_of(d.patterns, does_map)) {
            break;
        }
    }
    assert(ranges::all_of(d.code, does_map));
    return m;
}

int64_t getDigit(Mapping const& m, Segment const& s)
{
    if (mapsTo0(m, s)) { return 0; }
    else if (mapsTo1(m, s)) { return 1; }
    else if (mapsTo2(m, s)) { return 2; }
    else if (mapsTo3(m, s)) { return 3; }
    else if (mapsTo4(m, s)) { return 4; }
    else if (mapsTo5(m, s)) { return 5; }
    else if (mapsTo6(m, s)) { return 6; }
    else if (mapsTo7(m, s)) { return 7; }
    else if (mapsTo8(m, s)) { return 8; }
    else {
        assert(mapsTo9(m, s)); { return 9; }
    }
}

int64_t getCode(Mapping const& m, Display const& d)
{
    int64_t ret = 0;
    for (auto const& s : d.code) {
        ret *= 10;
        ret += getDigit(m, s);
    }
    return ret;
}

int64_t result2(std::vector<Display> const& displays)
{
    return ranges::accumulate(displays, static_cast<int64_t>(0), ranges::plus{},
        [](Display const& d) -> int64_t { return getCode(determineValidMapping(d), d); });
}
