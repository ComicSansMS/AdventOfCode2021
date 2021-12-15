#include <chiton.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>
#include <limits>

Map parseInput(std::string_view input)
{
    Map ret{};
    for (auto const& rline : input | ranges::views::split('\n')) {
        int w = 0;
        for (char const c : rline) {
            assert((c >= '0') && (c <= '9'));
            ret.m.push_back(c - '0');
            ++w;
        }
        if (ret.width == 0) { ret.width = w; }
        assert(ret.width == w);
        ++ret.height;
    }
    return ret;
}

int getIndex(Map const& m, int x, int y)
{
    assert((x >= 0) && (x < m.width));
    assert((y >= 0) && (y < m.height));
    return y*m.width + x;
}

template<typename Func>
void visit4Neighbourhood(Map const& m, int x, int y, Func func)
{
    if (x > 0) {
        // left
        func(x - 1, y);
    }
    if (x < m.width - 1) {
        // right
        func(x + 1, y);
    }
    if (y > 0) {
        // top
        func(x, y - 1);
    }
    if (y < m.height - 1) {
        // bottom
        func(x, y + 1);
    }
}

namespace {
struct UnvisitedNodes {
    int distance;
    int index;
    friend bool operator==(UnvisitedNodes const&, UnvisitedNodes const&) = default;
    friend bool operator<(UnvisitedNodes const& lhs, UnvisitedNodes const& rhs) {
        return lhs.distance < rhs.distance;
    }
};
}

int shortestPath(Map const& m)
{
    struct Node {
        int distance = std::numeric_limits<int>::max();
        bool visited = false;
    };
    std::vector<Node> nodes(m.m.size());
    nodes[0].distance = 0;
    std::vector<UnvisitedNodes> unvisited;
    unvisited.push_back(UnvisitedNodes{ .distance = 0, .index = 0 });
    while (!unvisited.empty()) {
        int const index = unvisited.front().index;
        Node& n = nodes[index];
        int const x = index % m.width;
        int const y = index / m.width;
        n.visited = true;
        unvisited.erase(unvisited.begin());
        visit4Neighbourhood(m, x, y, [&](int nx, int ny) {
                int const neighbour_index = getIndex(m, nx, ny);
                Node& neighbour = nodes[neighbour_index];
                int const new_cost = m.m[neighbour_index] + n.distance;
                if (new_cost < neighbour.distance) {
                    assert(neighbour.distance == std::numeric_limits<int>::max());
                    neighbour.distance = new_cost;
                    UnvisitedNodes const uv{ .distance = new_cost, .index = neighbour_index };
                    unvisited.insert(std::upper_bound(begin(unvisited), end(unvisited), uv), uv);
                }
            });
        if (index == nodes.size() - 1) { break; }
    }

    return nodes.back().distance;
}

Map expandMap(Map const& m)
{
    Map ret;
    ret.width = m.width * 5;
    ret.height = m.height * 5;
    ret.m.resize(ret.width * ret.height);
    for (int iy = 0; iy < m.height; ++iy) {
        for (int ix = 0; ix < m.width; ++ix) {
            int const cost = m.m[iy*m.width + ix];
            for (int my = 0; my < 5; ++my) {
                for (int mx = 0; mx < 5; ++mx) {
                    int const cost_increase = mx + my;
                    int new_cost = cost + cost_increase;
                    if (new_cost > 9) { new_cost -= 9; }
                    int const ny = iy + (my * m.height);
                    int const nx = ix + (mx * m.width);
                    ret.m[ny*ret.width + nx] = new_cost;
                }
            }
        }
    }
    return ret;
}
