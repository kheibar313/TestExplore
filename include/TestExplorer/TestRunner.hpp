#ifndef TESTRUNNER
#define TESTRUNNER

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

    private:
        TestReporter *m_reporter;
    };
}

#endif
