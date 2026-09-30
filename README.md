# TestExplorer

A lightweight, reusable, dependency-free C++20 testing framework for building a clean and extensible test execution system.

> **Status:** Early Development — API and architecture are subject to change.

## Overview

**TestExplorer** is an experimental C++ testing framework built from the ground up with a focus on clean architecture, separation of responsibilities, and extensibility.

The project aims to provide a reusable testing library that can eventually support features commonly found in modern test frameworks and IDE test explorers.

The current implementation focuses on establishing a reliable testing core before adding higher-level features such as filtering, fixtures, parameterized tests, reporting, and CI/CD integration.

---

## Current Features

The current core provides:

- `TestCase`
- `TestContext`
- `TestFailure`
- `TestResult`
- `TestRegistry`
- `TestRunner`
- Assertion functions
- User-facing assertion macros
- Test registration
- Test lookup by ID
- Single-test execution
- Run-all execution
- Failure collection
- Execution timing
- C++20 `std::source_location` support
- Current test context management
- Initial reporter abstraction

### Assertions

TestExplorer provides a user-facing assertion API through the following macros:

```cpp
EXPECT_TRUE(condition);
EXPECT_FALSE(condition);
EXPECT_EQ(actual, expected);
EXPECT_NE(actual, expected);
```

For example:

```cpp
TestCase test(
    "math.addition",
    "Addition Test",
    [](TestContext& context)
    {
        EXPECT_EQ(2 + 2, 4);
        EXPECT_TRUE(10 > 5);
    }
);
```

The assertion macros provide a clean test-writing interface while the underlying assertion functions remain responsible for performing the actual checks and recording failures.

Assertion failures currently include:

- Failure messages
- Expected values where applicable
- Actual values where applicable
- `std::source_location`

For example:

```text
Expected values to be equal
Expected: world
Actual:   hello
```

The reported source location points to the actual assertion call inside the test.

---

## Basic Usage

A typical TestExplorer workflow consists of three steps:

1. Define a test using `TestCase`
2. Register the test with `TestRegistry`
3. Execute the registered tests using `TestRunner`

### 1. Define a Test

A test is represented by a `TestCase`.

The test body currently receives a `TestContext`, while the public assertion API accesses the active context automatically through TestExplorer's current-context mechanism.

```cpp
TestCase test(
    "math.addition",
    "Addition Test",
    [](TestContext& context)
    {
        EXPECT_EQ(2 + 2, 4);
        EXPECT_TRUE(10 > 5);
    }
);
```

### 2. Register the Test

Tests are explicitly registered in a `TestRegistry`:

```cpp
TestRegistry registry;

registry.registerTest(test);
```

Multiple tests can be registered in the same registry:

```cpp
registry.registerTest(additionTest);
registry.registerTest(subtractionTest);
registry.registerTest(multiplicationTest);
```

### 3. Run the Tests

A `TestRunner` executes the registered tests and produces a `TestResult` for each execution:

```cpp
TestRunner runner;

const auto results = runner.runAll(registry);
```

The complete workflow can therefore be summarized as:

```cpp
#include <TestExplorer/Assertions.hpp>
#include <TestExplorer/TestCase.hpp>
#include <TestExplorer/TestRegistry.hpp>
#include <TestExplorer/TestRunner.hpp>

TestCase test(
    "math.addition",
    "Addition Test",
    [](TestContext& context)
    {
        EXPECT_EQ(2 + 2, 4);
        EXPECT_TRUE(10 > 5);
    }
);

TestRegistry registry;
registry.registerTest(test);

TestRunner runner;

const auto results = runner.runAll(registry);
```

### Test Results

Each test execution produces a `TestResult` containing:

- Test ID
- Test name
- Execution status
- Execution duration
- Failure information

Current statuses are:

```cpp
enum class TestStatus
{
    Passed,
    Failed,
    Skipped,
    Error
};
```

A successful test produces a `Passed` result, while a test with failed assertions produces a `Failed` result containing the corresponding failure information.

---

## A More Realistic Example

Tests can contain multiple operations and assertions. TestExplorer does not impose a limit on the complexity of the test logic itself.

```cpp
TestCase test(
    "vector.operations",
    "Vector Operations",
    [](TestContext& context)
    {
        std::vector<int> values = {1, 2, 3, 4, 5};

        EXPECT_EQ(values.size(), 5);
        EXPECT_EQ(values.front(), 1);
        EXPECT_EQ(values.back(), 5);

        values.push_back(6);

        EXPECT_EQ(values.size(), 6);
        EXPECT_TRUE(values[5] == 6);
    }
);
```

The framework is responsible for:

- Executing the test
- Managing its execution context
- Collecting assertion failures
- Measuring execution time
- Producing the corresponding `TestResult`

The logic being tested remains entirely within the test itself.

---

## Architecture

TestExplorer is built around a set of independent components with clearly separated responsibilities.

```text
                         Test Application
                                │
                                ▼
                           TestRunner
                                │
              ┌─────────────────┼─────────────────┐
              │                 │                 │
              ▼                 ▼                 ▼
        TestRegistry        TestContext       Reporter
              │                 │                 │
              ▼                 ▼                 ▼
          TestCase          Assertions      TestResult
                                │
                                ▼
                           TestFailure
```

The intended dependency direction is:

```text
Consumer Tests / CLI / Examples
              │
              ▼
      TestExplorer Library
```

The framework is designed so that the execution core does not depend on a specific reporting format.

---

### TestCase

Represents a test definition.

A `TestCase` contains:

- Test ID
- Test name
- Test function

A `TestCase` does **not** store the result of its execution.

---

### TestRegistry

Responsible for storing and organizing registered tests.

It currently provides:

- Test registration
- Access to registered tests
- Test lookup by ID

The registry does not execute tests.

---

### TestRunner

Responsible for executing tests and producing `TestResult` objects.

It currently handles:

- Test execution
- Execution timing
- Failure collection
- Status determination
- Running individual tests
- Running all registered tests
- Managing the active `TestContext`

The runner does not perform assertion comparisons or format test output.

---

### TestContext

Represents the execution context of a single test.

Currently, it stores assertion failures generated during test execution.

---

### CurrentTestContext

`CurrentTestContext` provides temporary access to the `TestContext` associated with the currently executing test.

Its purpose is to allow the user-facing assertion API to remain simple:

```cpp
EXPECT_EQ(actual, expected);
```

instead of requiring:

```cpp
expectEqual(context, actual, expected);
```

The current context is stored using `thread_local` state so that the mechanism does not depend on a process-wide shared context.

---

### TestFailure

Represents a single assertion failure.

A failure currently contains:

- Failure message
- `std::source_location`

---

### TestResult

Represents the result of one test execution.

It contains:

- Test ID
- Test name
- Status
- Duration
- Failures

---

### TestReporter

`TestReporter` defines the reporting interface used to separate test execution from result presentation.

The reporter abstraction currently provides lifecycle hooks for:

```cpp
testStarted(...)
testFinished(...)
testRunFinished(...)
```

This allows future reporters such as:

- Console reporter
- JSON reporter
- XML reporter
- IDE-oriented reporters

to consume test execution results without coupling those output formats to `TestRunner`.

---

## Project Structure

```text
TestExplore/
├── CMakeLists.txt
├── README.md
├── BUILD.md
│
├── apps/
│   └── testexplorer/
│       └── main.cpp
│
├── examples/
│   └── basic/
│       └── main.cpp
│
├── include/
│   └── TestExplorer/
│       ├── TestExplorer.hpp
│       ├── TestCase.hpp
│       ├── TestContext.hpp
│       ├── TestFailure.hpp
│       ├── TestResult.hpp
│       ├── TestRunner.hpp
│       ├── TestRegistry.hpp
│       ├── Assertions.hpp
│       ├── CurrentTestContext.hpp
│       └── TestReporter.hpp
│
├── src/
│   ├── TestExplorer.cpp
│   ├── TestCase.cpp
│   ├── TestContext.cpp
│   ├── TestFailure.cpp
│   ├── TestResult.cpp
│   ├── TestRunner.cpp
│   ├── TestRegistry.cpp
│   └── CurrentTestContext.cpp
│
└── tests/
    └── main.cpp
```

### Current Executables

The repository currently contains three executable targets:

| Target | Purpose | Status |
|---|---|---|
| `TestExplorerSelfTests` | Tests the framework itself | Active |
| `TestExplorerExample` | Basic framework usage example | Under development |
| `TestExplorerCLI` | Future command-line test runner | Under development |

The `examples` and `apps` directories are currently project scaffolding and will be implemented as the framework develops.

---

## Requirements

TestExplorer requires:

- **C++20**
- **CMake 3.20+**
- A C++20-compatible compiler

The current development environment is:

- Windows 11
- MSYS2 UCRT64
- GCC 16.2.0
- CMake 4.1.2
- MinGW Makefiles

The framework is designed to remain dependency-free and rely primarily on the C++ standard library.

---

## Building

Clone the repository:

```bash
git clone https://github.com/kheibar313/TestExplore.git
cd TestExplore
```

Configure the project:

```bash
cmake -S . -B build -G "MinGW Makefiles"
```

Build:

```bash
cmake --build build --parallel
```

Run the framework self-tests:

```powershell
.\build\TestExplorerSelfTests.exe
```

For complete build, clean-build, and development commands, see [`BUILD.md`](BUILD.md).

---

## Development Roadmap

The project is being developed incrementally.

### Core

- [x] Test case model
- [x] Test context
- [x] Failure representation
- [x] Test result model
- [x] Test registry
- [x] Test runner
- [x] Basic assertions
- [x] Single-test execution
- [x] Run-all execution
- [x] Execution timing

### Assertions

- [x] Professional failure messages
- [x] Actual value reporting
- [x] Expected value reporting
- [x] Correct assertion source-location reporting
- [x] User-facing assertion macros
- [x] Current test context

### Reporting

- [x] Reporter abstraction
- [ ] Console reporter
- [ ] Test summaries
- [ ] JSON reporter
- [ ] XML reporter
- [ ] Structured output

### Test Organization

- [ ] Test filtering
- [ ] Run single test by ID
- [ ] Run failed tests
- [ ] Test groups / suites
- [ ] Tags / categories
- [ ] Test hierarchy

### Test Lifecycle

- [ ] Fixtures
- [ ] Setup / teardown
- [ ] Exception handling
- [ ] Parameterized tests
- [ ] Test data

### Integration

- [ ] Exit codes
- [ ] CI/CD integration
- [ ] CLI test discovery and execution
- [ ] IDE integration

The roadmap is intentionally incremental. New features will be introduced only after the underlying architecture is stable enough to support them.

---

## Design Goals

### Separation of Responsibilities

Test registration, execution, assertions, results, and reporting should remain independent components.

### Reusability

The core framework should be usable as a library rather than being tied to a single application.

### Minimal Dependencies

The framework should rely primarily on the C++ standard library.

### Extensibility

The architecture should allow features such as reporters, filtering, fixtures, and parameterized tests to be added without rewriting the core execution model.

### Clear Execution Model

A test definition should remain independent from the result of its execution.

### Incremental Development

Features should be introduced in small, testable milestones rather than building the entire framework at once.

---

## Current Non-Goals

The following are intentionally outside the current scope:

- Graphical test explorer
- Reflection system
- Plugin architecture
- Distributed test execution
- Dependency injection
- Complex matcher framework
- Premature parallel execution

These may be reconsidered later if actual project requirements justify them.

---

## Project Status

TestExplorer is an experimental open-source project under active development.

The API is **not stable** and may change significantly before the first release.

The current development focus is the framework's execution, assertion, and reporting architecture.

---

## License

A project license will be added before the first public release.