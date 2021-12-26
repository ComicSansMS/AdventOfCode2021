#include <sea_cucumber.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>

Map parseInput(std::string_view input)
{
    Map ret{};
    int w = 0;
    int h = 0;
    for (auto const& c : input) {
        if (c == '\n') {
            if (ret.width == 0) {
                ret.width = w;
            }
            assert(ret.width == w);
            w = 0;
            ++h;
            continue;
        } else if (c == '>') {
            ret.v.push_back(Field::East);
        } else if (c == 'v') {
            ret.v.push_back(Field::South);
        } else {
            assert(c == '.');
            ret.v.push_back(Field::Empty);
        }
        ++w;
    }
    assert(w == 0);
    ret.height = h;
    assert(static_cast<int>(ret.v.size()) == ret.width * ret.height);
    return ret;
}

Map moveEast(Map const& m, std::size_t& cucumbers_moved)
{
    Map ret = m;
    for (int iy = 0; iy < ret.height; ++iy) {
        for (int ix = 0; ix < ret.width; ++ix) {
            Field const f = m.v[iy * ret.width + ix];
            if (f == Field::East) {
                int target_x = (ix + 1);
                if (target_x == ret.width) { target_x = 0; }
                Field const target = m.v[iy * ret.width + target_x];
                if (target == Field::Empty) {
                    ret.v[iy * ret.width + ix] = Field::Empty;
                    ret.v[iy * ret.width + target_x] = Field::East;
                    ++cucumbers_moved;
                }
            }
        }
    }
    return ret;
}

Map moveSouth(Map const& m, std::size_t& cucumbers_moved)
{
    Map ret = m;
    for (int iy = 0; iy < ret.height; ++iy) {
        for (int ix = 0; ix < ret.width; ++ix) {
            Field const f = m.v[iy * ret.width + ix];
            if (f == Field::South) {
                int target_y = (iy + 1);
                if (target_y == ret.height) { target_y = 0; }
                Field const target = m.v[target_y * ret.width + ix];
                if (target == Field::Empty) {
                    ret.v[iy * ret.width + ix] = Field::Empty;
                    ret.v[target_y * ret.width + ix] = Field::South;
                    ++cucumbers_moved;
                }
            }
        }
    }
    return ret;
}

StepReturn step(Map const& m)
{
    std::size_t cucumbers_moved = 0;
    Map const e = moveEast(m, cucumbers_moved);
    Map const s = moveSouth(e, cucumbers_moved);
    return StepReturn{ .m = s, .cucumbers_moved = cucumbers_moved };
}

int play(Map const& m)
{
    StepReturn s;
    s.cucumbers_moved = 1;
    s.m = m;
    int ret = 0;
    while (s.cucumbers_moved != 0) {
        s = step(s.m);
        ++ret;
    }
    return ret;
}
