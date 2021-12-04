#include <giant_squid.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <regex>
#include <string>

Game parseInput(std::string_view input)
{
    Game ret;
    enum class ParseState : int32_t {
        AwaitNumbers = 0,
        AwaitBoard,
        AwaitBoardLine1,
        AwaitBoardLine2,
        AwaitBoardLine3,
        AwaitBoardLine4,
        AwaitBoardLine5,
    } parseState = ParseState::AwaitNumbers;
    std::regex const rx_line(R"(^(\s+)?(\d+)(\s+)(\d+)(\s+)(\d+)(\s+)(\d+)(\s+)(\d+)(\s+)?$)");
    for (auto const& lr : (input | ranges::views::split('\n'))) {
        switch (parseState) {
        case ParseState::AwaitNumbers:
            for (auto const& nr : (lr | ranges::views::split(','))) {
                ret.numbers.push_back(std::stoi(ranges::to<std::string>(nr)));
            }
            parseState = ParseState::AwaitBoard;
            break;
         case ParseState::AwaitBoard:
             assert(ranges::to<std::string>(lr).empty());
             ret.boards.emplace_back();
             parseState = ParseState::AwaitBoardLine1;
             break;
         case ParseState::AwaitBoardLine1: [[fallthrough]];
         case ParseState::AwaitBoardLine2: [[fallthrough]];
         case ParseState::AwaitBoardLine3: [[fallthrough]];
         case ParseState::AwaitBoardLine4: [[fallthrough]];
         case ParseState::AwaitBoardLine5:
             std::smatch smatch;
             std::string const line = ranges::to<std::string>(lr);
             bool does_match = std::regex_match(line, smatch, rx_line);
             assert(does_match);
             int32_t const row = static_cast<int32_t>(parseState) - static_cast<int32_t>(ParseState::AwaitBoardLine1);
             ret.boards.back().board[row * 5] = std::stoi(smatch[2]);
             ret.boards.back().board[row * 5 + 1] = std::stoi(smatch[4]);
             ret.boards.back().board[row * 5 + 2] = std::stoi(smatch[6]);
             ret.boards.back().board[row * 5 + 3] = std::stoi(smatch[8]);
             ret.boards.back().board[row * 5 + 4] = std::stoi(smatch[10]);
             if (parseState == ParseState::AwaitBoardLine5) {
                 parseState = ParseState::AwaitBoard;
             } else {
                 parseState = static_cast<ParseState>(static_cast<int32_t>(parseState) + 1);
             }
             break;
        }
    }
    return ret;
}

bool isHit(Board const& b, int x, int y)
{
    assert(x >= 0 && x < 5);
    assert(y >= 0 && y < 5);
    return b.hits[y*5 + x];
}

bool markNumbers(Board& b, int32_t number)
{
    bool did_hit = false;
    for (int i = 0; i < 25; ++i) {
        if (b.board[i] == number) {
            assert(!b.hits[i]);
            b.hits[i] = true;
            did_hit = true;
        }
    }
    return did_hit;
}

std::optional<Bingo> isBingo(Board const& b)
{
    for (int row_to_check = 0; row_to_check < 5; ++row_to_check) {
        bool is_bingo = true;
        for (int i = 0; i < 5; ++i) {
            if (!isHit(b, i, row_to_check)) {
                is_bingo = false;
                break;
            }
        }
        if (is_bingo) { return Bingo{ Bingo::Kind::Row, row_to_check }; }
    }

    for (int col_to_check = 0; col_to_check < 5; ++col_to_check) {
        bool is_bingo = true;
        for (int i = 0; i < 5; ++i) {
            if (!isHit(b, col_to_check, i)) {
                is_bingo = false;
                break;
            }
        }
        if (is_bingo) { return Bingo{ Bingo::Kind::Column, col_to_check }; }
    }
    return std::nullopt;
}

int64_t sumUnmarked(Board const& b)
{
    int64_t ret = 0;
    for (int i = 0; i < 25; ++i) {
        if (!b.hits[i]) {
            ret += b.board[i];
        }
    }
    return ret;
}

std::optional<Bingo> playBoard(Board& b, int32_t number)
{
    if (markNumbers(b, number)) {
        return isBingo(b);
    }
    return std::nullopt;
}

std::optional<Outcome> playGame(Game& g, int32_t number)
{
    std::optional<Outcome> ret;
    for (int i = 0, i_end = static_cast<int>(g.boards.size()); i != i_end; ++i) {
        auto& b = g.boards[i];
        auto const bingo = playBoard(b, number);
        if (bingo) {
            if (!ret) {
                ret = Outcome{ .board_index = i, .score = sumUnmarked(b) };
            }
        }
    }
    return ret;
}

int64_t result1(Game g)
{
    for (auto const& n : g.numbers) {
        auto const bingo = playGame(g, n);
        if (bingo) {
            return (bingo->score) * n;
        }
    }
    return -1;
}

int64_t result2(Game g)
{
    for (auto const& n : g.numbers) {
        auto const bingo = playGame(g, n);
        if (bingo) {
            if (g.boards.size() == 1) {
                return bingo->score * n;
            } else {
                auto const it = std::remove_if(begin(g.boards) + bingo->board_index, end(g.boards), isBingo);
                g.boards.erase(it, end(g.boards));
            }
        }
    }
    return -1;
}
