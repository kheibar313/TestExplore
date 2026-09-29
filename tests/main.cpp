#include <TestExplorer/Assertions.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestContext.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/TestRunner.hpp>
#include <iostream>

int main()
{
    using namespace testexplorer;

    TestCase passedTest(
        "test.passed",
        "Passing Test",
        [](TestContext &context)
        {
            expectTrue(context, true);
            expectFalse(context, false);
            expectEqual(context, 10, 10);
            expectNotEqual(context, 10, 20);
        });

    TestCase failedTest(
        "test.failed",
        "Failing Test",
        [](TestContext &context)
        {
            expectTrue(context, false);
            expectEqual(context, 10, 20);
        });

    TestRegistry registry;

    registry.registerTest(passedTest);
    registry.registerTest(failedTest);

    TestRunner runner;

    const std::vector<TestResult> results =
        runner.runAll(registry);

    std::cout << "=== Test Results ===\n\n";

    for (const TestResult &result : results)
    {
        std::cout
            << result.testName()
            << " ["
            << result.testId()
            << "]: ";

        if (result.status() == TestStatus::Passed)
        {
            std::cout << "PASSED";
        }
        else
        {
            std::cout << "FAILED";
        }

        std::cout
            << " | "
            << result.duration().count()
            << " ns\n";

        if (!result.failures().empty())
        {
            std::cout
                << "  Failures: "
                << result.failures().size()
                << '\n';
        }
    }

    return 0;
}