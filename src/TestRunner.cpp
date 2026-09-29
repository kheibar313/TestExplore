#include <TestExplorer/TestRunner.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestContext.hpp>
#include <TestExplorer/TestRegistry.hpp>

#include <chrono>
#include <vector>

namespace testexplorer
{
    TestResult TestRunner::run(const TestCase &test)
    {
        TestContext context;

        const auto start = std::chrono::steady_clock::now();

        test.execute(context);

        const auto end = std::chrono::steady_clock::now();

        const TestStatus status =
            context.failures().empty()
                ? TestStatus::Passed
                : TestStatus::Failed;

        const auto duration =
            std::chrono::duration_cast<TestResult::Duration>(
                end - start);

        return TestResult(
            test.id(),
            test.name(),
            status,
            duration,
            context.failures());
    }

    std::vector<TestResult> TestRunner::runAll(
        const TestRegistry &registry)
    {
        std::vector<TestResult> results;

        for (const TestCase &test : registry.tests())
        {
            results.push_back(run(test));
        }

        return results;
    }
}