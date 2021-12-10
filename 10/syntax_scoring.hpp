#ifndef ADVENT_OF_CODE_10_SYNTAX_SCORING_INCLUDE_GUARD
#define ADVENT_OF_CODE_10_SYNTAX_SCORING_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <ostream>
#include <vector>

std::vector<std::string> parseInput(std::string_view input);

struct SyntaxCheck {
    enum class Status {
        Ok,
        Error,
        Incomplete
    } status;
    int column;

    std::string msg;
    std::vector<char> check_stack;

    friend bool operator==(SyntaxCheck const& lhs, SyntaxCheck const& rhs) {
        return (lhs.status == rhs.status) && (lhs.column == rhs.column);
    }
};

template<>
struct fmt::formatter<SyntaxCheck::Status>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(SyntaxCheck::Status const& s, FormatContext& ctx) {
        switch (s) {
        case SyntaxCheck::Status::Ok:
            fmt::format_to(ctx.out(), "Ok");
            break;
        case SyntaxCheck::Status::Error:
            fmt::format_to(ctx.out(), "Error");
            break;
        case SyntaxCheck::Status::Incomplete:
            fmt::format_to(ctx.out(), "Incomplete");
            break;
        }
        return ctx.out();
    }
};


template<>
struct fmt::formatter<SyntaxCheck>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(SyntaxCheck const& sc, FormatContext& ctx) {
        fmt::format_to(ctx.out(), "{{ .status = {}, .column = {}, .msg = \"{}\" }}", sc.status, sc.column, sc.msg);
        return ctx.out();
    }
};

inline std::ostream& operator<<(std::ostream& os, SyntaxCheck const& sc) {
    return os << fmt::format("{}", sc);
}

SyntaxCheck checkSyntax(std::string_view line);

int64_t result1(std::vector<std::string> const& lines);

int64_t scoreCheckStack(SyntaxCheck const& sc);

int64_t result2(std::vector<std::string> const& lines);

#endif
