#ifndef ADVENT_OF_CODE_18_SNAILFISH_INCLUDE_GUARD
#define ADVENT_OF_CODE_18_SNAILFISH_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <memory>
#include <memory_resource>
#include <ostream>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

struct Number {
    int64_t n = -1;
};

struct Pair;

struct PairPoolDeleter {
    std::pmr::memory_resource* pmr;

    PairPoolDeleter(std::pmr::memory_resource* ppmr) : pmr(ppmr) {}

    void operator()(Pair* p) const;
};

using PairPtr = std::unique_ptr<Pair, PairPoolDeleter>;

using Element = std::variant<Number, PairPtr>;

struct Pair {
    Element first;
    Element second;
};

template<>
struct fmt::formatter<Pair>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Pair const& p, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "[{},{}]", p.first, p.second);
    }
};

template<>
struct fmt::formatter<Element>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Element const& e, FormatContext& ctx) {
        struct FormatVisitor {
            FormatContext* m_ctx;

            FormatVisitor(FormatContext& ctx) : m_ctx(&ctx) {}

            auto operator()(Number const& n) {
                return fmt::format_to(m_ctx->out(), "{}", n.n);
            }
            auto operator()(PairPtr const& p) {
                return fmt::format_to(m_ctx->out(), "{}", *p);
            }
        };
        return std::visit(FormatVisitor{ ctx }, e);
    }
};

inline void PairPoolDeleter::operator()(Pair* p) const {
    p->~Pair();
    pmr->deallocate(p, sizeof(Pair));
}

class PairPool {
private:
    std::pmr::monotonic_buffer_resource m_alloc;
    std::pmr::pool_options pool_opts;
    std::pmr::unsynchronized_pool_resource m_pool;
public:

    PairPool()
        :m_alloc(), m_pool(std::pmr::pool_options{ .max_blocks_per_chunk = 0, .largest_required_pool_block = sizeof(Pair) }, & m_alloc)
    {}

    PairPool(PairPool const&) = delete;
    PairPool& operator=(PairPool const&) = delete;

    operator std::pmr::memory_resource* () { return &m_pool; }

    PairPtr createPairPtr(Pair&& p) {
        void* memptr = m_pool.allocate(sizeof(Pair));
        Pair* ptr = new (memptr) Pair(std::move(p));
        PairPtr ret(ptr, PairPoolDeleter{ *this });
        return ret;
    }
};

Pair parseLine(PairPool& pool, std::string_view line);

std::vector<PairPtr> parseInput(PairPool& pool, std::string_view input);

int64_t getNumber(Element const& e);

Pair* getPair(Element const& e);

PairPtr clone(PairPool& pool, Pair const& p);

PairPtr add(PairPool& pool, Pair const& lhs, Pair const& rhs);

bool explode(Pair& p);

bool split(PairPool& pool, Pair& p);

bool reduce_step(PairPool& pool, Pair& p);

void reduce(PairPool& pool, Pair& p);

PairPtr addReduce(PairPool& pool, std::vector<PairPtr> const& pairs);

int64_t magnitude(Pair const& p);

int64_t result1(PairPool& pool, std::vector<PairPtr> const& pairs);

int64_t result2(PairPool& pool, std::vector<PairPtr> const& pairs);

#endif
