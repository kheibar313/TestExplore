#ifndef TESTFILTER_HPP
#define TESTFILTER_HPP

#include <TestExplorer/TestCase.hpp>

#include <functional>

namespace testexplorer
{

    using TestFilter =
        std::function<bool(const TestCase &)>;

}

#endif