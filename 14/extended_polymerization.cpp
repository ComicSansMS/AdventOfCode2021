#include <extended_polymerization.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>
#include <regex>

Formula parseInput(std::string_view input)
{
    Formula ret;
    std::fill(begin(ret.insertion_rules.map), end(ret.insertion_rules.map), '?');
    enum class ParseState {
        Template,
        Empty,
        Rules
    } state = ParseState::Template;
    std::regex rx_formula(R"(^(\w)(\w) -> (\w)$)");
    for (auto const& rline : input | ranges::views::split('\n')) {
        if (state == ParseState::Template) {
            for (char const c : rline) {
                ret.polymer.chain.push_back(c);
            }
            state = ParseState::Empty;
        } else if (state == ParseState::Empty) {
            auto const line = ranges::to<std::string>(rline);
            assert(rline.empty());
            state = ParseState::Rules;
        } else {
            assert(state == ParseState::Rules);
            auto const line = ranges::to<std::string>(rline);
            std::smatch smatch;
            bool const does_match = std::regex_match(line, smatch, rx_formula);
            assert(does_match);
            char const first = *smatch[1].first;
            assert((first >= 'A') && (first <= 'Z'));
            int fi = first - 'A';
            char const second = *smatch[2].first;
            assert((second >= 'A') && (second <= 'Z'));
            int si = second - 'A';
            char const inserted = *smatch[3].first;
            ret.insertion_rules.map[fi*26 + si] = inserted;
        }
    }
    return ret;
}

void polymerize(Formula& f)
{
    assert(f.polymer.chain.size() > 2);
    auto it = f.polymer.chain.begin();
    auto const it_end = f.polymer.chain.end();
    char second = *it;
    ++it;
    char first = '\0';
    for (; it != it_end; ++it) {
        char const c = *it;
        first = second;
        second = c;
        char const insert = f.insertion_rules(first, second);
        if (insert != '?') {
            it = f.polymer.chain.insert(it, insert);
            ++it;
        }
    }
}

std::vector<OccurrenceCount> countOccurrences(Polymer const& p)
{
    std::vector<OccurrenceCount> ret;
    for (char const c : p.chain) {
        auto it = std::find_if(begin(ret), end(ret), [c](OccurrenceCount const& oc) { return oc.c == c; });
        if (it == end(ret)) {
            ret.push_back(OccurrenceCount{ .c = c, .count = 0 });
            it = begin(ret) + (ret.size() - 1);
        }
        ++it->count;
    }
    std::sort(begin(ret), end(ret),
        [](OccurrenceCount const& lhs, OccurrenceCount const& rhs) { return lhs.count < rhs.count; });
    return ret;
}

int64_t result1(Formula f)
{
    for (int i = 0; i < 10; ++i) {
        polymerize(f);
    }
    auto const oc = countOccurrences(f.polymer);
    return oc[oc.size() - 1].count - oc[0].count;
}
