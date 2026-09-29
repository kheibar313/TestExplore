#ifndef TESTRUNNER
#define TESTRUNNER

#include <TestExplorer/TestResult.hpp>
#include <vector>

namespace testexplorer
{
    class TestCase;
    class TestRegistry;

    class TestRunner
    {
    public:
        TestResult run(const TestCase &test);

        std::vector<TestResult> runAll(
            const TestRegistry &registry);
    };
}

#endif
