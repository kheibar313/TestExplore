#ifndef CONSOLEREPORTER
#define CONSOLEREPORTER

#include <TestExplorer/TestReporter.hpp>

#include <iosfwd>

namespace testexplorer
{
    class ConsoleReporter : public TestReporter
    {
    public:
        explicit ConsoleReporter(
            std::ostream &output);

        void testStarted(
            const TestCase &test) override;

        void testFinished(
            const TestResult &result) override;

        void testRunFinished(
            const std::vector<TestResult> &results) override;

    private:
        std::ostream &m_output;
    };
}

#endif