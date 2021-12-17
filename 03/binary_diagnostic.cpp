#include <binary_diagnostic.hpp>

#include <range/v3/range/conversion.hpp>
#include <range/v3/view/split.hpp>
#include <range/v3/view/transform.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <string>

std::tuple<std::vector<ReportEntry>, int64_t> parseInput(std::string_view input)
{
    std::optional<int64_t> entry_length;
    auto const ret = ranges::to<std::vector<ReportEntry>>(input | ranges::views::split('\n') |
        ranges::views::transform([&entry_length](auto sv) {
                assert(!entry_length || *entry_length == ranges::distance(sv));
                entry_length = ranges::distance(sv);
                return ReportEntry(ranges::to<std::string>(sv));
            }));
    assert(entry_length);
    return std::make_tuple(ret, *entry_length);
}

std::array<int, 32> pincountInPosition(std::vector<ReportEntry> const& report)
{
    std::array<int, 32> acc{};
    for (auto const& e : report) {
        for (int i = 0; i < static_cast<int>(acc.size()); ++i) {
            acc[i] += (e[i] ? 1 : 0);
        }
    }
    return acc;
}


ReportEntry gammaBits(std::vector<ReportEntry> const& report)
{
    std::array<int, 32> acc = pincountInPosition(report);
    ReportEntry ret{};
    for (int i = 0; i < static_cast<int>(acc.size()); ++i) {
        if (acc[i] > static_cast<int>(report.size()) / 2) {
            ret.set(i);
        }
    }
    return ret;
}

uint64_t result1(std::vector<ReportEntry> const& report, int64_t entry_length)
{
    auto const gamma = gammaBits(report);
    auto epsilon = ~gamma;
    for (int64_t i = entry_length; i < static_cast<int64_t>(epsilon.size()); ++i) { epsilon.reset(i); }
    return gamma.to_ullong() * epsilon.to_ullong();
}

void filterO2Candidate(std::vector<ReportEntry>& candidates, int64_t position)
{
    if (candidates.size() < 2) { return; }
    auto const acc = pincountInPosition(candidates);
    int const one_bits = acc[position];
    int const zero_bits = static_cast<int>(candidates.size()) - one_bits;
    if (one_bits >= zero_bits) {
        // keep 1s
        erase_if(candidates, [position](ReportEntry const& e) -> bool { return !(e[position]); });
    } else {
        // keep 0s
        erase_if(candidates, [position](ReportEntry const& e) -> bool { return e[position]; });
    }
}

ReportEntry getO2Rating(std::vector<ReportEntry> const& report, int64_t entry_length)
{
    auto candidates = report;
    for (int64_t i = entry_length; i != 0; --i) {
        filterO2Candidate(candidates, i - 1);
    }
    assert(candidates.size() == 1);
    return candidates.front();
}

void filterCO2Candidate(std::vector<ReportEntry>& candidates, int64_t position)
{
    if (candidates.size() < 2) { return; }
    auto const acc = pincountInPosition(candidates);
    int const one_bits = acc[position];
    int const zero_bits = static_cast<int>(candidates.size()) - one_bits;
    if (one_bits >= zero_bits) {
        // keep 0s
        erase_if(candidates, [position](ReportEntry const& e) -> bool { return e[position]; });
    } else {
        // keep 1s
        erase_if(candidates, [position](ReportEntry const& e) -> bool { return !(e[position]); });
    }
}

ReportEntry getCO2Rating(std::vector<ReportEntry> const& report, int64_t entry_length)
{
    auto candidates = report;
    for (int64_t i = entry_length; i != 0; --i) {
        filterCO2Candidate(candidates, i - 1);
    }
    assert(candidates.size() == 1);
    return candidates.front();
}


uint64_t result2(std::vector<ReportEntry> const& report, int64_t entry_length)
{
    auto const o2_rating = getO2Rating(report, entry_length);
    auto const co2_rating = getCO2Rating(report, entry_length);
    return o2_rating.to_ullong() * co2_rating.to_ullong();
}

