#include <dirac_dice.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>

#include <algorithm>
#include <cassert>

Board parseInput(std::string_view input)
{
    Board ret{};
    auto const first_lb = input.find("\n", 0);
    auto const line1 = input.substr(0, first_lb);
    assert(line1.starts_with("Player "));
    ret.player1 = std::stoi(std::string(line1.substr(27)));
    auto const second_lb = input.find("\n", first_lb + 1);
    auto const line2 = input.substr(first_lb + 1, second_lb - first_lb - 1);
    assert(line2.starts_with("Player "));
    ret.player2 = std::stoi(std::string(line2.substr(27)));
    assert(second_lb == input.size() - 1);
    return ret;
}

int movePlayer(int current_position, int die1, int die2, int die3)
{
    int const advance = die1 + die2 + die3;
    return (((current_position - 1) + advance) % 10) + 1;
}

int64_t result1(Board b)
{
    struct Score {
        int64_t s1;
        int64_t s2;
    } score = {};
    int die = 0;
    auto const roll_die = [&die]() -> int { int ret = die + 1; die = ((die + 1) % 10); return ret; };
    auto const makeMove = [&roll_die](int& p, int64_t& score) -> bool {
        p = movePlayer(p, roll_die(), roll_die(), roll_die());
        score += static_cast<int64_t>(p);
        return (score >= 1000);
    };
    int64_t die_count = 0;
    for (;;) {
        die_count += 3;
        if (makeMove(b.player1, score.s1)) { break; }
        die_count += 3;
        if (makeMove(b.player2, score.s2)) { break; }
    }
    return std::min(score.s1, score.s2) * die_count;
}

int64_t result2(Board b)
{
    std::vector<std::unordered_map<State, int64_t, HashState>> states_map;
    states_map.emplace_back();
    State const initial_state{
        .p1_pos = b.player1,
        .p2_pos = b.player2,
        .p1_score = 0,
        .p2_score = 0
    };
    states_map.back().insert({ initial_state, 1 });
    struct Roll {
        int die_sum;
        int count;
    };
    std::array<Roll, 7> const rolls{
        Roll{ .die_sum = 3, .count = 1 },
        Roll{ .die_sum = 4, .count = 3 },
        Roll{ .die_sum = 5, .count = 6 },
        Roll{ .die_sum = 6, .count = 7 },
        Roll{ .die_sum = 7, .count = 6 },
        Roll{ .die_sum = 8, .count = 3 },
        Roll{ .die_sum = 9, .count = 1 },
    };
    int64_t total_wins_p1 = 0;
    int64_t total_wins_p2 = 0;
    for (int round_no = 1;; ++round_no) {
        states_map.emplace_back();
        auto const& last_states = states_map[states_map.size() - 2];
        auto& new_states = states_map.back();
        for (auto const& [old_state, multiplier] : last_states) {
            for (auto const& roll1 : rolls) {
                int const new_p1 = movePlayer(old_state.p1_pos, roll1.die_sum, 0, 0);
                if (old_state.p1_score + new_p1 >= 21) {
                    total_wins_p1 += multiplier * roll1.count;
                    continue;
                }
                for (auto const& roll2 : rolls) {
                    int const new_p2 = movePlayer(old_state.p2_pos, roll2.die_sum, 0, 0);
                    State new_state{
                        .p1_pos = new_p1,
                        .p2_pos = new_p2,
                        .p1_score = old_state.p1_score + new_p1,
                        .p2_score = old_state.p2_score + new_p2,
                    };
                    assert (new_state.p1_score < 21);
                    if (new_state.p2_score >= 21) {
                        total_wins_p2 += multiplier * roll1.count * roll2.count;
                    } else {
                        new_states[new_state] += multiplier * roll1.count * roll2.count;
                    }
                }
            }
        }
        if (new_states.empty()) { break; }
        if (round_no > 13) { break; }
    }
    return std::max(total_wins_p1, total_wins_p2);
}
