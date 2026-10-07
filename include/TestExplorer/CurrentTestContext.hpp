#ifndef CURRENTTESTCONTEXT_HPP
#define CURRENTTESTCONTEXT_HPP

namespace testexplorer
{
    class TestContext;

    class CurrentTestContext
    {
    public:
        static void set(TestContext &context);
        static void clear();

        static TestContext &get();
    };
}

#endif