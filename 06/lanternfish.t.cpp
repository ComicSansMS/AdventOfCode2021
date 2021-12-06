#include <lanternfish.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Hydrothermal Venture")
{
    char const sample_input[] = "3,4,3,1,2\n";

    Fishes fishes = parseInput(sample_input);

    SECTION("Parse Input")
    {
        CHECK(fishes.a[0] == 0);
        CHECK(fishes.a[1] == 1);
        CHECK(fishes.a[2] == 1);
        CHECK(fishes.a[3] == 2);
        CHECK(fishes.a[4] == 1);
        CHECK(fishes.a[5] == 0);
        CHECK(fishes.a[6] == 0);
        CHECK(fishes.a[7] == 0);
        CHECK(fishes.a[8] == 0);
        CHECK(fishes.a[9] == 0);
    }

    SECTION("Simulate Day")
    {
        fishes = simulateDay(fishes);
        //After 1 day: 2,3,2,0,1
        CHECK(fishes.a[0] == 1);
        CHECK(fishes.a[1] == 1);
        CHECK(fishes.a[2] == 2);
        CHECK(fishes.a[3] == 1);
        CHECK(fishes.a[4] == 0);
        CHECK(fishes.a[5] == 0);
        CHECK(fishes.a[6] == 0);
        CHECK(fishes.a[7] == 0);
        CHECK(fishes.a[8] == 0);
        CHECK(fishes.a[9] == 0);
        fishes = simulateDay(fishes);
        //After 2 days: 1,2,1,6,0,8
        CHECK(fishes.a == std::array<int64_t, 10>{ 1, 2, 1, 0, 0, 0, 1, 0, 1, 0 });

        fishes = simulateDay(fishes);
        //After 3 days: 0,1,0,5,6,7,8
        CHECK(fishes.a == std::array<int64_t, 10>{ 2, 1, 0, 0, 0, 1, 1, 1, 1, 0 });

        fishes = simulateDay(fishes);
        //After 4 days: 6,0,6,4,5,6,7,8,8
        CHECK(fishes.a == std::array<int64_t, 10>{ 1, 0, 0, 0, 1, 1, 3, 1, 2, 0 });

        fishes = simulateDay(fishes);
        //After 5 days: 5,6,5,3,4,5,6,7,7,8
        CHECK(fishes.a == std::array<int64_t, 10>{ 0, 0, 0, 1, 1, 3, 2, 2, 1, 0 });

        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        //After 10 days: 0,1,0,5,6,0,1,2,2,3,7,8
        CHECK(fishes.a == std::array<int64_t, 10>{ 3, 2, 2, 1, 0, 1, 1, 1, 1, 0 });

        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        fishes = simulateDay(fishes);
        //After 18 days: 6, 0, 6, 4, 5, 6, 0, 1, 1, 2, 6, 0, 1, 1, 1, 2, 2, 3, 3, 4, 6, 7, 8, 8, 8, 8
        CHECK(fishes.a == std::array<int64_t, 10>{ 3, 5, 3, 2, 2, 1, 5, 1, 4, 0 });
    }

    SECTION("Result")
    {
        CHECK(result(fishes, 18) == 26);
        CHECK(result(fishes, 80) == 5934);
        CHECK(result(fishes, 256) == 26984457539);
    }
}
