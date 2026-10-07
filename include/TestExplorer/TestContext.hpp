#ifndef TESTCONTEXT_HPP
#define TESTCONTEXT_HPP

#include <TestExplorer/TestFailure.hpp>
#include <vector>

namespace testexplorer
{

    class TestContext
    {
    public:
        void addFailure(TestFailure failure);

        const std::vector<TestFailure> &failures() const;

    private:
        std::vector<TestFailure> m_failures;
    };

}

#endif