#include <passage_pathing.hpp>

#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <unordered_set>

Graph parseInput(std::string_view input)
{
    Graph ret{};
    auto const add_cave = [&ret](std::string const& cave_name) -> std::size_t {
        if (auto it = ranges::find_if(ret.caves,
            [&cave_name](Cave const& cave) { return cave.name == cave_name; });
            it != ranges::end(ret.caves))
        {
            return (it - begin(ret.caves));
        } else
        {
            ret.caves.emplace_back();
            ret.caves.back().name = cave_name;
            ret.caves.back().isBig = !std::islower(cave_name[0]);

            std::size_t const cave_index = ret.caves.size() - 1;
            if (cave_name == "start") {
                ret.startCaveIndex = cave_index;
            } else if (cave_name == "end") {
                ret.endCaveIndex = cave_index;
            }
            return cave_index;
        }
    };
    auto const add_edge = [&ret](std::size_t start_index, std::size_t dest_index) {
        ret.edges.emplace_back(Edge{ .start_index = start_index, .destination_index = dest_index });
        ret.caves[start_index].neighbours.push_back(dest_index);
        ret.caves[dest_index].neighbours.push_back(start_index);
    };
    for (auto const rline : input | ranges::views::split('\n')) {
        auto const rpath = rline | ranges::views::split('-');
        auto it_rpath = ranges::begin(rpath);
        std::string const start_node = ranges::to<std::string>(*it_rpath);
        std::size_t const start_index = add_cave(start_node);
        ++it_rpath;
        std::string const dest_node = ranges::to<std::string>(*it_rpath);
        std::size_t const dest_index = add_cave(dest_node);
        ++it_rpath;
        assert(it_rpath == ranges::end(rpath));
        add_edge(start_index, dest_index);
    }
    return ret;
}

void visitNode_rec(Graph const& g, std::size_t node_index, std::vector<Path>& paths, Path& current_path, bool did_visit_small_cave)
{
    current_path.nodes.push_back(node_index);
    if (node_index == g.endCaveIndex) {
        paths.push_back(current_path);
    } else {
        Cave const& node = g.caves[node_index];
        for (auto const& neighbour_index : node.neighbours) {
            Cave const& neighbour = g.caves[neighbour_index];
            if ((neighbour.isBig) ||
                (ranges::find(current_path.nodes, neighbour_index) == ranges::end(current_path.nodes)))
            {
                visitNode_rec(g, neighbour_index, paths, current_path, did_visit_small_cave);
            } else if ((!did_visit_small_cave) && (neighbour_index != g.startCaveIndex)) {
                bool const did_visit_rec = true;
                visitNode_rec(g, neighbour_index, paths, current_path, did_visit_rec);
            }
        }
    }
    current_path.nodes.pop_back();
}

std::vector<Path> allPaths(Graph const& g)
{
    std::vector<Path> ret;
    Path current_path;
    visitNode_rec(g, g.startCaveIndex, ret, current_path, true);
    return ret;
}

std::vector<Path> allPaths2(Graph const& g)
{
    std::vector<Path> ret;
    Path current_path;
    visitNode_rec(g, g.startCaveIndex, ret, current_path, false);
    return ret;
}
