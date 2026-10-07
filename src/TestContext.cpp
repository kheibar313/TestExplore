#include <TestExplorer/TestContext.hpp>

#include <utility>

namespace testexplorer
{
    void TestContext::addFailure(TestFailure failure)
    {
        m_failures.push_back(std::move(failure));
    }

    const std::vector<TestFailure> &TestContext::failures() const
    {
        return m_failures;
    }
}