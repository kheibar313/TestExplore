#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/Assertions.hpp>
#include <TestExplorer/TestRunner.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/ConsoleReporter.hpp>
#include <TestExplorer/CurrentTestContext.hpp>

#include <string>
#include <iostream>
#include <stdexcept>

using namespace testexplorer;

namespace
{
    class RecordingReporter final : public TestReporter
    {
    public:
        int startedCount = 0;
        int finishedCount = 0;
        int runFinishedCount = 0;

        std::vector<std::string> startedTestIds;
        std::vector<std::string> finishedTestIds;
        std::vector<std::vector<TestResult>> completedRuns;

        void testStarted(
            const TestCase &test) override
        {
            ++startedCount;
            startedTestIds.push_back(test.id());
        }

        void testFinished(
            const TestResult &result) override
        {
            ++finishedCount;
            finishedTestIds.push_back(result.testId());
        }

        void testRunFinished(
            const std::vector<TestResult> &results) override
        {
            ++runFinishedCount;
            completedRuns.push_back(results);
        }
    };

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

    void registerExceptionTests(TestRegistry &registry)
    {
        registry.registerTest(
            TestCase(
                "exceptions.runtime_error",
                "Runtime Error",
                [](TestContext &)
                {
                    throw std::runtime_error("runtime error");
                }));

        registry.registerTest(
            TestCase(
                "exceptions.unknown",
                "Unknown Exception",
                [](TestContext &)
                {
                    throw 42;
                }));

        registry.registerTest(
            TestCase(
                "exceptions.after_error",
                "Test After Error",
                [](TestContext &)
                {
                    EXPECT_TRUE(true);
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

    bool verifyEmptyRegistry(
        TestRunner &runner)
    {
        TestRegistry emptyRegistry;

        const auto results = runner.runAll(emptyRegistry);

        if (!results.empty())
        {
            std::cerr
                << "Self-test error: expected empty registry to produce no results.\n";

            return false;
        }

        return true;
    }

    bool verifyEmptyPreviousResults(
        TestRunner &runner,
        TestRegistry &registry)
    {
        const std::vector<TestResult> previousResults;

        const auto results =
            runner.runFailed(
                registry,
                previousResults);

        if (!results.empty())
        {
            std::cerr
                << "Self-test error: expected no failed tests from empty previous results.\n";

            return false;
        }

        return true;
    }

    bool verifyRunFailedIgnoresNonFailedResults(
        TestRunner &runner,
        TestRegistry &registry)
    {
        const std::vector<TestResult> previousResults =
            {
                TestResult(
                    "synthetic.passed",
                    "Synthetic Passed",
                    TestStatus::Passed,
                    TestResult::Duration::zero(),
                    {}),
                TestResult(
                    "synthetic.skipped",
                    "Synthetic Skipped",
                    TestStatus::Skipped,
                    TestResult::Duration::zero(),
                    {}),
                TestResult(
                    "synthetic.error",
                    "Synthetic Error",
                    TestStatus::Error,
                    TestResult::Duration::zero(),
                    {})};

        const auto results =
            runner.runFailed(
                registry,
                previousResults);

        if (!results.empty())
        {
            std::cerr
                << "Self-test error: runFailed should ignore non-failed results.\n";

            return false;
        }

        return true;
    }

    bool verifyRunFailedIgnoresMissingTests(
        TestRunner &runner,
        TestRegistry &registry)
    {
        const std::vector<TestResult> previousResults =
            {
                TestResult(
                    "does.not.exist",
                    "Missing Test",
                    TestStatus::Failed,
                    TestResult::Duration::zero(),
                    {})};

        const auto results =
            runner.runFailed(
                registry,
                previousResults);

        if (!results.empty())
        {
            std::cerr
                << "Self-test error: missing failed tests should be ignored.\n";

            return false;
        }

        return true;
    }

    bool verifyExceptionHandling(
        TestRunner &runner,
        TestRegistry &registry)
    {
        const auto results = runner.runAll(registry);

        if (results.size() != 3)
        {
            std::cerr
                << "Self-test error: expected 3 exception tests, got "
                << results.size()
                << '\n';

            return false;
        }

        if (results[0].status() != TestStatus::Error)
        {
            std::cerr
                << "Self-test error: runtime_error test should be Error.\n";

            return false;
        }

        if (results[0].errorMessage() != "runtime error")
        {
            std::cerr
                << "Self-test error: unexpected runtime_error message.\n";

            return false;
        }

        if (results[1].status() != TestStatus::Error)
        {
            std::cerr
                << "Self-test error: unknown exception test should be Error.\n";

            return false;
        }

        if (results[1].errorMessage() != "Unknown exception")
        {
            std::cerr
                << "Self-test error: unexpected unknown exception message.\n";

            return false;
        }

        if (results[2].status() != TestStatus::Passed)
        {
            std::cerr
                << "Self-test error: test after exception should still run.\n";

            return false;
        }

        return true;
    }

    bool verifyExceptionContextCleanup(
        TestRunner &runner,
        TestRegistry &registry)
    {
        const TestResult result =
            runner.run(
                registry,
                "exceptions.runtime_error");

        if (result.status() != TestStatus::Error)
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
            return true;
        }
    }

    bool verifyReporterLifecycle(
        TestRegistry &registry)
    {
        RecordingReporter reporter;
        TestRunner runner(&reporter);

        const TestCase *test =
            registry.find("assertions.passing");

        if (test == nullptr)
        {
            std::cerr
                << "Self-test error: lifecycle test case was not found.\n";

            return false;
        }

        const auto singleResult = runner.run(*test);

        if (singleResult.status() != TestStatus::Passed)
        {
            std::cerr
                << "Self-test error: lifecycle single test should pass.\n";

            return false;
        }

        if (reporter.startedCount != 1 ||
            reporter.finishedCount != 1 ||
            reporter.runFinishedCount != 0)
        {
            std::cerr
                << "Self-test error: invalid single-test reporter lifecycle.\n";

            return false;
        }

        const auto results = runner.runAll(registry);

        if (reporter.startedCount != 8 ||
            reporter.finishedCount != 8 ||
            reporter.runFinishedCount != 1)
        {
            std::cerr
                << "Self-test error: invalid runAll reporter lifecycle.\n";

            return false;
        }

        if (reporter.completedRuns.size() != 1 ||
            reporter.completedRuns.front().size() != registry.tests().size())
        {
            std::cerr
                << "Self-test error: invalid runFinished result set.\n";

            return false;
        }

        if (results.size() != registry.tests().size())
        {
            std::cerr
                << "Self-test error: runAll result count mismatch.\n";

            return false;
        }

        return true;
    }

    bool verifyTestResultConsistency(
        TestRegistry &registry,
        TestRegistry &exceptionRegistry)
    {
        TestRunner runner;

        const TestCase *passedTest =
            registry.find("assertions.passing");

        const TestCase *failedTest =
            registry.find("assertions.failing");

        const TestCase *errorTest =
            exceptionRegistry.find("exceptions.runtime_error");

        if (passedTest == nullptr ||
            failedTest == nullptr ||
            errorTest == nullptr)
        {
            std::cerr
                << "Self-test error: required TestResult consistency test case was not found.\n";

            return false;
        }

        const TestResult passed =
            runner.run(*passedTest);

        if (passed.status() != TestStatus::Passed ||
            !passed.failures().empty() ||
            !passed.errorMessage().empty())
        {
            std::cerr
                << "Self-test error: invalid Passed TestResult state.\n";

            return false;
        }

        const TestResult failed =
            runner.run(*failedTest);

        if (failed.status() != TestStatus::Failed ||
            failed.failures().empty() ||
            !failed.errorMessage().empty())
        {
            std::cerr
                << "Self-test error: invalid Failed TestResult state.\n";

            return false;
        }

        const TestResult error =
            runner.run(*errorTest);

        if (error.status() != TestStatus::Error ||
            error.errorMessage().empty())
        {
            std::cerr
                << "Self-test error: invalid Error TestResult state.\n";

            return false;
        }

        return true;
    }

    bool verifyRepeatedExecutionIsolation()
    {
        TestRunner runner;

        TestCase test(
            "stability.repeated",
            "Repeated Execution",
            [](TestContext &context)
            {
                EXPECT_TRUE(context.failures().empty());
            });

        const TestResult first =
            runner.run(test);

        const TestResult second =
            runner.run(test);

        if (first.status() != TestStatus::Passed ||
            second.status() != TestStatus::Passed)
        {
            std::cerr
                << "Self-test error: repeated execution should remain isolated.\n";

            return false;
        }

        if (!first.failures().empty() ||
            !second.failures().empty())
        {
            std::cerr
                << "Self-test error: repeated execution leaked failures.\n";

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

    // Verify empty registry execution
    if (!verifyEmptyRegistry(runner))
    {
        return 1;
    }

    // Verify runFailed with no previous results
    if (!verifyEmptyPreviousResults(
            runner,
            registry))
    {
        return 1;
    }

    // Verify runFailed ignores non-failed results
    if (!verifyRunFailedIgnoresNonFailedResults(
            runner,
            registry))
    {
        return 1;
    }

    // Verify runFailed ignores missing tests
    if (!verifyRunFailedIgnoresMissingTests(
            runner,
            registry))
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

    // Exception handling tests
    TestRegistry exceptionRegistry;
    registerExceptionTests(exceptionRegistry);

    if (!verifyExceptionHandling(
            runner,
            exceptionRegistry))
    {
        return 1;
    }

    if (!verifyExceptionContextCleanup(
            runner,
            exceptionRegistry))
    {
        return 1;
    }

    if (!verifyReporterLifecycle(registry))
    {
        return 1;
    }

    if (!verifyTestResultConsistency(
            registry,
            exceptionRegistry))
    {
        return 1;
    }

    if (!verifyRepeatedExecutionIsolation())
    {
        return 1;
    }

    // Self-test summary
    std::cout
        << "\n=== Framework Self-Tests Passed ===\n";

    return 0;
}