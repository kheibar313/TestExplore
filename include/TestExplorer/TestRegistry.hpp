#ifndef TESTREGISTRY
#define TESTREGISTRY

#include <E:\MyFile\myCode\TestExplore\include\TestExplorer\TestCase.hpp>
#include <vector>
#include <string>

namespace testexplorer
{
    class TestRegistry
    {
    public:
        void registerTest(TestCase test);

        const std::vector<TestCase> &tests() const;

        const TestCase *find(const std::string &id) const;

    private:
        std::vector<TestCase> m_tests;
    };
}

#endif
