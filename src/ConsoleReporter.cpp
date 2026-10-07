#include <TestExplorer/ConsoleReporter.hpp>

#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestResult.hpp>

#include <chrono>
#include <iostream>

namespace testexplorer
{
    namespace
    {
        const char *statusToString(TestStatus status)
        {
            switch (status)
            {
            case TestStatus::Passed:
                return "PASSED";

            case TestStatus::Failed:
                return "FAILED";

            case TestStatus::Skipped:
                return "SKIPPED";

            case TestStatus::Error:
                return "ERROR";
            }

            return "UNKNOWN";
        }
    }

    ConsoleReporter::ConsoleReporter(
        std::ostream &output)
        : m_output(output)
    {
    }

    void ConsoleReporter::testStarted(
        const TestCase &test)
    {
        // Intentionally empty for now.
        // The console reporter prints the result after execution.
        (void)test;
    }

    void ConsoleReporter::testFinished(
        const TestResult &result)
    {
        const auto duration =
            std::chrono::duration_cast<std::chrono::nanoseconds>(
                result.duration());

        m_output
            << result.testName()
            << " ["
            << result.testId()
            << "]: "
            << statusToString(result.status())
            << " | "
            << duration.count()
            << " ns\n";

        for (const TestFailure &failure : result.failures())
        {
            m_output
                << "  Message: "
                << failure.message()
                << '\n';

            m_output
                << "  Location: "
                << failure.location().file_name()
                << ':'
                << failure.location().line()
                << '\n';
        }

        if (result.status() == TestStatus::Error)
        {
            m_output
                << "  Error: "
                << result.errorMessage()
                << '\n';
        }
    }

    void ConsoleReporter::testRunFinished(
        const std::vector<TestResult> &results)
    {
        m_output
            << "\n=== Test Run Finished ===\n"
            << "Tests: "
            << results.size()
            << '\n';
    }
}