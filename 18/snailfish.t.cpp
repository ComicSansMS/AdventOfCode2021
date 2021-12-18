#include <snailfish.hpp>

#include <catch.hpp>

TEST_CASE("Snailfish")
{
    PairPool pool;

    SECTION("Parse Input")
    {
        char const sample_input[] = "[1,2]"                                                         "\n"
                                    "[[1,2],3]"                                                     "\n"
                                    "[9,[8,7]]"                                                     "\n"
                                    "[[1,9],[8,5]]"                                                 "\n"
                                    "[[[[1,2],[3,4]],[[5,6],[7,8]]],9]"                             "\n"
                                    "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]"                     "\n"
                                    "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]" "\n";
        auto const pairs = parseInput(pool, sample_input);
        REQUIRE(pairs.size() == 7);
        // [1,2]
        CHECK(getNumber(pairs[0]->first) == 1);
        CHECK(getNumber(pairs[0]->second) == 2);

        // [[1,2],3]
        REQUIRE(getPair(pairs[1]->first));
        CHECK(getNumber(getPair(pairs[1]->first)->first) == 1);
        CHECK(getNumber(getPair(pairs[1]->first)->second) == 2);
        CHECK(getNumber(pairs[1]->second) == 3);

        // "[9,[8,7]]"
        CHECK(getNumber(pairs[2]->first) == 9);
        REQUIRE(getPair(pairs[2]->second));
        CHECK(getNumber(getPair(pairs[2]->second)->first) == 8);
        CHECK(getNumber(getPair(pairs[2]->second)->second) == 7);

        // "[[1,9],[8,5]]"
        REQUIRE(getPair(pairs[3]->first));
        CHECK(getNumber(getPair(pairs[3]->first)->first) == 1);
        CHECK(getNumber(getPair(pairs[3]->first)->second) == 9);
        REQUIRE(getPair(pairs[3]->second));
        CHECK(getNumber(getPair(pairs[3]->second)->first) == 8);
        CHECK(getNumber(getPair(pairs[3]->second)->second) == 5);

        // [[[[1,2],[3,4]],[[5,6],[7,8]]],9]
        REQUIRE(getPair(pairs[4]->first));
        REQUIRE(getPair(getPair(pairs[4]->first)->first));
        REQUIRE(getPair(getPair(getPair(pairs[4]->first)->first)->first));
        CHECK(getNumber(getPair(getPair(getPair(pairs[4]->first)->first)->first)->first) == 1);
        CHECK(getNumber(getPair(getPair(getPair(pairs[4]->first)->first)->first)->second) == 2);
        REQUIRE(getPair(getPair(getPair(pairs[4]->first)->first)->second));
        CHECK(getNumber(getPair(getPair(getPair(pairs[4]->first)->first)->second)->first) == 3);
        CHECK(getNumber(getPair(getPair(getPair(pairs[4]->first)->first)->second)->second) == 4);
        REQUIRE(getPair(getPair(pairs[4]->first)->second));
        REQUIRE(getPair(getPair(getPair(pairs[4]->first)->second)->first));
        REQUIRE(getNumber(getPair(getPair(getPair(pairs[4]->first)->second)->first)->first) == 5);
        REQUIRE(getNumber(getPair(getPair(getPair(pairs[4]->first)->second)->first)->second) == 6);
        REQUIRE(getPair(getPair(getPair(pairs[4]->first)->second)->second));
        REQUIRE(getNumber(getPair(getPair(getPair(pairs[4]->first)->second)->second)->first) == 7);
        REQUIRE(getNumber(getPair(getPair(getPair(pairs[4]->first)->second)->second)->second) == 8);
        REQUIRE(getNumber(pairs[4]->second) == 9);

        CHECK(fmt::format("{}", *pairs[5]) == "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]");
        CHECK(fmt::format("{}", *pairs[6]) == "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]");
    }

    SECTION("Clone")
    {
        auto const p1 = parseLine(pool, "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]");
        PairPtr cp1 = clone(pool, p1);
        CHECK(fmt::format("{}", *cp1) == "[[[9,[3,8]],[[0,9],6]],[[[3,7],[4,9]],3]]");
        auto const p2 = parseLine(pool, "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]");
        PairPtr cp2 = clone(pool, p2);
        CHECK(fmt::format("{}", *cp2) == "[[[[1,3],[5,3]],[[1,3],[8,7]]],[[[4,9],[6,9]],[[8,2],[7,3]]]]");
    }

    SECTION("Add")
    {
        auto const sum = add(pool, parseLine(pool, "[1,2]"), parseLine(pool, "[[3,4],5]"));
        CHECK(fmt::format("{}", *sum) == "[[1,2],[[3,4],5]]");
    }

    SECTION("Explode")
    {
        {
            Pair p = parseLine(pool, "[[[[[9,8],1],2],3],4]");
            CHECK(explode(p));
            CHECK(fmt::format("{}", p) == "[[[[0,9],2],3],4]");
        }
        {
            Pair p = parseLine(pool, "[7,[6,[5,[4,[3,2]]]]]");
            CHECK(explode(p));
            CHECK(fmt::format("{}", p) == "[7,[6,[5,[7,0]]]]");
        }
        {
            Pair p = parseLine(pool, "[[6,[5,[4,[3,2]]]],1]");
            CHECK(explode(p));
            CHECK(fmt::format("{}", p) == "[[6,[5,[7,0]]],3]");
        }
        {
            Pair p = parseLine(pool, "[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]");
            CHECK(explode(p));
            CHECK(fmt::format("{}", p) == "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
        }
        {
            Pair p = parseLine(pool, "[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]");
            CHECK(explode(p));
            CHECK(fmt::format("{}", p) == "[[3,[2,[8,0]]],[9,[5,[7,0]]]]");
            CHECK(!explode(p));
        }
        {
            // [                                                                       ]
            //  [                                   ],[                               ]
            //   [           ],[                   ]   7,[                           ]
            //    [4,0],[5,0]   [           ],[9,5]       [           ],[           ]
            //                   [4,5],[2,6]               [3,7],[4,3]   [6,3],[8,8]
            // 
            Pair p = parseLine(pool, "[[[[4,0],[5,0]],[[[4,5],[2,6]],[9,5]]],[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]]");
            CHECK(explode(p));
            // [                                                                    ]
            //  [                                ],[                               ]
            //   [           ],[                ]   7,[                           ]
            //    [4,0],[5,4]   [        ],[9,5]       [           ],[           ]
            //                   0,[7,6]                [3,7],[4,3]   [6,3],[8,8]
            REQUIRE(fmt::format("{}", p) == "[[[[4,0],[5,4]],[[0,[7,6]],[9,5]]],[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]]");
        }
    }

    SECTION("Split")
    {
        {
            Pair p{ .first = Number{.n = 10}, .second = Number{.n = 11} };
            CHECK(fmt::format("{}", p) == "[10,11]");
            CHECK(split(pool, p));
            CHECK(fmt::format("{}", p) == "[[5,5],11]");
            CHECK(split(pool, p));
            CHECK(fmt::format("{}", p) == "[[5,5],[5,6]]");
            CHECK(!split(pool, p));
        }
    }

    SECTION("Reduce Step")
    {
        auto p = add(pool, parseLine(pool, "[[[[4,3],4],4],[7,[[8,4],9]]]"), parseLine(pool, "[1,1]"));
        CHECK(fmt::format("{}", *p) == "[[[[[4,3],4],4],[7,[[8,4],9]]],[1,1]]");
        CHECK(reduce_step(pool, *p));
        CHECK(fmt::format("{}", *p) == "[[[[0,7],4],[7,[[8,4],9]]],[1,1]]");
        CHECK(reduce_step(pool, *p));
        CHECK(fmt::format("{}", *p) == "[[[[0,7],4],[15,[0,13]]],[1,1]]");
        CHECK(reduce_step(pool, *p));
        CHECK(fmt::format("{}", *p) == "[[[[0,7],4],[[7,8],[0,13]]],[1,1]]");
        CHECK(reduce_step(pool, *p));
        CHECK(fmt::format("{}", *p) == "[[[[0,7],4],[[7,8],[0,[6,7]]]],[1,1]]");
        CHECK(reduce_step(pool, *p));
        CHECK(fmt::format("{}", *p) == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
        CHECK(!reduce_step(pool, *p));
    }

    SECTION("Reduce")
    {
        {
            auto p = add(pool, parseLine(pool, "[[[[4,3],4],4],[7,[[8,4],9]]]"), parseLine(pool, "[1,1]"));
            reduce(pool, *p);
            CHECK(fmt::format("{}", *p) == "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]");
        }
        {
            auto p = add(pool, parseLine(pool, "[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]"), parseLine(pool, "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]"));
            CHECK(fmt::format("{}", *p) == "[[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]],[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]]");
            reduce(pool, *p);
            CHECK(fmt::format("{}", *p) == "[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]");
        }
    }

    SECTION("Add/Reduce")
    {
        {
            char const sample[] = "[1,1]" "\n"
                                  "[2,2]" "\n"
                                  "[3,3]" "\n"
                                  "[4,4]" "\n";
            auto const ps = parseInput(pool, sample);
            auto const r = addReduce(pool, ps);
            CHECK(fmt::format("{}", *r) == "[[[[1,1],[2,2]],[3,3]],[4,4]]");
        }
        {
            char const sample[] = "[1,1]" "\n"
                                  "[2,2]" "\n"
                                  "[3,3]" "\n"
                                  "[4,4]" "\n"
                                  "[5,5]" "\n";
            auto const ps = parseInput(pool, sample);
            auto const r = addReduce(pool, ps);
            CHECK(fmt::format("{}", *r) == "[[[[3,0],[5,3]],[4,4]],[5,5]]");
        }
        {
            char const sample[] = "[1,1]" "\n"
                                  "[2,2]" "\n"
                                  "[3,3]" "\n"
                                  "[4,4]" "\n"
                                  "[5,5]" "\n"
                                  "[6,6]" "\n";
            auto const ps = parseInput(pool, sample);
            auto const r = addReduce(pool, ps);
            CHECK(fmt::format("{}", *r) == "[[[[5,0],[7,4]],[5,5]],[6,6]]");
        }
        {
            char const sample[] = "[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]"              "\n"
                                  "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]"                      "\n"
                                  "[[2,[[0,8],[3,4]]],[[[6,7],1],[7,[1,6]]]]"              "\n"
                                  "[[[[2,4],7],[6,[0,5]]],[[[6,8],[2,8]],[[2,1],[4,5]]]]"  "\n"
                                  "[7,[5,[[3,8],[1,4]]]]"                                  "\n"
                                  "[[2,[2,2]],[8,[8,1]]]"                                  "\n"
                                  "[2,9]"                                                  "\n"
                                  "[1,[[[9,3],9],[[9,0],[0,7]]]]"                          "\n"
                                  "[[[5,[7,4]],7],1]"                                      "\n"
                                  "[[[[4,2],2],6],[8,7]]"                                  "\n";
            auto const ps = parseInput(pool, sample);
            auto const r = addReduce(pool, ps);
            CHECK(fmt::format("{}", *r) == "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]");
        }
        {
            char const sample[] = "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]" "\n"
                                  "[[[5,[2,8]],4],[5,[[9,9],0]]]"                     "\n"
                                  "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]"                 "\n"
                                  "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]"                 "\n"
                                  "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]"             "\n"
                                  "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]"             "\n"
                                  "[[[[5,4],[7,7]],8],[[8,3],8]]"                     "\n"
                                  "[[9,3],[[9,9],[6,[4,9]]]]"                         "\n"
                                  "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]"             "\n"
                                  "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]"         "\n";
            auto const ps = parseInput(pool, sample);
            auto const r = addReduce(pool, ps);
            CHECK(fmt::format("{}", *r) == "[[[[6,6],[7,6]],[[7,7],[7,0]]],[[[7,7],[7,7]],[[7,8],[9,9]]]]");
        }
    }

    SECTION("Magnitude")
    {
        CHECK(magnitude(parseLine(pool, "[9,1]")) == 29);
        CHECK(magnitude(parseLine(pool, "[1,9]")) == 21);
        CHECK(magnitude(parseLine(pool, "[[9,1],[1,9]]")) == 129);
        CHECK(magnitude(parseLine(pool, "[[1,2],[[3,4],5]]")) == 143);
        CHECK(magnitude(parseLine(pool, "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")) == 1384);
        CHECK(magnitude(parseLine(pool, "[[[[1,1],[2,2]],[3,3]],[4,4]]")) == 445);
        CHECK(magnitude(parseLine(pool, "[[[[3,0],[5,3]],[4,4]],[5,5]]")) == 791);
        CHECK(magnitude(parseLine(pool, "[[[[5,0],[7,4]],[5,5]],[6,6]]")) == 1137);
        CHECK(magnitude(parseLine(pool, "[[[[8,7],[7,7]],[[8,6],[7,7]]],[[[0,7],[6,6]],[8,7]]]")) == 3488);
    }

    SECTION("Result 1")
    {
        char const sample[] = "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]" "\n"
                              "[[[5,[2,8]],4],[5,[[9,9],0]]]"                     "\n"
                              "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]"                 "\n"
                              "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]"                 "\n"
                              "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]"             "\n"
                              "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]"             "\n"
                              "[[[[5,4],[7,7]],8],[[8,3],8]]"                     "\n"
                              "[[9,3],[[9,9],[6,[4,9]]]]"                         "\n"
                              "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]"             "\n"
                              "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]"         "\n";
        CHECK(result1(pool, parseInput(pool, sample)) == 4140);
    }

    SECTION("Result 2")
    {
        char const sample[] = "[[[0,[5,8]],[[1,7],[9,6]]],[[4,[1,2]],[[1,4],2]]]" "\n"
                              "[[[5,[2,8]],4],[5,[[9,9],0]]]"                     "\n"
                              "[6,[[[6,2],[5,6]],[[7,6],[4,7]]]]"                 "\n"
                              "[[[6,[0,7]],[0,9]],[4,[9,[9,0]]]]"                 "\n"
                              "[[[7,[6,4]],[3,[1,3]]],[[[5,5],1],9]]"             "\n"
                              "[[6,[[7,3],[3,2]]],[[[3,8],[5,7]],4]]"             "\n"
                              "[[[[5,4],[7,7]],8],[[8,3],8]]"                     "\n"
                              "[[9,3],[[9,9],[6,[4,9]]]]"                         "\n"
                              "[[2,[[7,7],7]],[[5,8],[[9,3],[0,2]]]]"             "\n"
                              "[[[[5,2],5],[8,[3,7]]],[[5,[7,5]],[4,4]]]"         "\n";
        CHECK(result2(pool, parseInput(pool, sample)) == 3993);
    }
}
