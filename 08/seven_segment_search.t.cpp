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

    SECTION("Is Valid Mapping")
    {
        CHECK(isValidMapping(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }));
        CHECK(isValidMapping(Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }));
        CHECK(isValidMapping(Mapping{ .map = { 1, 0, 3, 2, 5, 4, 6 } }));
        CHECK(isValidMapping(Mapping{ .map = { 1, 2, 3, 4, 5, 6, 0 } }));
        CHECK(!isValidMapping(Mapping{ .map = { 1, 2, 3, 4, 5, 6, 1 } }));
        CHECK(!isValidMapping(Mapping{ .map = { 1, 2, 3, 4, 5, 6, 7 } }));
    }


    SECTION("Number checking")
    {

        CHECK(!mapsTo1(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b001'0010 }));
        CHECK(mapsTo1( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b001'0010 }));

        CHECK(!mapsTo2(Mapping{ .map = { 1, 0, 2, 3, 4, 5, 6 } }, Segment{ 0b101'1101 }));
        CHECK(mapsTo2( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b101'1101 }));

        CHECK(!mapsTo3(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b101'1011 }));
        CHECK(mapsTo3( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b101'1011 }));

        CHECK(!mapsTo4(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b011'1010 }));
        CHECK(mapsTo4( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b011'1010 }));

        CHECK(!mapsTo5(Mapping{ .map = { 0, 2, 1, 3, 4, 5, 6 } }, Segment{ 0b110'1011 }));
        CHECK(mapsTo5( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b110'1011 }));

        CHECK(!mapsTo6(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b110'1111 }));
        CHECK(mapsTo6( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b110'1111 }));

        CHECK(!mapsTo7(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b101'0010 }));
        CHECK(mapsTo7( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b101'0010 }));

        CHECK(mapsTo8(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b111'1111 }));
        CHECK(mapsTo8(Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b111'1111 }));
        CHECK(!mapsTo8(Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b111'1110 }));

        CHECK(!mapsTo9(Mapping{ .map = { 0, 1, 2, 3, 4, 5, 6 } }, Segment{ 0b111'1011 }));
        CHECK(mapsTo9( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b111'1011 }));

        CHECK(!mapsTo0(Mapping{ .map = { 0, 1, 3, 2, 4, 5, 6 } }, Segment{ 0b111'0111 }));
        CHECK(mapsTo0( Mapping{ .map = { 6, 5, 4, 3, 2, 1, 0 } }, Segment{ 0b111'0111 }));
    }

    SECTION("Determine Valid Mapping")
    {
        std::vector<Display> displays = parseInput(sample_input1);
        CHECK(determineValidMapping(displays[0]).map == Mapping{ .map = { 3, 2, 6, 1, 0, 5, 4 } }.map);
    }

    SECTION("Get Code")
    {
        {
            std::vector<Display> displays = parseInput(sample_input1);
            CHECK(getCode(determineValidMapping(displays[0]), displays[0]) == 5353);
        }
        {
            std::vector<Display> displays = parseInput(sample_input2);
            CHECK(getCode(determineValidMapping(displays[0]), displays[0]) == 8394);
            CHECK(getCode(determineValidMapping(displays[1]), displays[1]) == 9781);
            CHECK(getCode(determineValidMapping(displays[2]), displays[2]) == 1197);
            CHECK(getCode(determineValidMapping(displays[3]), displays[3]) == 9361);
            CHECK(getCode(determineValidMapping(displays[4]), displays[4]) == 4873);
            CHECK(getCode(determineValidMapping(displays[5]), displays[5]) == 8418);
            CHECK(getCode(determineValidMapping(displays[6]), displays[6]) == 4548);
            CHECK(getCode(determineValidMapping(displays[7]), displays[7]) == 1625);
            CHECK(getCode(determineValidMapping(displays[8]), displays[8]) == 8717);
            CHECK(getCode(determineValidMapping(displays[9]), displays[9]) == 4315);
        }
    }

    SECTION("Result 2")
    {
        CHECK(result2(parseInput(sample_input1)) == 5353);
        CHECK(result2(parseInput(sample_input2)) == 61229);
    }
}
