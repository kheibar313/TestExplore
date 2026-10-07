#ifndef TEST_ASSERTIONS_HPP
#define TEST_ASSERTIONS_HPP

#include <TestExplorer/TestResult.hpp>

#include <vector>

bool verifyAssertionResults(
    const std::vector<testexplorer::TestResult> &results);

bool verifyAssertionFailures(
    const std::vector<testexplorer::TestResult> &results);

#endif