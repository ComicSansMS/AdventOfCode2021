#include <passage_pathing.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Passage Pathing")
{
    char const sample_input1[] = "start-A" "\n"
                                 "start-b" "\n"
                                 "A-c"     "\n"
                                 "A-b"     "\n"
                                 "b-d"     "\n"
                                 "A-end"   "\n"
                                 "b-end"   "\n";

    char const sample_input2[] = "dc-end"   "\n"
                                 "HN-start" "\n"
                                 "start-kj" "\n"
                                 "dc-start" "\n"
                                 "dc-HN"    "\n"
                                 "LN-dc"    "\n"
                                 "HN-end"   "\n"
                                 "kj-sa"    "\n"
                                 "kj-HN"    "\n"
                                 "kj-dc"    "\n";

    char const sample_input3[] = "fs-end"   "\n"
                                 "he-DX"    "\n"
                                 "fs-he"    "\n"
                                 "start-DX" "\n"
                                 "pj-DX"    "\n"
                                 "end-zg"   "\n"
                                 "zg-sl"    "\n"
                                 "zg-pj"    "\n"
                                 "pj-he"    "\n"
                                 "RW-he"    "\n"
                                 "fs-DX"    "\n"
                                 "pj-RW"    "\n"
                                 "zg-RW"    "\n"
                                 "start-pj" "\n"
                                 "he-WI"    "\n"
                                 "zg-he"    "\n"
                                 "pj-fs"    "\n"
                                 "start-RW" "\n";

    SECTION("Parse Input")
    {
        auto const graph = parseInput(sample_input1);
        REQUIRE(graph.caves.size() == 6);
        CHECK(graph.caves[0] == Cave{ .name = "start", .isBig = false, .neighbours = { 1, 2 } });
        CHECK(graph.caves[1] == Cave{ .name = "A", .isBig = true, .neighbours = { 0, 3, 2, 5 } });
        CHECK(graph.caves[2] == Cave{ .name = "b", .isBig = false, .neighbours = { 0, 1, 4, 5 } });
        CHECK(graph.caves[3] == Cave{ .name = "c", .isBig = false, .neighbours = { 1 } });
        CHECK(graph.caves[4] == Cave{ .name = "d", .isBig = false, .neighbours = { 2 } });
        CHECK(graph.caves[5] == Cave{ .name = "end", .isBig = false, .neighbours = { 1, 2 } });
        REQUIRE(graph.edges.size() == 7);
        CHECK(graph.edges[0] == Edge{ .start_index = 0, .destination_index = 1 });
        CHECK(graph.edges[1] == Edge{ .start_index = 0, .destination_index = 2 });
        CHECK(graph.edges[2] == Edge{ .start_index = 1, .destination_index = 3 });
        CHECK(graph.edges[3] == Edge{ .start_index = 1, .destination_index = 2 });
        CHECK(graph.edges[4] == Edge{ .start_index = 2, .destination_index = 4 });
        CHECK(graph.edges[5] == Edge{ .start_index = 1, .destination_index = 5 });
        CHECK(graph.edges[6] == Edge{ .start_index = 2, .destination_index = 5 });
        CHECK(graph.startCaveIndex == 0);
        CHECK(graph.endCaveIndex == 5);
    }

    SECTION("All Paths")
    {
        auto const graph = parseInput(sample_input1);
        auto const ap = allPaths(graph);
        REQUIRE(ap.size() == 10);
        // start,A,b,A,c,A,end
        CHECK(ap[3].nodes == std::vector<std::size_t>{ 0, 1, 2, 1, 3, 1, 5 });
        // start,A,b,A,end
        CHECK(ap[4].nodes == std::vector<std::size_t>{ 0, 1, 2, 1, 5 });
        // start,A,b,end
        CHECK(ap[5].nodes == std::vector<std::size_t>{ 0, 1, 2, 5 });
        // start,A,c,A,b,A,end
        CHECK(ap[0].nodes == std::vector<std::size_t>{ 0, 1, 3, 1, 2, 1, 5 });
        // start,A,c,A,b,end
        CHECK(ap[1].nodes == std::vector<std::size_t>{ 0, 1, 3, 1, 2, 5 });
        // start,A,c,A,end
        CHECK(ap[2].nodes == std::vector<std::size_t>{ 0, 1, 3, 1, 5 });
        // start,A,end
        CHECK(ap[6].nodes == std::vector<std::size_t>{ 0, 1, 5 });
        // start,b,A,c,A,end
        CHECK(ap[7].nodes == std::vector<std::size_t>{ 0, 2, 1, 3, 1, 5 });
        // start,b,A,end
        CHECK(ap[8].nodes == std::vector<std::size_t>{ 0, 2, 1, 5 });
        // start,b,end
        CHECK(ap[9].nodes == std::vector<std::size_t>{ 0, 2, 5 });

        auto const graph2 = parseInput(sample_input2);
        CHECK(allPaths(graph2).size() == 19);
        auto const graph3 = parseInput(sample_input3);
        CHECK(allPaths(graph3).size() == 226);
    }

    SECTION("All Paths 2")
    {
        auto const graph = parseInput(sample_input1);
        auto const ap = allPaths2(graph);
        REQUIRE(ap.size() == 36);

        auto const graph2 = parseInput(sample_input2);
        CHECK(allPaths2(graph2).size() == 103);
        auto const graph3 = parseInput(sample_input3);
        CHECK(allPaths2(graph3).size() == 3509);
    }
}
