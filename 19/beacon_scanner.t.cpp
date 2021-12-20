#include <beacon_scanner.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Beacon Scanner")
{
    char const sample_input[] =
        "--- scanner 0 ---" "\n"
        "404,-588,-901"     "\n"
        "528,-643,409"      "\n"
        "-838,591,734"      "\n"
        "390,-675,-793"     "\n"
        "-537,-823,-458"    "\n"
        "-485,-357,347"     "\n"
        "-345,-311,381"     "\n"
        "-661,-816,-575"    "\n"
        "-876,649,763"      "\n"
        "-618,-824,-621"    "\n"
        "553,345,-567"      "\n"
        "474,580,667"       "\n"
        "-447,-329,318"     "\n"
        "-584,868,-557"     "\n"
        "544,-627,-890"     "\n"
        "564,392,-477"      "\n"
        "455,729,728"       "\n"
        "-892,524,684"      "\n"
        "-689,845,-530"     "\n"
        "423,-701,434"      "\n"
        "7,-33,-71"         "\n"
        "630,319,-379"      "\n"
        "443,580,662"       "\n"
        "-789,900,-551"     "\n"
        "459,-707,401"      "\n"
        ""                  "\n"
        "--- scanner 1 ---" "\n"
        "686,422,578"       "\n"
        "605,423,415"       "\n"
        "515,917,-361"      "\n"
        "-336,658,858"      "\n"
        "95,138,22"         "\n"
        "-476,619,847"      "\n"
        "-340,-569,-846"    "\n"
        "567,-361,727"      "\n"
        "-460,603,-452"     "\n"
        "669,-402,600"      "\n"
        "729,430,532"       "\n"
        "-500,-761,534"     "\n"
        "-322,571,750"      "\n"
        "-466,-666,-811"    "\n"
        "-429,-592,574"     "\n"
        "-355,545,-477"     "\n"
        "703,-491,-529"     "\n"
        "-328,-685,520"     "\n"
        "413,935,-424"      "\n"
        "-391,539,-444"     "\n"
        "586,-435,557"      "\n"
        "-364,-763,-893"    "\n"
        "807,-499,-711"     "\n"
        "755,-354,-619"     "\n"
        "553,889,-390"      "\n"
        ""                  "\n"
        "--- scanner 2 ---" "\n"
        "649,640,665"       "\n"
        "682,-795,504"      "\n"
        "-784,533,-524"     "\n"
        "-644,584,-595"     "\n"
        "-588,-843,648"     "\n"
        "-30,6,44"          "\n"
        "-674,560,763"      "\n"
        "500,723,-460"      "\n"
        "609,671,-379"      "\n"
        "-555,-800,653"     "\n"
        "-675,-892,-343"    "\n"
        "697,-426,-610"     "\n"
        "578,704,681"       "\n"
        "493,664,-388"      "\n"
        "-671,-858,530"     "\n"
        "-667,343,800"      "\n"
        "571,-461,-707"     "\n"
        "-138,-166,112"     "\n"
        "-889,563,-600"     "\n"
        "646,-828,498"      "\n"
        "640,759,510"       "\n"
        "-630,509,768"      "\n"
        "-681,-892,-333"    "\n"
        "673,-379,-804"     "\n"
        "-742,-814,-386"    "\n"
        "577,-820,562"      "\n"
        ""                  "\n"
        "--- scanner 3 ---" "\n"
        "-589,542,597"      "\n"
        "605,-692,669"      "\n"
        "-500,565,-823"     "\n"
        "-660,373,557"      "\n"
        "-458,-679,-417"    "\n"
        "-488,449,543"      "\n"
        "-626,468,-788"     "\n"
        "338,-750,-386"     "\n"
        "528,-832,-391"     "\n"
        "562,-778,733"      "\n"
        "-938,-730,414"     "\n"
        "543,643,-506"      "\n"
        "-524,371,-870"     "\n"
        "407,773,750"       "\n"
        "-104,29,83"        "\n"
        "378,-903,-323"     "\n"
        "-778,-728,485"     "\n"
        "426,699,580"       "\n"
        "-438,-605,-362"    "\n"
        "-469,-447,-387"    "\n"
        "509,732,623"       "\n"
        "647,635,-688"      "\n"
        "-868,-804,481"     "\n"
        "614,-800,639"      "\n"
        "595,780,-596"      "\n"
        ""                  "\n"
        "--- scanner 4 ---" "\n"
        "727,592,562"       "\n"
        "-293,-554,779"     "\n"
        "441,611,-461"      "\n"
        "-714,465,-776"     "\n"
        "-743,427,-804"     "\n"
        "-660,-479,-426"    "\n"
        "832,-632,460"      "\n"
        "927,-485,-438"     "\n"
        "408,393,-506"      "\n"
        "466,436,-512"      "\n"
        "110,16,151"        "\n"
        "-258,-428,682"     "\n"
        "-393,719,612"      "\n"
        "-211,-452,876"     "\n"
        "808,-476,-593"     "\n"
        "-575,615,604"      "\n"
        "-485,667,467"      "\n"
        "-680,325,-822"     "\n"
        "-627,-443,-432"    "\n"
        "872,-547,-609"     "\n"
        "833,512,582"       "\n"
        "807,604,487"       "\n"
        "839,-516,451"      "\n"
        "891,-625,532"      "\n"
        "-652,-548,-490"    "\n"
        "30,-46,-14"        "\n";

    auto const scans = parseInput(sample_input);
    SECTION("Parse Input")
    {
        REQUIRE(scans.size() == 5);
        REQUIRE(scans[0].beacons.size() == 25);
        CHECK(scans[0].scanOffset == Vector3{ .x = 0, .y = 0, .z = 0 });
        CHECK(scans[0].transformationIndex == -1);
        CHECK(scans[0].beacons[0] == Vector3{ .x = 404, .y = -588, .z = -901 });
        CHECK(scans[0].beacons[1] == Vector3{ .x = 528, .y = -643, .z = 409 });
        CHECK(scans[0].beacons[2] == Vector3{ .x = -838, .y = 591, .z = 734 });
        CHECK(scans[0].beacons[24] == Vector3{ .x = 459, .y = -707, .z = 401 });
        REQUIRE(scans[1].beacons.size() == 25);
        CHECK(scans[1].scanOffset == Vector3{ .x = 0, .y = 0, .z = 0 });
        CHECK(scans[1].transformationIndex == -1);
        REQUIRE(scans[2].beacons.size() == 26);
        CHECK(scans[2].scanOffset == Vector3{ .x = 0, .y = 0, .z = 0 });
        CHECK(scans[2].transformationIndex == -1);
        REQUIRE(scans[3].beacons.size() == 25);
        CHECK(scans[3].scanOffset == Vector3{ .x = 0, .y = 0, .z = 0 });
        CHECK(scans[3].transformationIndex == -1);
        REQUIRE(scans[4].beacons.size() == 26);
        CHECK(scans[4].scanOffset == Vector3{ .x = 0, .y = 0, .z = 0 });
        CHECK(scans[4].transformationIndex == -1);
        CHECK(scans[4].beacons[24] == Vector3{ .x = -652, .y = -548, .z = -490 });
        CHECK(scans[4].beacons[25] == Vector3{ .x = 30, .y = -46, .z = -14 });
    }

    SECTION("Vector3")
    {
        std::stringstream sstr;
        sstr << Vector3{ .x = 1, .y = -2, .z = 33 };
        CHECK(sstr.str() == "{ 1, -2, 33 }");
    }

    SECTION("Is in Range")
    {
        CHECK(isInRange(Vector3{ .x = 500, .y = 0, .z = -500 }, Vector3{ .x = -500, .y = 1000, .z = -1500 }));
        CHECK(!isInRange(Vector3{ .x = 500, .y = 0, .z = -500 }, Vector3{ .x = 1501, .y = 0, .z = -500 }));
        CHECK(!isInRange(Vector3{ .x = 500, .y = 0, .z = -500 }, Vector3{ .x = 500, .y = 1001, .z = -500 }));
        CHECK(!isInRange(Vector3{ .x = 500, .y = 0, .z = -500 }, Vector3{ .x = 500, .y = 0, .z = 501 }));
    }

    SECTION("Match Scans Offset")
    {
        char const sample2d[] =
            "--- scanner 0 ---" "\n"
            "0,2,0"             "\n"
            "4,1,0"             "\n"
            "3,3,0"             "\n"
            ""                  "\n"
            "--- scanner 1 ---" "\n"
            "-1,-1,0"           "\n"
            "-5,0,0"            "\n"
            "-2,1,0"            "\n";
        auto scans2d = parseInput(sample2d);
        CHECK(findOffset(scans2d[0], scans2d[1], 3));
        CHECK(scans2d[0].scanOffset == Vector3{ .x = 0, .y = 0, .z = 0 });
        CHECK(scans2d[1].scanOffset == Vector3{ .x = 5, .y = 2, .z = 0 });
    }

    SECTION("Transform")
    {
        char const sample_transform_input[] =
            "--- scanner 0 ---" "\n"
            "-1,-1,1"           "\n"
            "-2,-2,2"           "\n"
            "-3,-3,3"           "\n"
            "-2,-3,1"           "\n"
            "5,6,-4"            "\n"
            "8,0,7"             "\n";
        auto sample_scan = parseInput(sample_transform_input);
        REQUIRE(sample_scan.size() == 1);
        CHECK(sample_scan[0].transformationIndex == -1);
        auto const s0 = transformScan(sample_scan[0], 0);
        CHECK(s0.transformationIndex == 0);
        CHECK(s0.beacons[4] == Vector3{ .x = 5, .y = 6, .z = -4 });

        auto const s1 = transformScan(sample_scan[0], 1);
        CHECK(s1.transformationIndex == 1);
        CHECK(s1.beacons[4] == Vector3{ .x = 5, .y = 4, .z = 6 });

        auto const s2 = transformScan(sample_scan[0], 2);
        CHECK(s2.transformationIndex == 2);
        CHECK(s2.beacons[4] == Vector3{ .x = 5, .y = -6, .z = 4 });

        auto const s3 = transformScan(sample_scan[0], 3);
        CHECK(s3.transformationIndex == 3);
        CHECK(s3.beacons[4] == Vector3{ .x = 5, .y = -4, .z = -6 });
    }

    SECTION("Match Scans")
    {
        Scan c0 = scans[0];
        Scan c1 = scans[1];
        CHECK(matchScans(scans[0], c1));
        CHECK(c1.scanOffset == Vector3{ .x = 68, .y = -1246, .z = -43 });
        Scan c4 = scans[4];
        CHECK(matchScans(c1, c4));
        CHECK(c4.scanOffset == Vector3{ .x = -20, .y = -1133, .z = 1061 });
    }

    SECTION("Match all Scans")
    {
        auto matched_scans = matchAllScans(scans);
        CHECK(countBeacons(matched_scans) == 79);
        CHECK(largestDistance(matched_scans) == 3621);
    }
}
