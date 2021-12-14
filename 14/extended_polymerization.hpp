#ifndef ADVENT_OF_CODE_14_EXTENDED_POLYMERIZATION_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_EXTENDED_POLYMERIZATION_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

struct Rule {
    char first;
    char second;

    char insert;

    friend bool operator==(Rule const&, Rule const&) = default;     // LCOV_EXCL_LINE
};

// LCOV_EXCL_START
struct HashRule {
    std::size_t operator()(Rule const& r) const {
        return (r.first << 16) | r.second;
    }
};
// LCOV_EXCL_STOP

struct SplitRule {
    Rule r1;
    Rule r2;
};

using RuleProgression = std::unordered_map<Rule, SplitRule, HashRule>;
using RuleCount = std::unordered_map<Rule, int64_t, HashRule>;

struct Formula {
    std::string polymer;
    std::vector<Rule> rules;
};

Formula parseInput(std::string_view input);

struct OccurrenceCount {
    char c;
    int64_t count;
};

std::vector<OccurrenceCount> countOccurrences(RuleCount const& rc);

RuleProgression computeRuleProgression(Formula const& f);

RuleCount computeInitialRuleCount(Formula const& f);

RuleCount step(RuleCount const& rc, RuleProgression const& rp);

int64_t result1(Formula const& f);

int64_t result2(Formula const& f);



#endif
