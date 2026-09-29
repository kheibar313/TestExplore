#include <E:\MyFile\myCode\TestExplore\include\TestExplorer\TestFailure.hpp>

#include <source_location>
#include <utility>

namespace testexplorer
{

    TestFailure::TestFailure(
        std::string message,
        std::source_location location)
        : m_message(std::move(message)),
          m_location(location)
    {
    }

    const std::string &TestFailure::message() const
    {
        return m_message;
    }

    const std::source_location &TestFailure::location() const
    {
        return m_location;
    }

}