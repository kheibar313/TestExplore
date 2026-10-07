#ifndef TESTREPORTER_HPP
#define TESTREPORTER_HPP

#include <TestExplorer/TestResult.hpp>
#include <TestExplorer/TestCase.hpp>

#include <vector>

namespace testexplorer
{
    class TestReporter
    {
    public:
        virtual ~TestReporter() = default;

        virtual void testStarted(
            const TestCase &test) = 0;

        virtual void testFinished(
            const TestResult &result) = 0;

        virtual void testRunFinished(
            const std::vector<TestResult> &results) = 0;
    };
}

#endif