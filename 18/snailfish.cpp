#include <snailfish.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/max.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/filter.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <optional>

std::vector<PairPtr> parseInput(PairPool& pool, std::string_view input)
{
    return input
        | ranges::views::split('\n')
        | ranges::views::transform([](auto const& rng) -> std::string { return ranges::to<std::string>(rng); })
        | ranges::views::transform([&pool](std::string const& line) -> Pair { return parseLine(pool, line); })
        | ranges::views::transform([&pool](Pair&& pair) -> PairPtr { return pool.createPairPtr(std::move(pair)); })
        | ranges::to<std::vector>;
}

Pair parseLine(PairPool& pool, std::string_view line)
{
    enum class ParseState {
        Start,
        Element1,
        Comma,
        Element2,
        End
    };
    std::vector<ParseState> stack;
    std::vector<Pair> pair_stack;
    stack.push_back(ParseState::Start);
    for (char const c : line) {
        assert(!stack.empty());
        ParseState& s = stack.back();
        if (s == ParseState::Start) {
            assert(c == '[');
            pair_stack.emplace_back();
            s = ParseState::Element1;
        } else if (s == ParseState::Element1) {
            if ((c >= '0') && (c <= '9')) {
                int64_t const n = c - '0';
                pair_stack.back().first = Number{ .n = n };
                s = ParseState::Comma;
            } else {
                assert(c == '[');
                pair_stack.emplace_back();
                stack.push_back(ParseState::Element1);
            }
        } else if (s == ParseState::Comma) {
            assert(c == ',');
            s = ParseState::Element2;
        } else if (s == ParseState::Element2) {
            if ((c >= '0') && (c <= '9')) {
                int64_t const n = c - '0';
                pair_stack.back().second = Number{ .n = n };
                s = ParseState::End;
            } else {
                assert(c == '[');
                pair_stack.emplace_back();
                stack.push_back(ParseState::Element1);
            }
        } else if (s == ParseState::End) {
            assert(c == ']');
            stack.pop_back();
            if (!stack.empty()) {
                assert(pair_stack.size() > 1);
                ParseState& s_up = stack.back();
                if (s_up == ParseState::Element1) {
                    Pair p = std::move(pair_stack.back());
                    pair_stack.pop_back();
                    pair_stack.back().first = pool.createPairPtr(std::move(p));
                    s_up = ParseState::Comma;
                } else {
                    assert(s_up == ParseState::Element2);
                    Pair p = std::move(pair_stack.back());
                    pair_stack.pop_back();
                    pair_stack.back().second = pool.createPairPtr(std::move(p));
                    s_up = ParseState::End;
                }
            }

        }
    }
    assert(stack.empty());
    assert(pair_stack.size() == 1);
    return std::move(pair_stack.back());
}

int64_t getNumber(Element const& e)
{
    struct ExtractNumberVisitor {
        int64_t operator()(Number const& n) {
            return n.n;
        }

        int64_t operator()(PairPtr const&) {    // LCOV_EXCL_LINE
            return -99;                         // LCOV_EXCL_LINE
        }                                       // LCOV_EXCL_LINE
    };
    return std::visit(ExtractNumberVisitor{}, e);
}

Pair* getPair(Element const& e)
{
    struct ExtractPairVisitor {
        Pair* operator()(Number const&) {       // LCOV_EXCL_LINE
            return nullptr;                     // LCOV_EXCL_LINE
        }                                       // LCOV_EXCL_LINE

        Pair* operator()(PairPtr const& p) {
            return p.get();
        }
    };
    return std::visit(ExtractPairVisitor{}, e);
}

PairPtr clone(PairPool& pool, Pair const& p)
{
    struct CloneElementVisitor {
        PairPool* m_pool;
        CloneElementVisitor(PairPool& pool) :m_pool(&pool) {}
        Element operator()(Number const& n) { return n; }
        Element operator()(PairPtr const& p) {
            return clone(*m_pool, *p);
        }
    };
    auto const cloneElement = [&pool](Element const& e) -> Element {
        return std::visit(CloneElementVisitor{ pool }, e);
    };
    return pool.createPairPtr(Pair{ .first = cloneElement(p.first), .second = cloneElement(p.second) });
}

PairPtr add(PairPool& pool, Pair const& lhs, Pair const& rhs)
{
    return pool.createPairPtr(Pair{ .first = clone(pool, lhs), .second = clone(pool, rhs) });
}

bool isNumber(Element const& e)
{
    return e.index() == 0;
}

bool isPair(Element const& e)
{
    return e.index() == 1;
}

enum class Traversal {
    Continue,
    Stop
};

template<typename Func>
void traverse(Pair& start_p, Func const& func)
{
    std::vector<Pair*> path;
    enum VisitState {
        New,
        LeftDone,
        FuncDone,
        RightDone
    };
    std::vector<VisitState> was_visited;
    path.push_back(&start_p);
    was_visited.push_back(VisitState::New);
    while (!path.empty()) {
        Pair& p = *path.back();
        VisitState& visited = was_visited.back();
        if (visited == VisitState::New) {
            visited = VisitState::LeftDone;
            if (isPair(p.first)) {
                path.push_back(std::get<PairPtr>(p.first).get());
                was_visited.push_back(VisitState::New);
            }
        } else if (visited == VisitState::LeftDone) {
            if (func(path) == Traversal::Stop) { return; }
            visited = VisitState::FuncDone;
        } else if (visited == VisitState::FuncDone) {
            visited = VisitState::RightDone;
            if (isPair(p.second)) {
                path.push_back(std::get<PairPtr>(p.second).get());
                was_visited.push_back(VisitState::New);
            }
        } else {
            assert(visited == VisitState::RightDone);
            path.pop_back();
            was_visited.pop_back();
        }
        
    }
}

void doExplode(Pair const& p, Pair& parent, Number* left_neighbour, Number* right_neighbour)
{
    assert(isNumber(p.first) && isNumber(p.second));
    if (left_neighbour) {
        left_neighbour->n += std::get<Number>(p.first).n;
    }
    if (right_neighbour) {
        right_neighbour->n += std::get<Number>(p.second).n;
    }
    if (Pair* pp = (isPair(parent.first)) ? (std::get<PairPtr>(parent.first).get()) : nullptr; pp == &p) {
        parent.first = Number{ .n = 0 };
    } else {
        assert((isPair(parent.second)) && ((std::get<PairPtr>(parent.second).get()) == &p));
        parent.second = Number{ .n = 0 };
    }
}

bool explode(Pair& p)
{
    Number* left_neighbour = nullptr;
    Pair* to_explode = nullptr;
    Pair* to_explode_parent = nullptr;
    Number* right_neighbour = nullptr;
    traverse(p, [&](std::vector<Pair*> path) -> Traversal {
        Pair& p = *path.back();
        if (!to_explode) {
            std::size_t const depth = path.size();
            if ((depth > 4) && isNumber(p.first) && isNumber(p.second)) {
                to_explode = path[path.size() - 1];
                to_explode_parent = path[path.size() - 2];
            } else {
                if (isNumber(p.second)) {
                    left_neighbour = &std::get<Number>(p.second);
                } else if (isNumber(p.first)) {
                    left_neighbour = &std::get<Number>(p.first);
                }
            }
        } else {
            if (isNumber(p.first)) {
                right_neighbour = &std::get<Number>(p.first);
                return Traversal::Stop;
            } else if (isNumber(p.second)) {
                right_neighbour = &std::get<Number>(p.second);
                return Traversal::Stop;
            }
        }
        return Traversal::Continue;
    });
    if (to_explode) {
        assert(to_explode_parent);
        doExplode(*to_explode, *to_explode_parent, left_neighbour, right_neighbour);
        return true;
    }
    return false;
}

bool split(PairPool& pool, Pair& p)
{
    bool did_split = false;
    traverse(p, [&did_split, &pool](std::vector<Pair*> path) -> Traversal {
        Pair& p = *path.back();
        if (isNumber(p.first)) {
            Number& n = std::get<Number>(p.first);
            if (n.n >= 10) {
                p.first = pool.createPairPtr(Pair{ .first = Number{ .n = n.n / 2 }, .second = Number{ .n = (n.n + 1) / 2 }});
                did_split = true;
                return Traversal::Stop;
            }
        }
        if (isNumber(p.second)) {
            Number& n = std::get<Number>(p.second);
            if (n.n >= 10) {
                p.second = pool.createPairPtr(Pair{ .first = Number{ .n = n.n / 2 }, .second = Number{ .n = (n.n + 1) / 2 } });
                did_split = true;
                return Traversal::Stop;
            }
        }
        return Traversal::Continue;
    });
    return did_split;
}

bool reduce_step(PairPool& pool, Pair& p)
{
    return explode(p) || split(pool, p);
}

void reduce(PairPool& pool, Pair& p)
{
    while (reduce_step(pool, p)) 
        ;
}

PairPtr addReduce(PairPool& pool, std::vector<PairPtr> const& pairs)
{
    assert(pairs.size() >= 2);
    PairPtr acc = clone(pool, *pairs.front());
    for (auto it = begin(pairs) + 1, it_end = end(pairs); it != it_end; ++it) {
        acc = add(pool, *acc, **it);
        reduce(pool, *acc);
    }
    return acc;
}

int64_t magnitude(Pair const& p)
{
    struct MagnitudeVisitor {
        int64_t operator()(Number const& n) {
            return n.n;
        }
        int64_t operator()(PairPtr const& p) {
            return (*this)(*p);
        }
        int64_t operator()(Pair const& p) {
            int64_t const f = std::visit(MagnitudeVisitor{}, p.first);
            int64_t const s = std::visit(MagnitudeVisitor{}, p.second);
            return 3*f + 2*s;
        }
    };
    return MagnitudeVisitor{}(p);
}

int64_t result1(PairPool& pool, std::vector<PairPtr> const& pairs)
{
    return magnitude(*addReduce(pool, pairs));;

}

int64_t result2(PairPool& pool, std::vector<PairPtr> const& pairs)
{
    auto rng_indices = ranges::views::iota(std::size_t{0}, pairs.size());
    return ranges::max(
        ranges::views::cartesian_product(rng_indices, rng_indices)
        | ranges::views::filter([](auto const& i12) { auto const [i1, i2] = i12; return i1 != i2; })
        | ranges::views::transform([&](auto const& i12) -> int64_t {
                auto const [i1, i2] = i12;
                Pair const& p1 = *pairs[i1];
                Pair const& p2 = *pairs[i2];
                PairPtr sum = add(pool, p1, p2);
                reduce(pool, *sum);
                return magnitude(*sum);
            })
    );
}
