#include <dive.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <cassert>
#include <stdexcept>
#include <string>

std::vector<Command> parseInput(std::string_view input)
{
    return ranges::to<std::vector<Command>>(input | ranges::views::split('\n') |
        ranges::views::transform([](auto inrng) -> Command {
            std::string const str = ranges::to<std::string>(inrng);
            char const str_forward[] = "forward ";
            char const str_down[] = "down ";
            char const str_up[] = "up ";
            if (str.starts_with(str_forward)) {
                int64_t const n = std::stoi(str.substr(sizeof(str_forward) - 1));
                return Forward{ .n = n };
            } else if (str.starts_with(str_down)) {
                int64_t const n = std::stoi(str.substr(sizeof(str_down) - 1));
                return Down{ .n = n };
            } else if (str.starts_with(str_up)) {
                int64_t const n = std::stoi(str.substr(sizeof(str_up) - 1));
                return Up{ .n = n };
            } else {
                assert(str.empty());
            }
            throw std::logic_error{ "Unreachable" };
        }));
}

void courseCommand(Position& pos, Forward const& f)
{
    pos.x += f.n;
}

void courseCommand(Position& pos, Down const& d)
{
    pos.y += d.n;
}

void courseCommand(Position& pos, Up const& u)
{
    pos.y -= u.n;
}

Position plotCourse(std::vector<Command> const& commands)
{
    Position pos{};
    for (auto const& cmd : commands) {
        std::visit([&pos](auto v) { courseCommand(pos, v); }, cmd);
    }
    return pos;
}

void courseCommand2(Submarine& sub, Forward const& f)
{
    sub.x += f.n;
    sub.y += sub.aim * f.n;
}

void courseCommand2(Submarine& sub, Down const& d)
{
    sub.aim += d.n;;
}

void courseCommand2(Submarine& sub, Up const& u)
{
    sub.aim -= u.n;
}

Submarine plotCourse2(std::vector<Command> const& commands)
{
    Submarine sub{};
    for (auto const& cmd : commands) {
        std::visit([&sub](auto v) { courseCommand2(sub, v); }, cmd);
    }
    return sub;
}
