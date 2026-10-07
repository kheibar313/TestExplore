#ifndef TESTFAILURE_HPP
#define TESTFAILURE_HPP

#include <string>
#include <source_location>

namespace testexplorer
{
    class TestFailure
    {
    public:
        TestFailure(
            std::string message,
            std::source_location location = std::source_location::current());

        const std::string &message() const;
        const std::source_location &location() const;

    private:
        std::string m_message;
        std::source_location m_location;
    };
}

#endif