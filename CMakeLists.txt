cmake_minimum_required(VERSION 3.20)

project(
    TestExplorer
    VERSION 0.1.0
    DESCRIPTION "A lightweight, reusable C++20 testing framework"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# TestExplorer Library
# ============================================================

add_library(TestExplorer STATIC
    src/TestExplorer.cpp
)

target_include_directories(TestExplorer
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(TestExplorer
    PUBLIC
        cxx_std_20
)

# ============================================================
# TestExplorer CLI
# ============================================================

add_executable(TestExplorerCLI
    apps/testexplorer/main.cpp
)

target_link_libraries(TestExplorerCLI
    PRIVATE
        TestExplorer
)

# ============================================================
# TestExplorer Self Tests
# ============================================================

add_executable(TestExplorerSelfTests
    tests/main.cpp
)

target_link_libraries(TestExplorerSelfTests
    PRIVATE
        TestExplorer
)

# ============================================================
# Example
# ============================================================

add_executable(TestExplorerExample
    examples/basic/main.cpp
)

target_link_libraries(TestExplorerExample
    PRIVATE
        TestExplorer
)