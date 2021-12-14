#ifndef ADVENT_OF_CODE_14_EXTENDED_POLYMERIZATION_INCLUDE_GUARD
#define ADVENT_OF_CODE_14_EXTENDED_POLYMERIZATION_INCLUDE_GUARD

#include <fmt/format.h>

#include <array>
#include <cassert>
#include <cstdint>
#include <optional>
#include <list>
#include <memory>
#include <memory_resource>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>


struct Polymer {
    std::pmr::monotonic_buffer_resource allocator;
    std::pmr::list<char> chain;

    Polymer()
        :chain(&allocator)
    {}

    Polymer(Polymer const& rhs)
        :chain(rhs.chain, &allocator)
    {}

    Polymer& operator=(Polymer const& rhs) {
        if (this != &rhs) {
            chain = rhs.chain;
        }
        return *this;
    }

    operator std::string() const {
        std::string ret;
        ret.reserve(chain.size());
        for (char const c : chain) { ret.push_back(c); }
        return ret;
    }
};

struct RuleLookup {
    std::array<char, 26*26> map;

    char operator()(char first, char second) const {
        assert((first >= 'A') && (first <= 'Z'));
        assert((second >= 'A') && (second <= 'Z'));
        int const fi = first - 'A';
        int const si = second - 'A';
        return map[fi*26 + si];
    }
};

struct Formula {
    Polymer polymer;
    RuleLookup insertion_rules;
};

Formula parseInput(std::string_view input);

void polymerize(Formula& f);

struct OccurrenceCount {
    char c;
    int64_t count;
};

std::vector<OccurrenceCount> countOccurrences(Polymer const& p);

int64_t result1(Formula f);

#endif
