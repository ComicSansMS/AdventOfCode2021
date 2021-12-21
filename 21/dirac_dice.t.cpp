#include <dirac_dice.hpp>

#include <catch.hpp>

TEST_CASE("Trench Map")
{
    char const sample_input[] = 
        "Player 1 starting position: 4" "\n"
        "Player 2 starting position: 8" "\n";

    auto const start = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(start.player1 == 4);
        CHECK(start.player2 == 8);
    }

    SECTION("Move Player")
    {
        CHECK(movePlayer(7, 2, 2, 1) == 2);

        CHECK(movePlayer(4, 1, 2, 3) == 10);
        CHECK(movePlayer(8, 4, 5, 6) == 3);
        CHECK(movePlayer(10, 7, 8, 9) == 4);
        CHECK(movePlayer(3, 10, 11, 12) == 6);
        CHECK(movePlayer(4, 13, 14, 15) == 6);
        CHECK(movePlayer(6, 16, 17, 18) == 7);
        CHECK(movePlayer(6, 19, 20, 21) == 6);
        CHECK(movePlayer(7, 22, 23, 24) == 6);
    }

    SECTION("Result 1")
    {
        CHECK(result1(start) == 739785);
    }

    SECTION("Result 2")
    {
        CHECK(result2(start) == 444356092776315);
    }
}
