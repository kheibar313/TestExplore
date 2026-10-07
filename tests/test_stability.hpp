#ifndef TEST_STABILITY_HPP
#define TEST_STABILITY_HPP

#include <TestExplorer/TestRunner.hpp>
#include <TestExplorer/TestRegistry.hpp>

bool runStabilityTests(
    testexplorer::TestRunner &runner,
    testexplorer::TestRegistry &registry,
    testexplorer::TestRegistry &exceptionRegistry);

#endif