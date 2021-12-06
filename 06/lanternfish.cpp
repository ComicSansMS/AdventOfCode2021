#include <lanternfish.hpp>

#include <range/v3/numeric/accumulate.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <regex>
#include <string>


Fishes parseInput(std::string_view input)
{
    Fishes ret = {};
    enum class ParseState {
        AwaitNumber,
        AwaitComma,
        Fin,
    } state = ParseState::AwaitNumber;
    for (char c : input) {
        switch (state) {
        case ParseState::AwaitNumber:
        {
            assert((c >= '0') && (c <= '9'));
            state = ParseState::AwaitComma;
            int const number = static_cast<int>(c - '0');
            ++ret.a[number];
        } break;
        case ParseState::AwaitComma:
            if (c == '\n') {
                state = ParseState::Fin;
            } else {
                assert(c == ',');
                state = ParseState::AwaitNumber;
            }
            break;
        case ParseState::Fin:
            assert(false);
            break;
        }
    }
    assert(state == ParseState::Fin);
    return ret;
}

Fishes simulateDay(Fishes fishes)
{
    int64_t const new_spawn = fishes.a[0];
    std::rotate(begin(fishes.a), begin(fishes.a) + 1, begin(fishes.a) + 7);
    fishes.a[6] += fishes.a[7];
    fishes.a[7] = fishes.a[8];
    fishes.a[8] = new_spawn;
    return fishes;
}

int64_t result(Fishes fishes, int const n_days)
{
    for (int day = 0; day < n_days; ++day) {
        fishes = simulateDay(fishes);
    }
    return ranges::accumulate(fishes.a, static_cast<int64_t>(0));
}
