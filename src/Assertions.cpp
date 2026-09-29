#ifndef ASSERTIONS
#define ASSERTIONS

namespace testexplorer
{
    class TestContext;

    void expectTrue(
        TestContext &context,
        bool condition);

    void expectFalse(
        TestContext &context,
        bool condition);

    template <typename T, typename U>
    void expectEqual(
        TestContext &context,
        const T &actual,
        const U &expected);

    template <typename T, typename U>
    void expectNotEqual(
        TestContext &context,
        const T &actual,
        const U &expected);
}

#endif