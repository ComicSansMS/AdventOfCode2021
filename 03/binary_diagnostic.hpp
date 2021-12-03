#ifndef ADVENT_OF_CODE_03_BINARY_DIAGNOSTIC_INCLUDE_GUARD
#define ADVENT_OF_CODE_03_BINARY_DIAGNOSTIC_INCLUDE_GUARD

#include <array>
#include <bitset>
#include <cstdint>
#include <string_view>
#include <tuple>
#include <vector>

using ReportEntry = std::bitset<32>;


std::tuple<std::vector<ReportEntry>, int64_t> parseInput(std::string_view input);

ReportEntry gammaBits(std::vector<ReportEntry> const& report);
uint64_t result1(std::vector<ReportEntry> const& report, int64_t entry_length);


std::array<int, 32> pincountInPosition(std::vector<ReportEntry> const& report);

void filterO2Candidate(std::vector<ReportEntry>& candidates, int64_t position);
ReportEntry getO2Rating(std::vector<ReportEntry> const& report, int64_t entry_length);
void filterCO2Candidate(std::vector<ReportEntry>& candidates, int64_t position);
ReportEntry getCO2Rating(std::vector<ReportEntry> const& report, int64_t entry_length);
uint64_t result2(std::vector<ReportEntry> const& report, int64_t entry_length);

#endif
