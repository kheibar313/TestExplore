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
- Basic assertions
- Test registration
- Test lookup by ID
- Single-test execution
- Run-all execution
- Failure collection
- Execution timing
- C++20 `std::source_location` support

### Assertions

The current assertion API includes:

```cpp
expectTrue(context, condition);
expectFalse(context, condition);
expectEqual(context, actual, expected);
expectNotEqual(context, actual, expected);
```

---

## Basic Usage

A typical TestExplorer workflow consists of three steps:

1. Define a test using `TestCase`
2. Register the test with `TestRegistry`
3. Execute the registered tests using `TestRunner`

### 1. Define a Test

A test is represented by a `TestCase`. The test body receives a `TestContext`, which is used to perform assertions and collect failures.

```cpp
TestCase test(
    "math.addition",
    "Addition Test",
    [](TestContext& context)
    {
        expectEqual(context, 2 + 2, 4);
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
TestCase test(
    "math.addition",
    "Addition Test",
    [](TestContext& context)
    {
        expectEqual(context, 2 + 2, 4);
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

For example, a successful test produces a result with the `Passed` status, while a test with failed assertions produces a `Failed` result containing the corresponding failure information.

### A More Realistic Example

Tests can contain multiple operations and assertions. TestExplorer does not impose a limit on the complexity of the test logic itself:

```cpp
TestCase test(
    "vector.operations",
    "Vector Operations",
    [](TestContext& context)
    {
        std::vector<int> values = {1, 2, 3, 4, 5};

        expectEqual(context, values.size(), 5);
        expectEqual(context, values.front(), 1);
        expectEqual(context, values.back(), 5);

        values.push_back(6);

        expectEqual(context, values.size(), 6);
        expectTrue(context, values[5] == 6);
    }
);
```

The framework is responsible for executing the test, collecting assertion failures, measuring execution time, and producing the corresponding `TestResult`.

The logic being tested remains entirely within the test itself.

---

## Architecture

TestExplorer is currently built around a small set of independent components:

```text
                    Test Application
                           │
                           ▼
                      TestRunner
                           │
                  ┌────────┴────────┐
                  │                 │
                  ▼                 ▼
            TestRegistry         TestResult
                  │
                  ▼
               TestCase
                  │
                  ▼
             TestContext
                  │
                  ▼
              Assertions
                  │
                  ▼
             TestFailure
```

### TestCase

Represents a test definition.

A `TestCase` contains:

- Test ID
- Test name
- Test function

A `TestCase` does **not** store the result of its execution.

### TestRegistry

Responsible for storing and organizing registered tests.

It currently provides:

- Test registration
- Access to registered tests
- Test lookup by ID

The registry does not execute tests.

### TestRunner

Responsible for executing tests and producing `TestResult` objects.

It currently handles:

- Test execution
- Execution timing
- Failure collection
- Status determination
- Running individual tests
- Running all registered tests

### TestContext

Represents the execution context of a single test.

Currently, it stores assertion failures generated during test execution.

### TestFailure

Represents a single assertion failure.

A failure currently contains:

- Failure message
- `std::source_location`

### TestResult

Represents the result of one test execution.

It contains:

- Test ID
- Test name
- Status
- Duration
- Failures

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
│       └── Assertions.hpp
│
├── src/
│   ├── TestExplorer.cpp
│   ├── TestCase.cpp
│   ├── TestContext.cpp
│   ├── TestFailure.cpp
│   ├── TestResult.cpp
│   ├── TestRunner.cpp
│   └── TestRegistry.cpp
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

- [ ] Professional failure messages
- [ ] Actual value reporting
- [ ] Expected value reporting
- [ ] Correct assertion source-location reporting
- [ ] User-facing assertion macros

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

### Reporting

- [ ] Console reporter
- [ ] Test summaries
- [ ] JSON reporter
- [ ] XML reporter
- [ ] Structured output

### Integration

- [ ] Exit codes
- [ ] CI/CD integration
- [ ] IDE integration
- [ ] CLI test discovery and execution

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

The current development focus is the core execution and assertion architecture.

---

## License

A project license will be added before the first public release.