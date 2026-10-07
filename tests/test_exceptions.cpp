#include "test_exceptions.hpp"

#include <TestExplorer/TestResult.hpp>
#include <TestExplorer/CurrentTestContext.hpp>

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

using namespace testexplorer;

bool runExceptionTests(
    testexplorer::TestRunner &runner,
    testexplorer::TestRegistry &registry)
{
    // Exception handling
    const auto results =
        runner.runAll(registry);

    if (results.size() != registry.tests().size())
    {
        std::cerr
            << "Self-test error: exception test result count mismatch.\n";

        return false;
    }

    const TestResult *runtimeError = nullptr;
    const TestResult *unknownException = nullptr;
    const TestResult *afterError = nullptr;

    for (const auto &result : results)
    {
        if (result.testId() == "exceptions.runtime_error")
        {
            runtimeError = &result;
        }
        else if (result.testId() == "exceptions.unknown")
        {
            unknownException = &result;
        }
        else if (result.testId() == "exceptions.after_error")
        {
            afterError = &result;
        }
    }

    if (runtimeError == nullptr ||
        unknownException == nullptr ||
        afterError == nullptr)
    {
        std::cerr
            << "Self-test error: required exception test result was not found.\n";

        return false;
    }

    if (runtimeError->status() != TestStatus::Error)
    {
        std::cerr
            << "Self-test error: runtime_error test should be Error.\n";

        return false;
    }

    if (runtimeError->errorMessage() != "runtime error")
    {
        std::cerr
            << "Self-test error: unexpected runtime_error message.\n";

        return false;
    }

    if (unknownException->status() != TestStatus::Error)
    {
        std::cerr
            << "Self-test error: unknown exception test should be Error.\n";

        return false;
    }

    if (unknownException->errorMessage() != "Unknown exception")
    {
        std::cerr
            << "Self-test error: unexpected unknown exception message.\n";

        return false;
    }

    if (afterError->status() != TestStatus::Passed)
    {
        std::cerr
            << "Self-test error: test after exception should still run.\n";

        return false;
    }

    // CurrentTestContext cleanup
    const TestResult cleanupResult =
        runner.run(
            registry,
            "exceptions.runtime_error");

    if (cleanupResult.status() != TestStatus::Error)
    {
        std::cerr
            << "Self-test error: expected exception test to return Error.\n";

        return false;
    }

    try
    {
        CurrentTestContext::get();

        std::cerr
            << "Self-test error: CurrentTestContext was not cleared.\n";

        return false;
    }
    catch (const std::logic_error &)
    {
        // Expected: no active test context should remain.
    }

    return true;
}