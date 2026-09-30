#include <TestExplorer/Assertions.hpp>
#include <TestExplorer/ConsoleReporter.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/TestRunner.hpp>

#include <iostream>
#include <string>

using namespace testexplorer;

int main()
{
    TestRegistry registry;

    // Basic passing test
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

    // Multiple assertion failures
    // ------------------------------------------------------------
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

    // String comparison
    // ------------------------------------------------------------
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

    // Multiple failures in one test
    // ------------------------------------------------------------
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

    // TestRunner + ConsoleReporter
    // ------------------------------------------------------------
    ConsoleReporter reporter(std::cout);

    TestRunner runner(&reporter);

    const auto results = runner.runAll(registry);

    // Basic result verification
    // ------------------------------------------------------------
    if (results.size() != 4)
    {
        std::cerr
            << "Self-test error: expected 4 results, got "
            << results.size()
            << '\n';

        return 1;
    }

    if (results[0].status() != TestStatus::Passed)
    {
        std::cerr
            << "Self-test error: Passing Assertions should pass.\n";

        return 1;
    }

    if (results[1].status() != TestStatus::Failed)
    {
        std::cerr
            << "Self-test error: Failing Assertions should fail.\n";

        return 1;
    }

    if (results[2].status() != TestStatus::Passed)
    {
        std::cerr
            << "Self-test error: String Assertions should pass.\n";

        return 1;
    }

    if (results[3].status() != TestStatus::Failed)
    {
        std::cerr
            << "Self-test error: Multiple Failures should fail.\n";

        return 1;
    }

    // Verify failure collection
    // ------------------------------------------------------------
    if (results[1].failures().size() != 4)
    {
        std::cerr
            << "Self-test error: expected 4 failures in "
               "Failing Assertions, got "
            << results[1].failures().size()
            << '\n';

        return 1;
    }

    if (results[3].failures().size() != 3)
    {
        std::cerr
            << "Self-test error: expected 3 failures in "
               "Multiple Failures, got "
            << results[3].failures().size()
            << '\n';

        return 1;
    }

    std::cout
        << "\n=== Framework Self-Tests Passed ===\n";

    return 0;
}