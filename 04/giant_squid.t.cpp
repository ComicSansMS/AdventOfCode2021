#include <giant_squid.hpp>

#include <catch.hpp>

#include <range/v3/algorithm/all_of.hpp>

TEST_CASE("Giant Squid")
{
    char const sample_input[] =
        "7, 4, 9, 5, 11, 17, 23, 2, 0, 14, 21, 24, 10, 16, 13, 6, 15, 25, 12, 22, 18, 20, 8, 19, 3, 26, 1" "\n"
        ""                                                                                                 "\n"
        "22 13 17 11  0"                                                                                   "\n"
        "8  2 23  4 24"                                                                                    "\n"
        "21  9 14 16  7"                                                                                   "\n"
        "6 10  3 18  5"                                                                                    "\n"
        "1 12 20 15 19"                                                                                    "\n"
        ""                                                                                                 "\n"
        "3 15  0  2 22"                                                                                    "\n"
        "9 18 13 17  5"                                                                                    "\n"
        "19  8  7 25 23"                                                                                   "\n"
        "20 11 10 24  4"                                                                                   "\n"
        "14 21 16 12  6"                                                                                   "\n"
        ""                                                                                                 "\n"
        "14 21 17 24  4"                                                                                   "\n"
        "10 16 15  9 19"                                                                                   "\n"
        "18  8 23 26 20"                                                                                   "\n"
        "22 11 13  6  5"                                                                                   "\n"
        "2  0 12  3  7"                                                                                    "\n";

    Game game = parseInput(sample_input);
    SECTION("Parse Input")
    {
        CHECK(game.numbers == std::vector<int>{7, 4, 9, 5, 11, 17, 23, 2, 0, 14, 21, 24, 10, 16, 13, 6, 15, 25, 12, 22, 18, 20, 8, 19, 3, 26, 1});
        REQUIRE(game.boards.size() == 3);
        CHECK(game.boards[0].board == std::array{ 22, 13, 17, 11,  0,
                                                   8,  2, 23,  4, 24,
                                                  21,  9, 14, 16,  7,
                                                   6, 10,  3, 18,  5,
                                                   1, 12, 20, 15, 19 });
        CHECK(ranges::all_of(game.boards[0].hits, [](bool h) { return !h; }));

        CHECK(game.boards[1].board == std::array{ 3, 15,  0,  2, 22,
                                                  9, 18, 13, 17,  5,
                                                 19,  8,  7, 25, 23,
                                                 20, 11, 10, 24,  4,
                                                 14, 21, 16, 12,  6 });
        CHECK(ranges::all_of(game.boards[1].hits, [](bool h) { return !h; }));

        CHECK(game.boards[2].board == std::array{ 14, 21, 17, 24,  4,
                                                  10, 16, 15,  9, 19,
                                                  18,  8, 23, 26, 20,
                                                  22, 11, 13,  6,  5,
                                                   2 , 0, 12,  3,  7 });
        CHECK(ranges::all_of(game.boards[2].hits, [](bool h) { return !h; }));
    }
    SECTION("Is Hit")
    {
        CHECK(!isHit(game.boards[0], 0, 0));
        CHECK(!isHit(game.boards[0], 0, 1));
        CHECK(!isHit(game.boards[0], 1, 0));

        game.boards[0].hits[1] = true;
        CHECK(!isHit(game.boards[0], 0, 0));
        CHECK(!isHit(game.boards[0], 0, 1));
        CHECK(isHit(game.boards[0], 1, 0));

        game.boards[0].hits[5] = true;
        CHECK(!isHit(game.boards[0], 0, 0));
        CHECK(isHit(game.boards[0], 0, 1));
        CHECK(isHit(game.boards[0], 1, 0));

        game.boards[0].hits[0] = true;
        CHECK(isHit(game.boards[0], 0, 0));
        CHECK(isHit(game.boards[0], 0, 1));
        CHECK(isHit(game.boards[0], 1, 0));
    }

    SECTION("Mark Numbers")
    {
        CHECK(game.boards[0].hits == std::array{ false, false, false, false, false,
                                                 false, false, false, false, false,
                                                 false, false, false, false, false,
                                                 false, false, false, false, false,
                                                 false, false, false, false, false });

        CHECK(markNumbers(game.boards[0], 7));
        CHECK(game.boards[0].hits == std::array{ false, false, false, false, false,
                                                 false, false, false, false, false,
                                                 false, false, false, false, true,
                                                 false, false, false, false, false,
                                                 false, false, false, false, false });

        CHECK(markNumbers(game.boards[0], 4));
        CHECK(game.boards[0].hits == std::array{ false, false, false, false, false,
                                                 false, false, false, true,  false,
                                                 false, false, false, false, true,
                                                 false, false, false, false, false,
                                                 false, false, false, false, false });

        CHECK(markNumbers(game.boards[0], 9));
        CHECK(game.boards[0].hits == std::array{ false, false, false, false, false,
                                                 false, false, false, true,  false,
                                                 false, true,  false, false, true,
                                                 false, false, false, false, false,
                                                 false, false, false, false, false });

        CHECK(markNumbers(game.boards[0], 5));
        CHECK(game.boards[0].hits == std::array{ false, false, false, false, false,
                                                 false, false, false, true,  false,
                                                 false, true,  false, false, true,
                                                 false, false, false, false, true,
                                                 false, false, false, false, false });

        CHECK(markNumbers(game.boards[0], 11));
        CHECK(game.boards[0].hits == std::array{ false, false, false, true,  false,
                                                 false, false, false, true,  false,
                                                 false, true,  false, false, true,
                                                 false, false, false, false, true,
                                                 false, false, false, false, false });

        CHECK(!markNumbers(game.boards[0], 59));
        CHECK(game.boards[0].hits == std::array{ false, false, false, true,  false,
                                                 false, false, false, true,  false,
                                                 false, true,  false, false, true,
                                                 false, false, false, false, true,
                                                 false, false, false, false, false });
    }

    SECTION("Is Bingo")
    {
        Board b{};
        b.hits = std::array{ false, false, false, false, false,
                             false, false, false, false, false,
                             false, false, false, false, false,
                             false, false, false, false, false,
                             false, false, false, false, false };
        CHECK(!isBingo(b));
        b.hits = std::array{ false, false, false, true,  false,
                             false, false, false, true,  false,
                             false, true,  false, false, true,
                             false, false, false, false, true,
                             false, false, false, false, false };
        CHECK(!isBingo(b));

        b.hits = std::array{ true,  true,  true,  true,  true,
                             false, false, false, true,  false,
                             false, true,  false, false, true,
                             false, false, false, false, true,
                             false, false, false, false, false };
        {
            auto const bingo = isBingo(b);
            REQUIRE(bingo);
            CHECK(bingo->kind == Bingo::Kind::Row);
            CHECK(bingo->i == 0);
        }

        b.hits = std::array{ false, false, false, true,  false,
                             false, false, false, true,  false,
                             true,  true,  true,  true,  true,
                             false, false, false, false, true,
                             false, false, false, false, false };
        {
            auto const bingo = isBingo(b);
            REQUIRE(bingo);
            CHECK(bingo->kind == Bingo::Kind::Row);
            CHECK(bingo->i == 2);
        }

        b.hits = std::array{ false, false, false, true,  false,
                             false, false, false, true,  false,
                             false, true,  false, false, true,
                             false, false, false, false, true,
                             true,  true,  true,  true,  true };
        {
            auto const bingo = isBingo(b);
            REQUIRE(bingo);
            CHECK(bingo->kind == Bingo::Kind::Row);
            CHECK(bingo->i == 4);
        }

        b.hits = std::array{ true,  false, false, true,  false,
                             true,  false, false, true,  false,
                             true,  true,  false, false, true,
                             true,  false, false, false, true,
                             true,  false, false, false, false };
        {
            auto const bingo = isBingo(b);
            REQUIRE(bingo);
            CHECK(bingo->kind == Bingo::Kind::Column);
            CHECK(bingo->i == 0);
        }

        b.hits = std::array{ false, false, true, true,  false,
                             false, false, true, true,  false,
                             false, true,  true, false, true,
                             false, false, true, false, true,
                             false, false, true, false, false };
        {
            auto const bingo = isBingo(b);
            REQUIRE(bingo);
            CHECK(bingo->kind == Bingo::Kind::Column);
            CHECK(bingo->i == 2);
        }

        b.hits = std::array{ false, false, false, true,  true,
                             false, false, false, true,  true,
                             false, true,  false, false, true,
                             false, false, false, false, true,
                             false, false, false, false, true };
        {
            auto const bingo = isBingo(b);
            REQUIRE(bingo);
            CHECK(bingo->kind == Bingo::Kind::Column);
            CHECK(bingo->i == 4);
        }
    }

    SECTION("Sum Unmarked")
    {
        CHECK(sumUnmarked(game.boards[2]) == 325);
        game.boards[2].hits = std::array{ true,  true,  true,  true,  true,
                                          false, false, false, true,  false,
                                          false, false, true,  false, false,
                                          false, true,  false, false, true,
                                          true,  true,  false, false, true  };
        CHECK(sumUnmarked(game.boards[2]) == 188);
    }

    SECTION("Play Board")
    {
        CHECK(!playBoard(game.boards[0], game.numbers[0]));
        CHECK(game.boards[0].hits ==
            std::array{ false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, false, false, true,
                        false, false, false, false, false,
                        false, false, false, false, false });
        CHECK(!playBoard(game.boards[0], 99));
        CHECK(game.boards[0].hits ==
            std::array{ false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, false, false, true,
                        false, false, false, false, false,
                        false, false, false, false, false });
    }

    SECTION("Play Game")
    {
        CHECK(!playGame(game, game.numbers[0]));
        CHECK(game.boards[0].hits ==
            std::array{ false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, false, false, true,
                        false, false, false, false, false,
                        false, false, false, false, false });
        CHECK(game.boards[1].hits ==
            std::array{ false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, true,  false, false,
                        false, false, false, false, false,
                        false, false, false, false, false });
        CHECK(game.boards[2].hits ==
            std::array{ false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, false, false, false,
                        false, false, false, false, true });
        CHECK(!playGame(game, game.numbers[1]));
        CHECK(!playGame(game, game.numbers[2]));
        CHECK(!playGame(game, game.numbers[3]));
        CHECK(!playGame(game, game.numbers[4]));
        CHECK(game.boards[0].hits ==
            std::array{ false, false, false, true,  false,
                        false, false, false, true,  false,
                        false, true,  false, false, true,
                        false, false, false, false, true,
                        false, false, false, false, false });
        CHECK(game.boards[1].hits ==
            std::array{ false, false, false, false, false,
                        true,  false, false, false, true,
                        false, false, true,  false, false,
                        false, true,  false, false, true,
                        false, false, false, false, false });
        CHECK(game.boards[2].hits ==
            std::array{ false, false, false, false, true,
                        false, false, false, true,  false,
                        false, false, false, false, false,
                        false, true,  false, false, true,
                        false, false, false, false, true });
        CHECK(!playGame(game, game.numbers[5]));
        CHECK(!playGame(game, game.numbers[6]));
        CHECK(!playGame(game, game.numbers[7]));
        CHECK(!playGame(game, game.numbers[8]));
        CHECK(!playGame(game, game.numbers[9]));
        CHECK(!playGame(game, game.numbers[10]));
        CHECK(game.boards[0].hits ==
            std::array{ false, false, true,  true,  true,
                        false, true,  true,  true,  false,
                        true,  true,  true,  false, true,
                        false, false, false, false, true,
                        false, false, false, false, false });
        CHECK(game.boards[1].hits ==
            std::array{ false, false, true,  true,  false,
                        true,  false, false, true,  true,
                        false, false, true,  false, true,
                        false, true,  false, false, true,
                        true,  true,  false, false, false });
        CHECK(game.boards[2].hits ==
            std::array{ true,  true,  true,  false, true,
                        false, false, false, true,  false,
                        false, false, true,  false, false,
                        false, true,  false, false, true,
                        true,  true,  false, false, true });
        auto const bingo = playGame(game, game.numbers[11]);
        REQUIRE(bingo);
        CHECK(bingo->score == 188);
        CHECK(bingo->board_index == 2);
        CHECK(game.boards[0].hits ==
            std::array{ false, false, true,  true,  true,
                        false, true,  true,  true,  true,
                        true,  true,  true,  false, true,
                        false, false, false, false, true,
                        false, false, false, false, false });
        CHECK(game.boards[1].hits ==
            std::array{ false, false, true,  true,  false,
                        true,  false, false, true,  true,
                        false, false, true,  false, true,
                        false, true,  false, true,  true,
                        true,  true,  false, false, false });
        CHECK(game.boards[2].hits ==
            std::array{ true,  true,  true,  true,  true,
                        false, false, false, true,  false,
                        false, false, true,  false, false,
                        false, true,  false, false, true,
                        true,  true,  false, false, true });
    }

    SECTION("Result 1")
    {
        CHECK(result1(game) == 4512);
        game.numbers.resize(12);
        CHECK(result1(game) == 4512);
        game.numbers.resize(11);
        CHECK(result1(game) == -1);
    }

    SECTION("Result 2")
    {
        CHECK(result2(game) == 1924);
        game.numbers.resize(15);
        CHECK(result2(game) == 1924);
        game.numbers.resize(14);
        CHECK(result2(game) == -1);
    }
}
