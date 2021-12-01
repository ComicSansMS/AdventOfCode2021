#include <sonar_sweep.hpp>

#include <catch.hpp>

TEST_CASE("Sonar Sweep")
{
    char const sample_input[] = "199\n200\n208\n210\n200\n207\n240\n269\n260\n263\n";
    SECTION("Parse Input")
    {
        CHECK(parseInput("1\n2\n42") == std::vector<int>{1, 2, 42});
        CHECK(parseInput(sample_input) == std::vector<int>{199, 200, 208, 210, 200, 207, 240, 269, 260, 263});
    }

    std::vector<int> const values = parseInput(sample_input);

    SECTION("Count Depth Increase")
    {
        CHECK(countDepthIncrease({ 1, 2 }) == 1);
        CHECK(countDepthIncrease({ 1, 1 }) == 0);
        CHECK(countDepthIncrease({ 2, 1 }) == 0);
        CHECK(countDepthIncrease(values) == 7);
    }

    SECTION("Three Measurment")
    {
        CHECK(threeMeasurement({ 1, 2, 3, 4 }) == 1);
        CHECK(threeMeasurement({ 2, 3, 1, 4 }) == 1);
        CHECK(threeMeasurement({ 3, 2, 1, 4 }) == 1);
        CHECK(threeMeasurement({ 3, 1, 2, 4 }) == 1);
        CHECK(threeMeasurement({ 1, 2, 3, 1 }) == 0);
        CHECK(threeMeasurement({ 2, 3, 1, 1 }) == 0);
        CHECK(threeMeasurement({ 3, 2, 1, 1 }) == 0);
        CHECK(threeMeasurement({ 3, 1, 2, 1 }) == 0);
        CHECK(threeMeasurement({ 1, 2, 3, 0 }) == 0);
        CHECK(threeMeasurement({ 2, 3, 1, 0 }) == 0);
        CHECK(threeMeasurement({ 3, 2, 1, 0 }) == 0);
        CHECK(threeMeasurement({ 3, 1, 2, 0 }) == 0);
        CHECK(threeMeasurement(values) == 5);
    }
}
