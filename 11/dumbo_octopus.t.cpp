#include <dumbo_octopus.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Dumbo Octopus")
{
    char const sample_input[] = "5483143223" "\n"
                                "2745854711" "\n"
                                "5264556173" "\n"
                                "6141336146" "\n"
                                "6357385478" "\n"
                                "4167524645" "\n"
                                "2176841721" "\n"
                                "6882881134" "\n"
                                "4846848554" "\n"
                                "5283751526" "\n";

    auto const map = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(fmt::format("{}", map) == sample_input);
        CHECK(map.flashes == 0);
    }

    SECTION("Step")
    {
        // After step 1:
        Map s = step(map);
        CHECK(fmt::format("{}", s) == "6594254334" "\n"
                                      "3856965822" "\n"
                                      "6375667284" "\n"
                                      "7252447257" "\n"
                                      "7468496589" "\n"
                                      "5278635756" "\n"
                                      "3287952832" "\n"
                                      "7993992245" "\n"
                                      "5957959665" "\n"
                                      "6394862637" "\n");
        CHECK(s.flashes == 0);

        // After step 2:
        s = step(s);
        CHECK(fmt::format("{}", s) == "8807476555" "\n"
                                      "5089087054" "\n"
                                      "8597889608" "\n"
                                      "8485769600" "\n"
                                      "8700908800" "\n"
                                      "6600088989" "\n"
                                      "6800005943" "\n"
                                      "0000007456" "\n"
                                      "9000000876" "\n"
                                      "8700006848" "\n");
        CHECK(s.flashes == 35);

        // After step 3:
        s = step(s);
        CHECK(fmt::format("{}", s) == "0050900866" "\n"
                                      "8500800575" "\n"
                                      "9900000039" "\n"
                                      "9700000041" "\n"
                                      "9935080063" "\n"
                                      "7712300000" "\n"
                                      "7911250009" "\n"
                                      "2211130000" "\n"
                                      "0421125000" "\n"
                                      "0021119000" "\n");
        CHECK(s.flashes == 45);

        // After step 4:
        s = step(s);
        CHECK(fmt::format("{}", s) == "2263031977" "\n"
                                      "0923031697" "\n"
                                      "0032221150" "\n"
                                      "0041111163" "\n"
                                      "0076191174" "\n"
                                      "0053411122" "\n"
                                      "0042361120" "\n"
                                      "5532241122" "\n"
                                      "1532247211" "\n"
                                      "1132230211" "\n");
        CHECK(s.flashes == 16);

        // After step 5:
        s = step(s);
        CHECK(fmt::format("{}", s) == "4484144000" "\n"
                                      "2044144000" "\n"
                                      "2253333493" "\n"
                                      "1152333274" "\n"
                                      "1187303285" "\n"
                                      "1164633233" "\n"
                                      "1153472231" "\n"
                                      "6643352233" "\n"
                                      "2643358322" "\n"
                                      "2243341322" "\n");
        CHECK(s.flashes == 8);

        // After step 6:
        s = step(s);
        CHECK(fmt::format("{}", s) == "5595255111" "\n"
                                      "3155255222" "\n"
                                      "3364444605" "\n"
                                      "2263444496" "\n"
                                      "2298414396" "\n"
                                      "2275744344" "\n"
                                      "2264583342" "\n"
                                      "7754463344" "\n"
                                      "3754469433" "\n"
                                      "3354452433" "\n");
        CHECK(s.flashes == 1);

        // After step 7:
        s = step(s);
        CHECK(fmt::format("{}", s) == "6707366222" "\n"
                                      "4377366333" "\n"
                                      "4475555827" "\n"
                                      "3496655709" "\n"
                                      "3500625609" "\n"
                                      "3509955566" "\n"
                                      "3486694453" "\n"
                                      "8865585555" "\n"
                                      "4865580644" "\n"
                                      "4465574644" "\n");
        CHECK(s.flashes == 7);

        // After step 8:
        s = step(s);
        CHECK(fmt::format("{}", s) == "7818477333" "\n"
                                      "5488477444" "\n"
                                      "5697666949" "\n"
                                      "4608766830" "\n"
                                      "4734946730" "\n"
                                      "4740097688" "\n"
                                      "6900007564" "\n"
                                      "0000009666" "\n"
                                      "8000004755" "\n"
                                      "6800007755" "\n");
        CHECK(s.flashes == 24);

        // After step 9:
        s = step(s);
        CHECK(fmt::format("{}", s) == "9060000644" "\n"
                                      "7800000976" "\n"
                                      "6900000080" "\n"
                                      "5840000082" "\n"
                                      "5858000093" "\n"
                                      "6962400000" "\n"
                                      "8021250009" "\n"
                                      "2221130009" "\n"
                                      "9111128097" "\n"
                                      "7911119976" "\n");
        CHECK(s.flashes == 39);

        // After step 10:
        s = step(s);
        CHECK(fmt::format("{}", s) == "0481112976" "\n"
                                      "0031112009" "\n"
                                      "0041112504" "\n"
                                      "0081111406" "\n"
                                      "0099111306" "\n"
                                      "0093511233" "\n"
                                      "0442361130" "\n"
                                      "5532252350" "\n"
                                      "0532250600" "\n"
                                      "0032240000" "\n");
        CHECK(s.flashes == 29);

        // After step 20:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "3936556452" "\n"
                                      "5686556806" "\n"
                                      "4496555690" "\n"
                                      "4448655580" "\n"
                                      "4456865570" "\n"
                                      "5680086577" "\n"
                                      "7000009896" "\n"
                                      "0000000344" "\n"
                                      "6000000364" "\n"
                                      "4600009543" "\n");
        CHECK(s.flashes == 28);

        // After step 30:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "0643334118" "\n"
                                      "4253334611" "\n"
                                      "3374333458" "\n"
                                      "2225333337" "\n"
                                      "2229333338" "\n"
                                      "2276733333" "\n"
                                      "2754574565" "\n"
                                      "5544458511" "\n"
                                      "9444447111" "\n"
                                      "7944446119" "\n");
        CHECK(s.flashes == 1);

        // After step 40:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "6211111981" "\n"
                                      "0421111119" "\n"
                                      "0042111115" "\n"
                                      "0003111115" "\n"
                                      "0003111116" "\n"
                                      "0065611111" "\n"
                                      "0532351111" "\n"
                                      "3322234597" "\n"
                                      "2222222976" "\n"
                                      "2222222762" "\n");
        CHECK(s.flashes == 12);

        // After step 50:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "9655556447" "\n"
                                      "4865556805" "\n"
                                      "4486555690" "\n"
                                      "4458655580" "\n"
                                      "4574865570" "\n"
                                      "5700086566" "\n"
                                      "6000009887" "\n"
                                      "8000000533" "\n"
                                      "6800000633" "\n"
                                      "5680000538" "\n");
        CHECK(s.flashes == 27);

        // After step 60:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "2533334200" "\n"
                                      "2743334640" "\n"
                                      "2264333458" "\n"
                                      "2225333337" "\n"
                                      "2225333338" "\n"
                                      "2287833333" "\n"
                                      "3854573455" "\n"
                                      "1854458611" "\n"
                                      "1175447111" "\n"
                                      "1115446111" "\n");
        CHECK(s.flashes == 3);

        // After step 70:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "8211111164" "\n"
                                      "0421111166" "\n"
                                      "0042111114" "\n"
                                      "0004211115" "\n"
                                      "0000211116" "\n"
                                      "0065611111" "\n"
                                      "0532351111" "\n"
                                      "7322235117" "\n"
                                      "5722223475" "\n"
                                      "4572222754" "\n");
        CHECK(s.flashes == 13);

        // After step 80:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "1755555697" "\n"
                                      "5965555609" "\n"
                                      "4486555680" "\n"
                                      "4458655580" "\n"
                                      "4570865570" "\n"
                                      "5700086566" "\n"
                                      "7000008666" "\n"
                                      "0000000990" "\n"
                                      "0000000800" "\n"
                                      "0000000000" "\n");
        CHECK(s.flashes == 40);

        // After step 90:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "7433333522" "\n"
                                      "2643333522" "\n"
                                      "2264333458" "\n"
                                      "2226433337" "\n"
                                      "2222433338" "\n"
                                      "2287833333" "\n"
                                      "2854573333" "\n"
                                      "4854458333" "\n"
                                      "3387779333" "\n"
                                      "3333333333" "\n");
        CHECK(s.flashes == 0);

        // After step 100:
        for (int i = 0; i < 10; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "0397666866" "\n"
                                      "0749766918" "\n"
                                      "0053976933" "\n"
                                      "0004297822" "\n"
                                      "0004229892" "\n"
                                      "0053222877" "\n"
                                      "0532222966" "\n"
                                      "9322228966" "\n"
                                      "7922286866" "\n"
                                      "6789998766" "\n");
        CHECK(s.flashes == 13);
    }

    SECTION("Result 1")
    {
        CHECK(result1(map) == 1656);
    }

    SECTION("Simulateneous Flash")
    {
        Map s = map;
        // After step 193:
        for (int i = 0; i < 193; ++i) { s = step(s); }
        CHECK(fmt::format("{}", s) == "5877777777" "\n"
                                      "8877777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n"
                                      "7777777777" "\n");
        CHECK(s.flashes == 0);

        // After step 194:
        s = step(s);
        CHECK(fmt::format("{}", s) == "6988888888" "\n"
                                      "9988888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n"
                                      "8888888888" "\n");
        CHECK(s.flashes == 0);

        // After step 195:
        s = step(s);
        CHECK(fmt::format("{}", s) == "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n"
                                      "0000000000" "\n");
        CHECK(s.flashes == 100);
    }

    SECTION("Result 2")
    {
        CHECK(result2(map) == 195);
    }
}
