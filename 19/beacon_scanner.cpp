#include <beacon_scanner.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/range/primitives.hpp>
#include <range/v3/algorithm/count_if.hpp>
#include <range/v3/algorithm/find.hpp>
#include <range/v3/algorithm/find_if.hpp>
#include <range/v3/view/cartesian_product.hpp>
#include <range/v3/view/iota.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <optional>
#include <regex>
#include <unordered_map>
#include <unordered_set>

std::vector<Scan> parseInput(std::string_view input)
{
    std::vector<Scan> ret;
    std::regex rx_line(R"(^([\-]?\d+),([\-]?\d+),([\-]?\d+)$)");
    std::regex rx_header(R"(^--- scanner (\d+) ---$)");
    std::smatch smatch;
    for (auto const& rline : input | ranges::views::split('\n')) {
        std::string const line = rline | ranges::to<std::string>;
        if (std::regex_match(line, smatch, rx_header)) {
            int const scanner_no = std::stoi(smatch[1]);
            assert(scanner_no == static_cast<int>(ret.size()));
            ret.emplace_back();
            ret.back().scanOffset = Vector3{ .x = 0, .y = 0, .z = 0 };
            ret.back().transformationIndex = -1;
        } else if (std::regex_match(line, smatch, rx_line)) {
            Vector3 v{ .x = std::stoi(smatch[1]),
                       .y = std::stoi(smatch[2]),
                       .z = std::stoi(smatch[3]) };
            ret.back().beacons.push_back(v);
        } else {
            assert(line.empty());
        }
    }
    return ret;
}

struct Mat3 {
    int m11, m12, m13;
    int m21, m22, m23;
    int m31, m32, m33;

    constexpr Mat3() noexcept = default;
    constexpr Mat3(Mat3 const&) noexcept = default;
    constexpr Mat3& operator=(Mat3 const&) noexcept = default;
    constexpr Mat3(int n11, int n12, int n13,
                   int n21, int n22, int n23,
                   int n31, int n32, int n33) noexcept :
        m11(n11), m12(n12), m13(n13),
        m21(n21), m22(n22), m23(n23),
        m31(n31), m32(n32), m33(n33)
    {}

    friend constexpr bool operator==(Mat3 const& lhs, Mat3 const& rhs) noexcept = default;
};

inline constexpr Mat3 mul(Mat3 const& lhs, Mat3 const& rhs) noexcept {
    return Mat3{ lhs.m11*rhs.m11 + lhs.m12*rhs.m21 + lhs.m13*rhs.m31,
                 lhs.m11*rhs.m12 + lhs.m12*rhs.m22 + lhs.m13*rhs.m32,
                 lhs.m11*rhs.m13 + lhs.m12*rhs.m23 + lhs.m13*rhs.m33,
                 lhs.m21*rhs.m11 + lhs.m22*rhs.m21 + lhs.m23*rhs.m31,
                 lhs.m21*rhs.m12 + lhs.m22*rhs.m22 + lhs.m23*rhs.m32,
                 lhs.m21*rhs.m13 + lhs.m22*rhs.m23 + lhs.m23*rhs.m33,
                 lhs.m31*rhs.m11 + lhs.m32*rhs.m21 + lhs.m33*rhs.m31,
                 lhs.m31*rhs.m12 + lhs.m32*rhs.m22 + lhs.m33*rhs.m32,
                 lhs.m31*rhs.m13 + lhs.m32*rhs.m23 + lhs.m33*rhs.m33 };
}

inline constexpr Vector3 transform(Mat3 const& m, Vector3 const& v) noexcept {
    return Vector3(m.m11*v.x + m.m12*v.y + m.m13*v.z,
                   m.m21*v.x + m.m22*v.y + m.m23*v.z,
                   m.m31*v.x + m.m32*v.y + m.m33*v.z);
}

constexpr std::array<Mat3, 24> transformations()
{
    std::array<Mat3, 24> ret;
    // rot_x 90
    Mat3 dir1(1,  0,  0,
              0,  0, -1,
              0,  1,  0);
    // rot_x 180
    Mat3 dir2(1,  0,  0,
              0, -1,  0,
              0,  0, -1);
    // rot_x 270
    Mat3 dir3(1,  0,  0,
              0,  0,  1,
              0, -1,  0);

    Mat3 rot[6];
    // id
    rot[0] = Mat3(1, 0, 0,
                  0, 1, 0,
                  0, 0, 1 );
    // rot_y 90
    rot[1] = Mat3( 0,  0,  1,
                   0,  1,  0,
                  -1,  0,  0);
    // rot_y 180
    rot[2] = Mat3(-1,  0,  0,
                   0,  1,  0,
                   0,  0, -1);
    // rot_y 270
    rot[3] = Mat3( 0,  0, -1,
                   0,  1,  0,
                   1,  0,  0);
    // rot_z 90
    rot[4] = Mat3( 0, -1,  0,
                   1,  0,  0,
                   0,  0,  1);
    // rot_z 270
    rot[5] = Mat3( 0,  1,  0,
                  -1,  0,  0,
                   0,  0,  1);

    for (int i = 0; i < 6; ++i) {
        ret[i * 4] = rot[i];
        ret[i * 4 + 1] = mul(dir1, rot[i]);
        ret[i * 4 + 2] = mul(dir2, rot[i]);
        ret[i * 4 + 3] = mul(dir3, rot[i]);
    }
    for (int i = 0; i < ret.size(); ++i) {
        for (int j = 0; j < ret.size(); ++j) {
            assert((i == j) || (!(ret[i] == ret[j])));
        }
    }

    return ret;
}

bool isInRange(Vector3 const& scanner, Vector3 const& beacon)
{
    return (std::abs(scanner.x - beacon.x) <= 1000) &&
           (std::abs(scanner.y - beacon.y) <= 1000) &&
           (std::abs(scanner.z - beacon.z) <= 1000);
}

bool findOffset(Scan const& ref, Scan& candidate, int match_limit)
{
    auto const rng_ri = ranges::views::ints(std::size_t{ 0 }, ref.beacons.size());
    auto const rng_ci = ranges::views::ints(std::size_t{ 0 }, candidate.beacons.size());
    for (auto const [ri, ci] : ranges::views::cartesian_product(rng_ri, rng_ci)) {
        Vector3 const offset = ref.beacons[ri] - candidate.beacons[ci];
        int count = 0;
        auto rng_candidate_offset = candidate.beacons | ranges::views::transform([&offset](Vector3 const& v) -> Vector3 { return v + offset; });
        for (Vector3 const& v : rng_candidate_offset) {
            if (ranges::find(ref.beacons, v) != ranges::end(ref.beacons)) { ++count; }
            if (count >= match_limit) { break; }
        }
        assert(count >= 1);
        if (count >= match_limit) {
            candidate.scanOffset = ref.scanOffset + offset;
            return true;
        }
    }
    return false;
}

Scan transformScan(Scan const& s, int transformation_index)
{
    auto constexpr const transforms = transformations();
    assert(s.transformationIndex == -1);
    assert((transformation_index >= 0) && (transformation_index < transforms.size()));
    Mat3 const t = transforms[transformation_index];
    Scan ret = s;
    ret.transformationIndex = transformation_index;
    for (auto& v : ret.beacons) {
        v = transform(t, v);
    }
    return ret;
}

bool matchScans(Scan const& ref, Scan& candidate)
{
    auto constexpr const n_transformations = transformations().size();
    for (int i = 0; i < static_cast<int>(n_transformations); ++i) {
        Scan ts = transformScan(candidate, i);
        ts.scanOffset = Vector3{};
        if (findOffset(ref, ts, 12)) {
            assert(!(ts.scanOffset == Vector3{}));
            candidate = ts;
            return true;
        }
    }
    return false;
}

std::vector<Scan> matchAllScans(std::vector<Scan> scans)
{
    std::vector<Scan> matched_scans;
    matched_scans.push_back(std::move(scans[0]));
    scans.erase(scans.begin());
    while (!scans.empty()) {
        // try to find new match
        bool found_match = false;
        for (auto const& m : matched_scans) {
            for (auto it_c = begin(scans), it_c_end = end(scans); it_c != it_c_end; ++it_c) {
                Scan& c = *it_c;
                if (matchScans(m, c)) {
                    matched_scans.emplace_back(std::move(c));
                    scans.erase(it_c);
                    found_match = true;
                    break;
                }
            }
            if (found_match) { break; }
        }
    }
    return matched_scans;
}

int64_t countBeacons(std::vector<Scan> const& matched_scans)
{
    std::unordered_set<Vector3, HashVector3> beacons;
    for (auto const& s : matched_scans) {
        for (auto const& b : s.beacons) {
            Vector3 beacon = b + s.scanOffset;
            beacons.insert(beacon);
        }
    }
    return static_cast<int64_t>(beacons.size());
}

int64_t largestDistance(std::vector<Scan> const& matched_scans)
{
    auto rng_i = ranges::views::ints(std::size_t{ 0 }, matched_scans.size());
    int64_t max = 0;
    for (auto const [i1, i2] : ranges::views::cartesian_product(rng_i, rng_i)) {
        Vector3 const v1 = matched_scans[i1].scanOffset;
        Vector3 const v2 = matched_scans[i2].scanOffset;
        int64_t const dist = std::abs(v1.x - v2.x) + std::abs(v1.y - v2.y) + std::abs(v1.z - v2.z);
        max = std::max(max, dist);
    }
    return max;
}
