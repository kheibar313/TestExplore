#ifndef ASSERTIONS
#define ASSERTIONS

#include <TestExplorer/TestContext.hpp>
#include <TestExplorer/TestFailure.hpp>

#include <string>

namespace testexplorer
{

    inline void expectTrue(
        TestContext &context,
        bool condition)
    {
        if (!condition)
        {
            context.addFailure(
                TestFailure("Expected condition to be true"));
        }
    }

    inline void expectFalse(
        TestContext &context,
        bool condition)
    {
        if (condition)
        {
            context.addFailure(
                TestFailure("Expected condition to be false"));
        }
    }

    template <typename T, typename U>
    void expectEqual(
        TestContext &context,
        const T &actual,
        const U &expected)
    {
        if (!(actual == expected))
        {
            context.addFailure(
                TestFailure("Expected values to be equal"));
        }
    }

    template <typename T, typename U>
    void expectNotEqual(
        TestContext &context,
        const T &actual,
        const U &expected)
    {
        if (actual == expected)
        {
            context.addFailure(
                TestFailure("Expected values to be different"));
        }
    }

}

#endif