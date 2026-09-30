#include <TestExplorer/TestRunner.hpp>

#include <TestExplorer/CurrentTestContext.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestContext.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/TestReporter.hpp>

#include <chrono>
#include <vector>

namespace testexplorer
{
    TestRunner::TestRunner(TestReporter *reporter) : m_reporter(reporter) {}

    TestResult TestRunner::run(
        const TestCase &test)
    {
        if (m_reporter != nullptr)
        {
            m_reporter->testStarted(test);
        }

        TestContext context;

        CurrentTestContext::set(context);

        const auto start = std::chrono::steady_clock::now();

        test.execute(context);

        const auto end = std::chrono::steady_clock::now();

        CurrentTestContext::clear();

        const TestStatus status =
            context.failures().empty()
                ? TestStatus::Passed
                : TestStatus::Failed;

        const auto duration =
            std::chrono::duration_cast<TestResult::Duration>(
                end - start);

        TestResult result(
            test.id(),
            test.name(),
            status,
            duration,
            context.failures());

        if (m_reporter != nullptr)
        {
            m_reporter->testFinished(result);
        }

        return result;
    }

    std::vector<TestResult> TestRunner::runAll(
        const TestRegistry &registry)
    {
        std::vector<TestResult> results;

        for (const TestCase &test : registry.tests())
        {
            results.push_back(run(test));
        }

        if (m_reporter != nullptr)
        {
            m_reporter->testRunFinished(results);
        }

        return results;
    }
}