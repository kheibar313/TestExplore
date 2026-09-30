#include <TestExplorer/Assertions.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestContext.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/TestRunner.hpp>
#include <iostream>
#include <string>

int main()
{
    using namespace testexplorer;

    TestCase passedTest(
        "test.passed",
        "Passing Test",
        [](TestContext &context)
        {
            EXPECT_TRUE(true);
            EXPECT_FALSE(false);
            EXPECT_EQ(10, 10);
            EXPECT_NE(10, 20);
        });

    TestCase failedTest(
        "test.failed",
        "Failing Test",
        [](TestContext &context)
        {
            EXPECT_EQ(
                std::string("hello"),
                std::string("world"));
            EXPECT_TRUE(false);
            EXPECT_EQ(10, 20);
            EXPECT_NE(10, 10);
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

            for (const TestFailure &failure : result.failures())
            {
                std::cout
                    << "  Message: "
                    << failure.message()
                    << '\n';

                std::cout
                    << "  Location: "
                    << failure.location().file_name()
                    << ':'
                    << failure.location().line()
                    << '\n';
            }
        }
    }

    return 0;
}