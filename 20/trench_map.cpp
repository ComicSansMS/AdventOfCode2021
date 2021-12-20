#include <trench_map.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <optional>
#include <regex>
#include <unordered_map>
#include <unordered_set>

Image parseInput(std::string_view input)
{
    Image ret;
    
    std::string_view::iterator it = begin(input);
    std::string_view::iterator const it_end = end(input);
    int li = 0;
    while (*it != '\n') {
        char const c = *it;
        assert((c == '#') || (c == '.'));
        ret.lookup.set(li, (c == '#'));
        ++li;
        ++it;
        assert(it != end(input));
    }
    assert(li == 512);
    ++it;
    assert(*it == '\n');
    ++it;
    int ix = 0;
    int iy = 0;
    int width = -1;
    for (; it != it_end; ++it) {
        char const c = *it;
        if (c == '\n') {
            if (width == -1) { width = ix; }
            assert(width == ix);
            ++iy;
            ix = 0;
        } else {
            assert((c == '#') || (c == '.'));
            if (c == '#') {
                ret.points.insert(Point{ .x = ix, .y = iy });
            }
            ++ix;
        }
    }
    ret.min = Point{ .x = 0, .y = 0 };
    ret.max = Point{ .x = width - 1, .y = iy - 1 };
    return ret;
}

int16_t neigbourhood2Number(Image const& input, Point const& p, bool out_of_bounds_are_0)
{
    int16_t ret = 0;
    Point const neighbours[9] = {
        Point{.x = (p.x - 1), .y = (p.y - 1) },
        Point{.x = (p.x),     .y = (p.y - 1) },
        Point{.x = (p.x + 1), .y = (p.y - 1) },
        Point{.x = (p.x - 1), .y = (p.y) },
        Point{.x = (p.x),     .y = (p.y) },
        Point{.x = (p.x + 1), .y = (p.y) },
        Point{.x = (p.x - 1), .y = (p.y + 1) },
        Point{.x = (p.x),     .y = (p.y + 1) },
        Point{.x = (p.x + 1), .y = (p.y + 1) }
    };
    auto const& points = input.points;
    auto const min = input.min;
    auto const max = input.max;
    for (auto const n : neighbours) {
        ret <<= 1;
        if ((n.x >= min.x) && (n.x <= max.x) && (n.y >= min.y) && (n.y <= max.y)) {
            if (points.contains(n)) {
                ret |= 0x1;
            }
        } else {
            ret |= (out_of_bounds_are_0) ? 0 : 1;
        }
        
    }
    assert((ret >= 0) && (ret < 512));
    return ret;
}

Image step(Image const& input_img, bool out_of_bounds_are_0)
{
    Image output;
    output.lookup = input_img.lookup;
    output.min.x = input_img.min.x - 1;
    output.min.y = input_img.min.y - 1;
    output.max.x = input_img.max.x + 1;
    output.max.y = input_img.max.y + 1;
    for (int iy = output.min.y, iy_end = output.max.y; iy <= iy_end; ++iy) {
        for (int ix = output.min.x, ix_end = output.max.x; ix <= ix_end; ++ix) {
            Point const p{ .x = ix, .y = iy };
            int16_t lookup_index = neigbourhood2Number(input_img, p, out_of_bounds_are_0);
            if (input_img.lookup[lookup_index]) {
                output.points.insert(p);
                output.min.x = std::min(output.min.x, p.x);
                output.min.y = std::min(output.min.y, p.y);
                output.max.x = std::max(output.max.x, p.x);
                output.max.y = std::max(output.max.y, p.y);
            }
        }
    }
    return output;
}

int64_t enhance(Image const& input_img, int steps)
{
    Image s = input_img;
    for (int i = 0; i < steps; ++i) {
        s = step(s, (i%2 == 0));
    }
    return s.points.size();
}
