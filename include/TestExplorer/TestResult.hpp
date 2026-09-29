#ifndef TESTRESULT
#define TESTRESULT

#include <TestExplorer/TestFailure.hpp>

#include <chrono>
#include <string>
#include <vector>

namespace testexplorer
{
    enum class TestStatus
    {
        Passed,
        Failed,
        Skipped,
        Error
    };

    class TestResult
    {
    public:
        using Duration = std::chrono::nanoseconds;

        TestResult(
            std::string testId,
            std::string testName,
            TestStatus status,
            Duration duration,
            std::vector<TestFailure> failures);

        const std::string &testId() const;
        const std::string &testName() const;

        TestStatus status() const;
        Duration duration() const;

        const std::vector<TestFailure> &failures() const;

    private:
        std::string m_testId;
        std::string m_testName;

        TestStatus m_status;
        Duration m_duration;

        std::vector<TestFailure> m_failures;
    };
}

#endif