#include <transparent_origami.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/view/split.hpp>

#include <algorithm>
#include <cassert>

Transparency parseInput(std::string_view input)
{
    Transparency ret{};
    ret.max = Point{ .x = 0, .y = 0 };

    enum class ParseState {
        InPoints,
        InFolds
    } state = ParseState::InPoints;

    for (auto const& rline : input | ranges::views::split('\n')) {
        if (rline.empty()) {
            assert(state == ParseState::InPoints);
            state = ParseState::InFolds;
        } else if (state == ParseState::InPoints) {
            auto const rpoints = rline | ranges::views::split(',');
            auto it_rpoints = ranges::begin(rpoints);
            int px = std::stoi(ranges::to<std::string>(*it_rpoints));
            ++it_rpoints;
            int py = std::stoi(ranges::to<std::string>(*it_rpoints));
            ++it_rpoints;
            assert(it_rpoints == ranges::end(rpoints));
            ret.max.x = std::max(ret.max.x, px);
            ret.max.y = std::max(ret.max.y, py);
            ret.points.insert(Point{ .x = px, .y = py });
        } else {
            assert(state == ParseState::InFolds);
            std::string fold_str = ranges::to<std::string>(rline);
            if (fold_str.starts_with("fold along x=")) {
                Fold f{ .coordinate = std::stoi(fold_str.substr(13)), .axis = Fold::Axis::X };
                ret.folds.push_back(f);
            } else {
                assert(fold_str.starts_with("fold along y="));
                Fold f{ .coordinate = std::stoi(fold_str.substr(13)), .axis = Fold::Axis::Y };
                ret.folds.push_back(f);
            }
        }
    }
    return ret;
}

Transparency fold(Transparency const& t)
{
    Transparency ret{ .points = {}, .folds = t.folds, .activeFold = {}, .max = t.max };
    if (t.activeFold) {
        Fold const& f = t.folds[*t.activeFold];
        int const c = f.coordinate;
        if (f.axis == Fold::Axis::X) {
            for (auto p : t.points) {
                assert(p.x != c);
                if (p.x > c) {
                    p.x = (c * 2) - p.x;
                }
                ret.points.insert(p);
            }
            ret.max.x = c - 1;
        } else {
            assert(f.axis == Fold::Axis::Y);
            for (auto p : t.points) {
                assert(p.y != c);
                if (p.y > c) {
                    p.y = (c * 2) - p.y;
                }
                ret.points.insert(p);
            }
            ret.max.y = c - 1;
        }
    }
    return ret;
}

std::size_t result1(Transparency t)
{
    t.activeFold = 0;
    t = fold(t);
    return t.points.size();
}

Transparency foldAll(Transparency const& t)
{
    Transparency ret = t;
    for (std::size_t i = 0; i < ret.folds.size(); ++i) {
        ret.activeFold = i;
        ret = fold(ret);
    }
    return ret;
}
