#include <dumbo_octopus.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <unordered_set>

Map parseInput(std::string_view input)
{
    Map ret{};
    int ix = 0;
    int iy = 0;
    for (auto const rline : input | ranges::views::split('\n')) {
        for (char const c : rline) {
            assert((c >= '0') && (c <= '9'));
            ret.a[iy*10 + ix] = (c - '0');
            ++ix;
        }
        assert(ix == 10);
        ix = 0;
        ++iy;
    }
    assert(iy == 10);
    return ret;
}

void increaseEnergy(Map& m)
{
    for (int& cell : m.a) {
        ++cell;
    }
}

void visit8Neighbourhood(int ix, int iy, auto func)
{
    if (iy > 0) {
        // upper left
        if (ix > 0) { func(ix - 1, iy - 1); }
        // up
        func(ix, iy - 1);
        // upper right
        if (ix < 9) { func(ix + 1, iy - 1); }
    }
    // left
    if (ix > 0) { func(ix - 1, iy); }
    // right
    if (ix < 9) { func(ix + 1, iy); }
    if (iy < 9) {
        // lower left
        if (ix > 0) { func(ix - 1, iy + 1); }
        // low
        func(ix, iy + 1);
        // lower right
        if (ix < 9) { func(ix + 1, iy + 1); }
    }
}

int64_t applyFlashes(Map& m)
{
    std::unordered_set<int> did_flash;
    struct XY {
        int x;
        int y;
    };
    std::vector<XY> to_flash;
    for (auto const [iy, ix] : ranges::views::cartesian_product(ranges::views::iota(0, 10), ranges::views::iota(0, 10))) {
        int const index = iy*10 + ix;
        int& cell = m.a[index];
        if (cell > 9) { to_flash.push_back(XY{ ix, iy }); }
    }
    while (!to_flash.empty()) {
        auto const [ix, iy] = to_flash.back();
        to_flash.pop_back();
        int const index = iy*10 + ix;
        if (!did_flash.contains(index)) {
            did_flash.insert(index);
            visit8Neighbourhood(ix, iy,
                [&m, &did_flash, &to_flash](int nx, int ny) {
                    int const nindex = ny*10 + nx;
                    int& ncell = m.a[nindex];
                    ++ncell;
                    if ((ncell > 9) && (!did_flash.contains(nindex))) {
                        to_flash.push_back(XY{ .x = nx, .y = ny });
                    }
                });
        }
    }
    return did_flash.size();
}

void resetEnergy(Map& m)
{
    for (int& cell : m.a) {
        if (cell > 9) {
            cell = 0;
        }
    }
}

Map step(Map const& m)
{
    Map ret = m;
    increaseEnergy(ret);
    ret.flashes = applyFlashes(ret);
    resetEnergy(ret);
    return ret;
}

int64_t result1(Map const& m)
{
    Map s = m;
    int64_t acc = 0;
    for (int i = 0; i < 100; ++i) {
        s = step(s);
        acc += s.flashes;
    }
    return acc;
}

int64_t result2(Map const& m)
{
    Map s = m;
    s.flashes = 0;
    int count = 0;
    while (s.flashes != 100) {
        s = step(s);
        ++count;
    }
    return count;
}
