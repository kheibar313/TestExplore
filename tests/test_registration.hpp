#ifndef TEST_REGISTRATION_HPP
#define TEST_REGISTRATION_HPP

#include <TestExplorer/TestRegistry.hpp>

void registerFrameworkTests(
    testexplorer::TestRegistry &registry);

void registerExceptionTests(
    testexplorer::TestRegistry &registry);

#endif