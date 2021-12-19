#ifndef ADVENT_OF_CODE_19_BEACON_SCANNER_INCLUDE_GUARD
#define ADVENT_OF_CODE_19_BEACON_SCANNER_INCLUDE_GUARD

#include <fmt/format.h>

#include <cstdint>
#include <memory>
#include <memory_resource>
#include <ostream>
#include <optional>
#include <string_view>
#include <variant>
#include <vector>

struct Vector3 {
    int x;
    int y;
    int z;

    friend bool operator==(Vector3 const&, Vector3 const&) = default;

    friend inline constexpr Vector3 operator+(Vector3 const& lhs, Vector3 const& rhs) noexcept
    {
        return Vector3{ lhs.x + rhs.x,
                        lhs.y + rhs.y,
                        lhs.z + rhs.z };
    }

    friend inline constexpr Vector3 operator-(Vector3 const& lhs, Vector3 const& rhs) noexcept
    {
        return Vector3{ lhs.x - rhs.x,
                        lhs.y - rhs.y,
                        lhs.z - rhs.z };
    }
};

template <class T>
inline void hash_combine(std::size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

struct HashVector3 {
    std::size_t operator()(Vector3 const& v) const {
        std::size_t seed = 0;
        hash_combine(seed, v.x);
        hash_combine(seed, v.y);
        hash_combine(seed, v.z);
        return seed;
    }
};


template<>
struct fmt::formatter<Vector3>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& ctx) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(Vector3 const& v, FormatContext& ctx) {
        return fmt::format_to(ctx.out(), "{{ {}, {}, {} }}", v.x, v.y, v.z);
    }
};

inline std::ostream& operator<<(std::ostream& os, Vector3 const& v) {
    return os << fmt::format("{}", v);
}

struct Scan {
    Vector3 scanOffset;
    int transformationIndex;
    std::vector<Vector3> beacons;
};

std::vector<Scan> parseInput(std::string_view input);

bool isInRange(Vector3 const& scanner, Vector3 const& beacon);

bool findOffset(Scan const& ref, Scan& candidate, int match_limit);

Scan transformScan(Scan const& s, int transformation_index);

bool matchScans(Scan const& ref, Scan& candidate);

std::vector<Scan> matchAllScans(std::vector<Scan> scans);

int64_t countBeacons(std::vector<Scan> const& matched_scans);

int64_t largestDistance(std::vector<Scan> const& matched_scans);

#endif
