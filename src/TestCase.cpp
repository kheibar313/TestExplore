#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestContext.hpp>

namespace testexplorer
{

    TestCase::TestCase(
        std::string id,
        std::string name,
        TestFunction function)
        : m_id(std::move(id)),
          m_name(std::move(name)),
          m_function(std::move(function))
    {
    }

    void TestCase::execute(TestContext &context) const
    {
        m_function(context);
    }

    const std::string &TestCase::id() const
    {
        return m_id;
    }

    const std::string &TestCase::name() const
    {
        return m_name;
    }

}