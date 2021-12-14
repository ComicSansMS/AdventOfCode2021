#include <extended_polymerization.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>
#include <regex>
#include <tuple>

Formula parseInput(std::string_view input)
{
    Formula ret;
    enum class ParseState {
        Template,
        Empty,
        Rules
    } state = ParseState::Template;
    std::regex rx_formula(R"(^(\w)(\w) -> (\w)$)");
    for (auto const& rline : input | ranges::views::split('\n')) {
        if (state == ParseState::Template) {
            ret.polymer = ranges::to<std::string>(rline);
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
            char const second = *smatch[2].first;
            assert((second >= 'A') && (second <= 'Z'));
            char const inserted = *smatch[3].first;
            ret.rules.push_back(Rule{ .first = first, .second = second, .insert = inserted });
        }
    }
    std::sort(begin(ret.rules), end(ret.rules),
        [](Rule const& lhs, Rule const& rhs) { return std::tie(lhs.first, lhs.second) < std::tie(rhs.first, rhs.second); });
    return ret;
}

RuleProgression computeRuleProgression(Formula const& f)
{
    RuleProgression ret;
    for (auto const& r : f.rules) {
        auto const it1 = std::find_if(begin(f.rules), end(f.rules),
            [f = r.first, s = r.insert](Rule const& rr) { return rr.first == f && rr.second == s; });
        auto const it2 = std::find_if(begin(f.rules), end(f.rules),
            [f = r.insert, s = r.second](Rule const& rr) { return rr.first == f && rr.second == s; });
        assert(it1 != end(f.rules));
        assert(it2 != end(f.rules));
        ret[r] = SplitRule{ .r1 = *it1, .r2 = *it2 };
    }
    return ret;
}

RuleCount computeInitialRuleCount(Formula const& f)
{
    RuleCount ret;
    char first = '\0';
    auto it = begin(f.polymer);
    char second = *it++;
    for (; it != end(f.polymer); ++it) {
        first = second;
        second = *it;
        auto const it_rule = std::find_if(begin(f.rules), end(f.rules),
            [first, second](Rule const& rr) { return rr.first == first && rr.second == second; });
        assert(it_rule != end(f.rules));
        ++ret[*it_rule];
    }
    return ret;
}

RuleCount step(RuleCount const& rc, RuleProgression const& rp)
{
    RuleCount ret;
    for (auto const r : rc) {
        Rule const& rule = r.first;
        int64_t const count = r.second;
        auto it = rp.find(rule);
        assert(it != end(rp));
        ret[it->second.r1] += count;
        ret[it->second.r2] += count;
    }
    return ret;
}

std::vector<OccurrenceCount> countOccurrences(RuleCount const& rc)
{
    std::vector<OccurrenceCount> first;
    std::vector<OccurrenceCount> second;
    auto addCount = [](std::vector<OccurrenceCount>& v, char c, int64_t count) {
        auto it = std::find_if(begin(v), end(v), [c](OccurrenceCount const& oc) { return oc.c == c; });
        if (it == end(v)) {
            v.push_back(OccurrenceCount{ .c = c, .count = 0 });
            it = v.begin() + (v.size() - 1);
        }
        it->count += count;
    };
    for (auto const& r : rc) {
        addCount(first, r.first.first, r.second);
        addCount(second, r.first.second, r.second);
    }

    std::vector<OccurrenceCount> ret;
    for (char c = 'A'; c <= 'Z'; ++c) {
        auto const it_first = std::find_if(begin(first), end(first), [c](OccurrenceCount const& oc) { return oc.c == c; });
        auto const it_second = std::find_if(begin(second), end(second), [c](OccurrenceCount const& oc) { return oc.c == c; });
        int64_t count_first = (it_first != end(first)) ? (it_first->count) : 0;
        int64_t count_second = (it_second != end(second)) ? (it_second->count) : 0;
        if ((count_first != 0) || (count_second != 0)) {
            ret.push_back(OccurrenceCount{ .c = c, .count = std::max(count_first, count_second) });
        }
    }
    std::sort(begin(ret), end(ret),
        [](OccurrenceCount const& lhs, OccurrenceCount const& rhs) { return lhs.count < rhs.count; });
    return ret;
}

int64_t result(Formula const& f, int steps)
{
    auto const rp = computeRuleProgression(f);
    auto rc = computeInitialRuleCount(f);
    for (int i = 0; i < steps; ++i) {
        rc = step(rc, rp);
    }
    auto const oc = countOccurrences(rc);
    return oc[oc.size() - 1].count - oc[0].count;
}

int64_t result1(Formula const& f)
{
    return result(f, 10);
}

int64_t result2(Formula const& f)
{
    return result(f, 40);
}
