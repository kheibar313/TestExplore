#include "test_assertions.hpp"

#include <TestExplorer/TestResult.hpp>

#include <string>
#include <vector>
#include <iostream>

using namespace testexplorer;

namespace
{
    const testexplorer::TestResult *findResult(
        const std::vector<testexplorer::TestResult> &results,
        const std::string &testId)
    {
        for (const testexplorer::TestResult &result : results)
        {
            if (result.testId() == testId)
            {
                return &result;
            }
        }

        return nullptr;
    }
}

bool verifyAssertionResults(
    const std::vector<testexplorer::TestResult> &results)
{
    if (results.size() != 7)
    {
        std::cerr
            << "Self-test error: expected 7 results, got "
            << results.size()
            << '\n';

        return false;
    }

    struct ExpectedResult
    {
        const char *testId;
        testexplorer::TestStatus status;
    };

    const ExpectedResult expectedResults[] =
        {
            {"assertions.passing",
             testexplorer::TestStatus::Passed},
            {"assertions.failing",
             testexplorer::TestStatus::Failed},
            {"assertions.strings",
             testexplorer::TestStatus::Passed},
            {"assertions.multiple_failures",
             testexplorer::TestStatus::Failed},
            {"math.addition",
             testexplorer::TestStatus::Passed},
            {"math.subtraction",
             testexplorer::TestStatus::Passed},
            {"string.compare",
             testexplorer::TestStatus::Passed}};

    for (const ExpectedResult &expected : expectedResults)
    {
        const auto *result =
            findResult(
                results,
                expected.testId);

        if (result == nullptr)
        {
            std::cerr
                << "Self-test error: expected test result was not found: "
                << expected.testId
                << '\n';

            return false;
        }

        if (result->status() != expected.status)
        {
            std::cerr
                << "Self-test error: unexpected status for test: "
                << expected.testId
                << '\n';

            return false;
        }
    }

    return true;
}

bool verifyAssertionFailures(
    const std::vector<testexplorer::TestResult> &results)
{
    const auto *failingAssertions =
        findResult(
            results,
            "assertions.failing");

    if (failingAssertions == nullptr)
    {
        std::cerr
            << "Self-test error: Failing Assertions result was not found.\n";

        return false;
    }

    if (failingAssertions->failures().size() != 4)
    {
        std::cerr
            << "Self-test error: expected 4 failures in "
               "Failing Assertions, got "
            << failingAssertions->failures().size()
            << '\n';

        return false;
    }

    const auto *multipleFailures =
        findResult(
            results,
            "assertions.multiple_failures");

    if (multipleFailures == nullptr)
    {
        std::cerr
            << "Self-test error: Multiple Failures result was not found.\n";

        return false;
    }

    if (multipleFailures->failures().size() != 3)
    {
        std::cerr
            << "Self-test error: expected 3 failures in "
               "Multiple Failures, got "
            << multipleFailures->failures().size()
            << '\n';

        return false;
    }

    return true;
}