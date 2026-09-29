#ifndef TESTCASE
#define TESTCASE

#include <functional>
#include <string>

namespace testexplorer
{
    class TestContext;

    class TestCase
    {
    public:
        using TestFunction = std::function<void(TestContext &)>;

        TestCase(
            std::string id,
            std::string name,
            TestFunction function);

        void execute(TestContext &context) const;

        const std::string &id() const;
        const std::string &name() const;

    private:
        std::string m_id;
        std::string m_name;
        TestFunction m_function;
    };
}

#endif