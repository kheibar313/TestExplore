#include <TestExplorer/Assertions.hpp>
#include <TestExplorer/ConsoleReporter.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/TestRunner.hpp>

#include <string>
#include <iostream>
#include <stdexcept>

using namespace testexplorer;

namespace
{
    void registerTests(TestRegistry &registry)
    {
        // ------------------------------------------------------------
        // Assertion tests
        // ------------------------------------------------------------

        registry.registerTest(
            TestCase(
                "assertions.passing",
                "Passing Assertions",
                [](TestContext &)
                {
                    EXPECT_TRUE(true);
                    EXPECT_FALSE(false);

                    EXPECT_EQ(10, 10);
                    EXPECT_NE(10, 20);
                }));

        registry.registerTest(
            TestCase(
                "assertions.failing",
                "Failing Assertions",
                [](TestContext &)
                {
                    EXPECT_TRUE(false);
                    EXPECT_FALSE(true);

                    EXPECT_EQ(10, 20);
                    EXPECT_NE(10, 10);
                }));

        registry.registerTest(
            TestCase(
                "assertions.strings",
                "String Assertions",
                [](TestContext &)
                {
                    EXPECT_EQ(
                        std::string("hello"),
                        std::string("hello"));

                    EXPECT_NE(
                        std::string("hello"),
                        std::string("world"));
                }));

        registry.registerTest(
            TestCase(
                "assertions.multiple_failures",
                "Multiple Failures",
                [](TestContext &)
                {
                    EXPECT_EQ(1, 2);
                    EXPECT_EQ(3, 4);
                    EXPECT_TRUE(false);
                }));

        // ------------------------------------------------------------
        // Filtering tests
        // ------------------------------------------------------------

        registry.registerTest(
            TestCase(
                "math.addition",
                "Math Addition",
                [](TestContext &)
                {
                    EXPECT_EQ(2 + 2, 4);
                }));

        registry.registerTest(
            TestCase(
                "math.subtraction",
                "Math Subtraction",
                [](TestContext &)
                {
                    EXPECT_EQ(5 - 3, 2);
                }));

        registry.registerTest(
            TestCase(
                "string.compare",
                "String Compare",
                [](TestContext &)
                {
                    EXPECT_EQ(
                        std::string("hello"),
                        std::string("hello"));
                }));
    }

    bool verifyBasicResults(
        const std::vector<TestResult> &results)
    {
        if (results.size() != 7)
        {
            std::cerr
                << "Self-test error: expected 7 results, got "
                << results.size()
                << '\n';

            return false;
        }

        // Verify expected test statuses.
        const TestStatus expectedStatuses[] =
            {
                TestStatus::Passed,
                TestStatus::Failed,
                TestStatus::Passed,
                TestStatus::Failed,
                TestStatus::Passed,
                TestStatus::Passed,
                TestStatus::Passed};

        for (std::size_t i = 0; i < results.size(); ++i)
        {
            if (results[i].status() != expectedStatuses[i])
            {
                std::cerr
                    << "Self-test error: unexpected status for test #"
                    << i
                    << ".\n";

                return false;
            }
        }

        return true;
    }

    bool verifyFailures(
        const std::vector<TestResult> &results)
    {
        if (results[1].failures().size() != 4)
        {
            std::cerr
                << "Self-test error: expected 4 failures in "
                   "Failing Assertions, got "
                << results[1].failures().size()
                << '\n';

            return false;
        }

        if (results[3].failures().size() != 3)
        {
            std::cerr
                << "Self-test error: expected 3 failures in "
                   "Multiple Failures, got "
                << results[3].failures().size()
                << '\n';

            return false;
        }

        return true;
    }

    bool verifyMathFilter(
        const std::vector<TestResult> &results)
    {
        if (results.size() != 2)
        {
            std::cerr
                << "Self-test error: expected 2 filtered results, got "
                << results.size()
                << '\n';

            return false;
        }

        if (results[0].testId() != "math.addition")
        {
            std::cerr
                << "Self-test error: first filtered test is incorrect.\n";

            return false;
        }

        if (results[1].testId() != "math.subtraction")
        {
            std::cerr
                << "Self-test error: second filtered test is incorrect.\n";

            return false;
        }

        if (results[0].status() != TestStatus::Passed ||
            results[1].status() != TestStatus::Passed)
        {
            std::cerr
                << "Self-test error: filtered tests should pass.\n";

            return false;
        }

        return true;
    }

    bool verifyEmptyFilter(
        const std::vector<TestResult> &results)
    {
        if (!results.empty())
        {
            std::cerr
                << "Self-test error: expected no tests to match filter, got "
                << results.size()
                << '\n';

            return false;
        }

        return true;
    }

    bool verifyAllFilter(
        const std::vector<TestResult> &results)
    {
        if (results.size() != 7)
        {
            std::cerr
                << "Self-test error: expected all 7 tests to match filter, got "
                << results.size()
                << '\n';

            return false;
        }

        return true;
    }

    bool verifyRunSingle(
        const TestResult &result)
    {
        return result.testId() == "math.addition" &&
               result.testName() == "Math Addition" &&
               result.status() == TestStatus::Passed;
    }

    bool verifyRunFailed(
        const std::vector<TestResult> &results)
    {
        if (results.size() != 2)
        {
            std::cerr
                << "Self-test error: expected 2 failed tests to be rerun, got "
                << results.size()
                << '\n';

            return false;
        }

        if (results[0].testId() != "assertions.failing")
        {
            std::cerr
                << "Self-test error: first failed test is incorrect.\n";

            return false;
        }

        if (results[1].testId() != "assertions.multiple_failures")
        {
            std::cerr
                << "Self-test error: second failed test is incorrect.\n";

            return false;
        }

        if (results[0].status() != TestStatus::Failed ||
            results[1].status() != TestStatus::Failed)
        {
            std::cerr
                << "Self-test error: rerun failed tests should still fail.\n";

            return false;
        }

        return true;
    }
} // namespace

int main()
{
    // Test registration
    TestRegistry registry;

    registerTests(registry);

    // Test runner and reporter
    ConsoleReporter reporter(std::cout);
    TestRunner runner(&reporter);

    // Run all tests
    const auto results = runner.runAll(registry);

    // Verify complete test run
    if (!verifyBasicResults(results))
    {
        return 1;
    }

    if (!verifyFailures(results))
    {
        return 1;
    }

    // Run tests matching the "math." filter
    const auto mathResults = runner.runAll(
        registry,
        [](const TestCase &test)
        {
            return test.id().starts_with("math.");
        });

    if (!verifyMathFilter(mathResults))
    {
        return 1;
    }

    // Run an intentionally empty filter
    const auto emptyResults = runner.runAll(
        registry,
        [](const TestCase &test)
        {
            return test.id().starts_with("database.");
        });

    if (!verifyEmptyFilter(emptyResults))
    {
        return 1;
    }

    // Run a filter that selects every test
    const auto allResults = runner.runAll(
        registry,
        [](const TestCase &)
        {
            return true;
        });

    if (!verifyAllFilter(allResults))
    {
        return 1;
    }

    // Run a single test
    const TestResult singleResult =
        runner.run(
            registry,
            "math.addition");

    if (!verifyRunSingle(singleResult))
    {
        return 1;
    }

    // Verify invalid test ID handling
    try
    {
        runner.run(
            registry,
            "does.not.exist");

        return 1;
    }
    catch (const std::invalid_argument &)
    {
    }

    // Rerun previously failed tests
    const auto failedResults =
        runner.runFailed(
            registry,
            results);

    if (!verifyRunFailed(failedResults))
    {
        return 1;
    }

    // Run failed tests when there are no failures
    const auto noFailedResults =
        runner.runFailed(
            registry,
            mathResults);

    if (!noFailedResults.empty())
    {
        std::cerr
            << "Self-test error: expected no failed tests to rerun.\n";

        return 1;
    }

    // Self-test summary
    std::cout
        << "\n=== Framework Self-Tests Passed ===\n";

    return 0;
}