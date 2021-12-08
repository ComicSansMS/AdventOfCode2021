#include <seven_segment_search.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Seven Segment Search")
{
    char const sample_input1[] = "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | "
                                 "cdfeb fcadb cdfeb cdbaf\n";
    char const sample_input2[] = "be cfbegad cbdgef fgaecd cgeb fdcge agebfd fecdb fabcd edb | "
                                 "fdgacbe cefdb cefbgd gcbe" "\n"
                                 "edbfga begcd cbg gc gcadebf fbgde acbgfd abcde gfcbed gfec | "
                                 "fcgedb cgb dgebacf gc" "\n"
                                 "fgaebd cg bdaec gdafb agbcfd gdcbef bgcad gfac gcb cdgabef | "
                                 "cg cg fdcagb cbg" "\n"
                                 "fbegcd cbd adcefb dageb afcb bc aefdc ecdab fgdeca fcdbega | "
                                 "efabcd cedba gadfec cb" "\n"
                                 "aecbfdg fbg gf bafeg dbefa fcge gcbea fcaegb dgceab fcbdga | "
                                 "gecf egdcabf bgf bfgea" "\n"
                                 "fgeab ca afcebg bdacfeg cfaedg gcfdb baec bfadeg bafgc acf | "
                                 "gebdcfa ecba ca fadegcb" "\n"
                                 "dbcfg fgd bdegcaf fgec aegbdf ecdfab fbedc dacgb gdcebf gf | "
                                 "cefg dcbef fcge gbcadfe" "\n"
                                 "bdfegc cbegaf gecbf dfcage bdacg ed bedf ced adcbefg gebcd | "
                                 "ed bcgafe cdgba cbgef" "\n"
                                 "egadfb cdbfeg cegd fecab cgb gbdefca cg fgcdab egfdb bfceg | "
                                 "gbdfcae bgc cg cgb" "\n"
                                 "gcafb gcf dcaebfg ecagb gf abcdeg gaef cafbge fdbac fegbdc | "
                                 "fgae cfgab fg bagce" "\n";

    SECTION("Parse Input")
    {
        std::vector<Display> displays = parseInput(sample_input1);
        REQUIRE(displays.size() == 1);
        //                                          abc defg
        CHECK(displays[0].patterns[0] == Segment{ 0b111'1111 });
        CHECK(displays[0].patterns[1] == Segment{ 0b011'1110 });
        CHECK(displays[0].patterns[2] == Segment{ 0b101'1011 });
        CHECK(displays[0].patterns[3] == Segment{ 0b111'1010 });
        CHECK(displays[0].patterns[4] == Segment{ 0b110'1000 });
        CHECK(displays[0].patterns[5] == Segment{ 0b111'1110 });
        CHECK(displays[0].patterns[6] == Segment{ 0b011'1111 });
        CHECK(displays[0].patterns[7] == Segment{ 0b110'0110 });
        CHECK(displays[0].patterns[8] == Segment{ 0b111'1101 });
        CHECK(displays[0].patterns[9] == Segment{ 0b110'0000 });
        //                                      abc defg
        CHECK(displays[0].code[0] == Segment{ 0b011'1110 });
        CHECK(displays[0].code[1] == Segment{ 0b111'1010 });
        CHECK(displays[0].code[2] == Segment{ 0b011'1110 });
        CHECK(displays[0].code[3] == Segment{ 0b111'1010 });

        std::vector<Display> displays2 = parseInput(sample_input2);
        REQUIRE(displays2.size() == 10);
        //                                           abc defg
        CHECK(displays2[0].patterns[0] == Segment{ 0b010'0100 });
        CHECK(displays2[1].patterns[0] == Segment{ 0b110'1111 });
        CHECK(displays2[9].code[2]     == Segment{ 0b000'0011 });
        CHECK(displays2[8].code[3]     == Segment{ 0b011'0001 });
    }

    SECTION("Result 1")
    {
        CHECK(result1(parseInput(sample_input1)) == 0);
        CHECK(result1(parseInput(sample_input2)) == 26);
    }
}
