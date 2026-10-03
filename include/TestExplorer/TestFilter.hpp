#ifndef TESTFILTER
#define TESTFILTER

#include <TestExplorer/TestCase.hpp>

#include <functional>

namespace testexplorer
{

using TestFilter =
    std::function<bool(const TestCase&)>;

}

#endif