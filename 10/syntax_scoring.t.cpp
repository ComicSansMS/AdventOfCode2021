#include <syntax_scoring.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("Syntax Scoring")
{
    char const sample_input[] = "[({(<(())[]>[[{[]{<()<>>"  "\n"
                                "[(()[<>])]({[<{<<[]>>("    "\n"
                                "{([(<{}[<>[]}>{[]{[(<()>"  "\n"
                                "(((({<>}<{<{<>}{[]{[]{}"   "\n"
                                "[[<[([]))<([[{}[[()]]]"    "\n"
                                "[{[{({}]{}}([{[{{{}}([]"   "\n"
                                "{<[[]]>}<{[{[{[]{()[[[]"   "\n"
                                "[<(<(<(<{}))><([]([]()"    "\n"
                                "<{([([[(<>()){}]>(<<{{"    "\n"
                                "<{([{{}}[<[[[<>{}]]]>[]]"  "\n";

    auto const lines = parseInput(sample_input);

    SECTION("Parse Input")
    {
        REQUIRE(lines.size() == 10);
        CHECK(lines[0] == "[({(<(())[]>[[{[]{<()<>>");
        CHECK(lines[1] == "[(()[<>])]({[<{<<[]>>(");
        CHECK(lines[2] == "{([(<{}[<>[]}>{[]{[(<()>");
        CHECK(lines[3] == "(((({<>}<{<{<>}{[]{[]{}");
        CHECK(lines[4] == "[[<[([]))<([[{}[[()]]]");
        CHECK(lines[5] == "[{[{({}]{}}([{[{{{}}([]");
        CHECK(lines[6] == "{<[[]]>}<{[{[{[]{()[[[]");
        CHECK(lines[7] == "[<(<(<(<{}))><([]([]()");
        CHECK(lines[8] == "<{([([[(<>()){}]>(<<{{");
        CHECK(lines[9] == "<{([{{}}[<[[[<>{}]]]>[]]");
    }

    SECTION("SyntaxCheck to String")
    {
        {
            std::stringstream sstr;
            sstr << SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 42, .msg = {}, .check_stack = {} };
            CHECK(sstr.str() == "{ .status = Ok, .column = 42, .msg = \"\" }");
        }
        {
            std::stringstream sstr;
            sstr << SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = -1, .msg = {}, .check_stack = {} };
            CHECK(sstr.str() == "{ .status = Error, .column = -1, .msg = \"\" }");
        }
        {
            std::stringstream sstr;
            sstr << SyntaxCheck{ .status = SyntaxCheck::Status::Incomplete, .column = 1234567890, .msg = {}, .check_stack = {} };
            CHECK(sstr.str() == "{ .status = Incomplete, .column = 1234567890, .msg = \"\" }");
        }
        {
            std::stringstream sstr;
            sstr << SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 0, .msg = "foo", .check_stack = {} };
            CHECK(sstr.str() == "{ .status = Ok, .column = 0, .msg = \"foo\" }");
        }

    }

    SECTION("Syntax Check Equality")
    {
        CHECK(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} });
        CHECK(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "bar", .check_stack = {} });
        CHECK(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "bar", .check_stack = { 1 } });
        CHECK(!(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 1, .msg = "foo", .check_stack = {} }));
        CHECK(!(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 2, .msg = "foo", .check_stack = {} }));
        CHECK(!(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 2, .msg = "foo", .check_stack = {} }));
        CHECK(!(SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 1, .msg = "foo", .check_stack = {} } == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 2, .msg = "bar", .check_stack = {} }));
    }

    SECTION("Check Syntax")
    {
        CHECK(checkSyntax("()") == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 2, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("([])") == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 4, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("{()()()}") == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 8, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("<([{}])>") == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 8, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("[<>({}){}[([])<>]]") == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 18, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("(((((((((())))))))))") == SyntaxCheck{ .status = SyntaxCheck::Status::Ok, .column = 20, .msg = {}, .check_stack = {} });
        
        CHECK(checkSyntax("(]") == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 1, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("{()()()>") == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 7, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("(((()))}") == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 7, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax("<([]){()}[{}])") == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 13, .msg = {}, .check_stack = {} });

        CHECK(checkSyntax("()()1()") == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 4, .msg = {}, .check_stack = {} });


        CHECK(checkSyntax(lines[0]) == SyntaxCheck{ .status = SyntaxCheck::Status::Incomplete, .column = 24, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[1]) == SyntaxCheck{ .status = SyntaxCheck::Status::Incomplete, .column = 22, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[3]) == SyntaxCheck{ .status = SyntaxCheck::Status::Incomplete, .column = 23, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[6]) == SyntaxCheck{ .status = SyntaxCheck::Status::Incomplete, .column = 23, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[9]) == SyntaxCheck{ .status = SyntaxCheck::Status::Incomplete, .column = 24, .msg = {}, .check_stack = {} });

        CHECK(checkSyntax(lines[2]) == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 12, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[4]) == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 8, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[5]) == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 7, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[7]) == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 10, .msg = {}, .check_stack = {} });
        CHECK(checkSyntax(lines[8]) == SyntaxCheck{ .status = SyntaxCheck::Status::Error, .column = 16, .msg = {}, .check_stack = {} });
    }

    SECTION("Result 1")
    {
        CHECK(result1(lines) == 26397);
    }

    SECTION("Check Stack")
    {
        {
            auto const stack = checkSyntax(lines[0]).check_stack;
            CHECK(std::string{ rbegin(stack), rend(stack) } == "}}]])})]");
        }
        {
            auto const stack = checkSyntax(lines[1]).check_stack;
            CHECK(std::string{ rbegin(stack), rend(stack) } == ")}>]})");
        }
        {
            auto const stack = checkSyntax(lines[3]).check_stack;
            CHECK(std::string{ rbegin(stack), rend(stack) } == "}}>}>))))");
        }
        {
            auto const stack = checkSyntax(lines[6]).check_stack;
            CHECK(std::string{ rbegin(stack), rend(stack) } == "]]}}]}]}>");
        }
        {
            auto const stack = checkSyntax(lines[9]).check_stack;
            CHECK(std::string{ rbegin(stack), rend(stack) } == "])}>");
        }
    }

    SECTION("Score Check Stack")
    {
        CHECK(scoreCheckStack(checkSyntax(lines[0])) == 288957);
        CHECK(scoreCheckStack(checkSyntax(lines[1])) == 5566);
        CHECK(scoreCheckStack(checkSyntax(lines[3])) == 1480781);
        CHECK(scoreCheckStack(checkSyntax(lines[6])) == 995444);
        CHECK(scoreCheckStack(checkSyntax(lines[9])) == 294);
    }

    SECTION("Result 2")
    {
        CHECK(result2(lines) == 288957);
    }
}
