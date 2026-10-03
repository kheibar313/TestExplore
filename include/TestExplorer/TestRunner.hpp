#ifndef TESTRUNNER
#define TESTRUNNER

#include <TestExplorer/TestFilter.hpp>
#include <TestExplorer/TestResult.hpp>

#include <vector>

namespace testexplorer
{
    class TestCase;
    class TestRegistry;
    class TestReporter;

    class TestRunner
    {
    public:
        explicit TestRunner(
            TestReporter *reporter = nullptr);

        TestResult run(
            const TestCase &test);

        std::vector<TestResult> runAll(
            const TestRegistry &registry);

        std::vector<TestResult> runAll(
            const TestRegistry &registry,
            TestFilter filter);

    private:
        TestReporter *m_reporter;
    };
}

#endif
