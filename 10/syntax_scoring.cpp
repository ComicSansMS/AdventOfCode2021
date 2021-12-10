#include <syntax_scoring.hpp>

#include <range/v3/numeric/accumulate.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/enumerate.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/reverse.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>

std::vector<std::string> parseInput(std::string_view input)
{
    return input | ranges::views::split('\n') |
           ranges::to<std::vector<std::string>>;
}

SyntaxCheck checkSyntax(std::string_view line)
{
    SyntaxCheck ret;
    std::vector<char> check_stack;
    for (ret.column = 0; ret.column < line.size(); ++ret.column) {
        char const c = line[ret.column];
        switch (c) {
        case '(':
            check_stack.push_back(')');
            break;
        case '[':
            check_stack.push_back(']');
            break;
        case '{':
            check_stack.push_back('}');
            break;
        case '<':
            check_stack.push_back('>');
            break;
        case ')': [[fallthrough]];
        case ']': [[fallthrough]];
        case '}': [[fallthrough]];
        case '>':
            if (check_stack.back() != c) {
                ret.msg = fmt::format("Unexpected '{}' while trying to match '{}'.", c, check_stack.back());
                ret.status = SyntaxCheck::Status::Error;
                return ret;
            }
            check_stack.pop_back();
            break;
        default:
            ret.msg = fmt::format("Invalid character '{}'.", c);
            ret.status = SyntaxCheck::Status::Error;
            return ret;
        }
    }
    ret.status = (check_stack.empty()) ? SyntaxCheck::Status::Ok : SyntaxCheck::Status::Incomplete;
    ret.check_stack = std::move(check_stack);
    return ret;
}

int64_t result1(std::vector<std::string> const& lines)
{
    return ranges::accumulate(
        lines
            | ranges::views::transform(checkSyntax)
            | ranges::views::enumerate
            | ranges::views::filter([](auto const& p) -> bool {
                    auto const& [i, sc] = p;
                    return sc.status == SyntaxCheck::Status::Error;
                })
            | ranges::views::transform([&lines](auto const& p) -> char {
                    auto const& [i, sc] = p;
                    return lines[i][sc.column];
                })
            | ranges::views::transform([](char c) -> int64_t {
                    if (c == ')') {
                        return 3;
                    } else if(c == ']') {
                        return 57;
                    } else if (c == '}') {
                        return 1197;
                    } else {
                        assert(c == '>');
                        return 25137;
                    }
                })
        , static_cast<int64_t>(0));
}

int64_t scoreCheckStack(SyntaxCheck const& sc)
{
    return ranges::accumulate(
        sc.check_stack | ranges::views::reverse,
        static_cast<int64_t>(0),
        [](int64_t acc, int64_t n) -> int64_t {
            return acc*5 + n;
        },
        [](char c) -> int64_t {
            if (c == ')') {
                return 1;
            } else if (c == ']') {
                return 2;
            } else if (c == '}') {
                return 3;
            } else {
                assert(c == '>');
                return 4;
            }
        });
}

int64_t result2(std::vector<std::string> const& lines)
{
    std::vector<int64_t> scores = lines
        | ranges::views::transform(checkSyntax)
        | ranges::views::filter([](auto const& sc) -> bool { return sc.status == SyntaxCheck::Status::Incomplete; })
        | ranges::views::transform(scoreCheckStack)
        | ranges::to<std::vector<int64_t>>;
    std::partial_sort(begin(scores), begin(scores) + scores.size() / 2 + 1, end(scores));
    return scores[scores.size() / 2];
}
