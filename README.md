# TestExplorer

A lightweight, reusable, dependency-free C++20 testing framework designed around a clean and extensible test execution architecture.

> **Status:** Early Development — API and architecture are subject to change.

## Overview

**TestExplorer** is an experimental C++ testing framework built from the ground up with a focus on:

- Clean architecture
- Separation of responsibilities
- Reusability
- Extensibility
- Explicit execution flow
- Minimal dependencies

The project is designed to provide a reusable testing library that can gradually evolve toward features commonly found in modern testing frameworks and IDE test explorers.

The development strategy is intentionally incremental: the execution core is established and validated first, while higher-level features are introduced as separate, testable milestones.

---

## Current Features

The current implementation provides:

- `TestCase`
- `TestContext`
- `TestFailure`
- `TestResult`
- `TestRegistry`
- `TestRunner`
- Assertion functions
- User-facing assertion macros
- Explicit test registration
- Test lookup by ID
- Single-test execution
- Run-all execution
- Test filtering
- Run failed tests
- Failure collection
- Execution timing
- C++20 `std::source_location` support
- Current test context management
- Reporter abstraction
- Console reporter
- Self-tests for framework validation

---

## Assertions

TestExplorer provides a user-facing assertion API through simple macros:

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
    [](TestContext&)
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

A typical TestExplorer workflow consists of three main steps:

1. Define a test using `TestCase`
2. Register the test with `TestRegistry`
3. Execute the registered tests using `TestRunner`

### 1. Define a Test

A test is represented by a `TestCase`.

The test body receives a `TestContext`, while the public assertion API accesses the active context automatically through TestExplorer's current-context mechanism.

```cpp
TestCase test(
    "math.addition",
    "Addition Test",
    [](TestContext&)
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

A `TestRunner` executes tests and produces a `TestResult` for each execution:

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

using namespace testexplorer;

int main()
{
    TestCase test(
        "math.addition",
        "Addition Test",
        [](TestContext&)
        {
            EXPECT_EQ(2 + 2, 4);
            EXPECT_TRUE(10 > 5);
        }
    );

    TestRegistry registry;
    registry.registerTest(test);

    TestRunner runner;

    const auto results = runner.runAll(registry);
}
```

---

## Test Results

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

A successful test produces a `Passed` result, while a test with failed assertions produces a `Failed` result containing the corresponding failures.

`Skipped` and `Error` are part of the result model, but their full execution semantics are planned for future milestones.

---

## Test Filtering and Single Test Execution

TestExplorer supports selecting tests before execution through a lightweight filtering API.

A filter is represented by:

```cpp
using TestFilter =
    std::function<bool(const TestCase&)>;
```

The filter receives each registered `TestCase` and returns:

- `true` → execute the test
- `false` → exclude the test

For example, to execute only tests whose IDs start with `math.`:

```cpp
const auto results = runner.runAll(
    registry,
    [](const TestCase& test)
    {
        return test.id().starts_with("math.");
    }
);
```

### Single Test Execution

A single registered test can also be executed directly by its ID:

```cpp
const TestResult result =
    runner.run(registry, "math.addition");
```

The registry-aware overload resolves the requested test through `TestRegistry` and delegates the actual execution to the existing `run(const TestCase&)` implementation.

If the requested test ID does not exist, `std::invalid_argument` is thrown.

Single-test execution uses the same execution lifecycle as normal test execution, including:

- Test context management
- Execution timing
- Result generation
- Reporter notifications

The execution flow is:

```text
TestRegistry
     │
     ├── TestFilter ──► selected TestCases
     │                       │
     │                       ▼
     │                  TestRunner::run()
     │                       │
     │                       ▼
     │                  TestResult
     │
     └── Test ID ─────► TestRegistry::find()
                              │
                              ▼
                         TestCase
                              │
                              ▼
                       TestRunner::run()
```

The current implementation has been validated with:

- A filter selecting two tests
- A filter matching no tests
- A filter matching all registered tests
- Single-test execution by ID
- Invalid test ID handling

### Run Failed Tests

TestExplorer can rerun tests that failed during a previous execution.

The previous results are supplied explicitly to `runFailed()`:

```cpp
const auto results = runner.runAll(registry);

const auto failedResults =
    runner.runFailed(registry, results);
```

`runFailed()` examines the supplied `TestResult` objects and reruns only the tests whose previous status is `TestStatus::Failed`.

The following results are not rerun:

- `Passed`
- `Skipped`
- `Error`

The API is:

```cpp
std::vector<TestResult> runFailed(
    const TestRegistry& registry,
    const std::vector<TestResult>& previousResults
);
```

The execution flow is:

```text
Previous TestResults
        │
        ▼
  TestStatus::Failed?
      /        \
    No          Yes
    │            │
    ▼            ▼
  Ignore    TestRegistry::find()
                 │
                 ▼
              TestCase
                 │
                 ▼
        TestRunner::run()
                 │
                 ▼
            TestResult
```

`TestRunner` does not store the previous results internally. The caller owns the previous execution results and explicitly passes them to `runFailed()`.

Each selected failed test is executed through the existing:

```cpp
run(const TestCase&)
```

execution path. This keeps the actual test execution logic centralized rather than introducing a separate execution mechanism for failed tests.

Reporter lifecycle is also preserved. When a reporter is configured, the failed-test execution uses the same test execution notifications as normal execution and reports the completion of the `runFailed()` operation.

If a previously failed test ID can no longer be found in the supplied `TestRegistry`, the current implementation skips that test.

Exception handling is intentionally not part of this milestone and remains a separate future feature.

The current implementation has been validated with:

- A previous result set containing failed and passed tests
- Rerunning exactly the previously failed tests
- Multiple failed tests
- A result set containing no failed tests
- Missing test IDs being skipped

---

## Reporting

TestExplorer separates test execution from result presentation through the `TestReporter` abstraction.

A reporter receives lifecycle events:

```cpp
testStarted(...);
testFinished(...);
testRunFinished(...);
```

A reporter can therefore observe individual test execution as well as the completion of an entire test run.

### Console Reporter

The current implementation includes `ConsoleReporter`:

```cpp
#include <TestExplorer/ConsoleReporter.hpp>

ConsoleReporter reporter(std::cout);
TestRunner runner(&reporter);

const auto results = runner.runAll(registry);
```

The console reporter currently displays:

- Test name
- Test ID
- Status
- Execution duration
- Failure messages
- Failure source locations
- Test run summary

Example:

```text
Math Addition [math.addition]: PASSED | 400 ns
Math Subtraction [math.subtraction]: PASSED | 400 ns

=== Test Run Finished ===
Tests: 2
```

The reporter is injected into `TestRunner` as a non-owning dependency. This keeps the execution core independent from a specific output format.

Future reporters can therefore be introduced without changing the core execution model.

---

## A More Realistic Example

Tests can contain multiple operations and assertions. TestExplorer does not impose a limit on the complexity of the test logic itself.

```cpp
TestCase test(
    "vector.operations",
    "Vector Operations",
    [](TestContext&)
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
- Reporting execution results when a reporter is configured

The logic being tested remains entirely within the test itself.

---

## Architecture

TestExplorer is built around independent components with clearly separated responsibilities.

```text
                         Test Application
                                │
                                ▼
                            TestRunner
                                │
              ┌─────────────────┼─────────────────┐
              │                 │                 │
              ▼                 ▼                 ▼
        TestRegistry       TestContext        Reporter
              │                 │                 │
              ▼                 ▼                 ▼
          TestCase          Assertions       TestResult
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

The execution core does not depend on a specific reporting implementation.

### Core Responsibilities

```text
TestRegistry
     │
     │ stores / discovers
     ▼
 TestCase
     │
     │ executes
     ▼
 TestRunner
     │
     ├── creates TestContext
     ├── manages execution
     ├── measures duration
     ├── determines status
     └── produces TestResult
              │
              ▼
           Reporter
```

Assertions operate inside the active `TestContext` and record `TestFailure` objects when an assertion fails.

Test selection is kept separate from execution. Filtering selects tests before normal execution, while single-test and failed-test execution reuse the same underlying `run(const TestCase&)` execution path.

---

## TestCase

`TestCase` represents a test definition.

A `TestCase` contains:

- Test ID
- Test name
- Test function

A `TestCase` does **not** store the result of its execution.

This separation allows the same test definition to be executed multiple times while each execution produces an independent `TestResult`.

---

## TestRegistry

`TestRegistry` is responsible for storing and organizing registered tests.

It currently provides:

- Test registration
- Access to registered tests
- Test lookup by ID

The registry does not execute tests.

This keeps test discovery and storage separate from execution.

---

## TestRunner

`TestRunner` is responsible for executing tests and producing `TestResult` objects.

It currently handles:

- Individual test execution
- Single-test execution by ID
- Running all registered tests
- Test filtering
- Rerunning previously failed tests
- Execution timing
- Failure collection
- Status determination
- Active `TestContext` management
- Reporter notification

The runner does not:

- Perform assertion comparisons
- Store the test registry
- Store previous test results
- Format console output
- Implement a specific reporting format

The current core execution API includes:

```cpp
TestResult run(
    const TestCase& test
);

TestResult run(
    const TestRegistry& registry,
    const std::string& testId
);

std::vector<TestResult> runAll(
    const TestRegistry& registry
);

std::vector<TestResult> runAll(
    const TestRegistry& registry,
    TestFilter filter
);

std::vector<TestResult> runFailed(
    const TestRegistry& registry,
    const std::vector<TestResult>& previousResults
);
```

The registry-aware `run()` overload resolves a test by its registered ID and delegates execution to `run(const TestCase&)`.

If the requested test cannot be found, the runner throws `std::invalid_argument`.

`runFailed()` does not introduce a separate execution mechanism. It selects previously failed tests and delegates their actual execution to the existing `run(const TestCase&)` implementation.

This keeps the execution model centralized and makes different test-selection strategies independent from the core execution path.

---

## TestContext

`TestContext` represents the execution context of a single test.

Currently, it stores assertion failures generated during test execution.

A fresh context is created for each test execution.

---

## CurrentTestContext

`CurrentTestContext` provides temporary access to the `TestContext` associated with the currently executing test.

Its purpose is to keep the user-facing assertion API simple:

```cpp
EXPECT_EQ(actual, expected);
```

instead of requiring:

```cpp
expectEqual(context, actual, expected);
```

The current context is stored using `thread_local` state so that the mechanism does not depend on a process-wide shared context.

---

## TestFailure

`TestFailure` represents a single assertion failure.

A failure currently contains:

- Failure message
- `std::source_location`

This allows the framework to report not only what failed, but also where the assertion was made.

---

## TestResult

`TestResult` represents the result of one test execution.

It contains:

- Test ID
- Test name
- Status
- Duration
- Failures

Keeping `TestResult` separate from `TestCase` allows one test definition to produce multiple independent execution results.

This is particularly important for features such as failed-test reruns, where a single `TestCase` may be executed multiple times and each execution must produce its own result.

---

## TestReporter

`TestReporter` defines the reporting interface used to separate test execution from result presentation.

The reporter abstraction currently provides:

```cpp
testStarted(...);
testFinished(...);
testRunFinished(...);
```

The current implementation includes `ConsoleReporter`.

Future reporters may include:

- JSON reporter
- XML reporter
- IDE-oriented reporters
- Other structured output formats

The goal is to add reporting capabilities without coupling those output formats to `TestRunner`.

---

## Project Structure

```text
TestExplore/
├── CMakeLists.txt
├── README.md
├── BUILD.md
├── .gitignore
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
│       ├── TestReporter.hpp
│       ├── ConsoleReporter.hpp
│       └── TestFilter.hpp
│
├── src/
│   ├── TestExplorer.cpp
│   ├── TestCase.cpp
│   ├── TestContext.cpp
│   ├── TestFailure.cpp
│   ├── TestResult.cpp
│   ├── TestRunner.cpp
│   ├── TestRegistry.cpp
│   ├── CurrentTestContext.cpp
│   └── ConsoleReporter.cpp
│
└── tests/
    └── main.cpp
```

### Current Executables

The repository currently contains three executable targets:

| Target | Purpose | Status |
|---|---|---|
| `TestExplorerSelfTests` | Tests the framework itself | Active |
| `TestExplorerExample` | Demonstrates framework usage | Under development |
| `TestExplorerCLI` | Future command-line test runner | Under development |

The `tests` directory contains the framework's own self-tests.

The `apps` and `examples` directories are currently scaffolding for future development.

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

For complete build, clean-build, troubleshooting, and development commands, see [`BUILD.md`](BUILD.md).

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
- [x] Single-test execution at core level
- [x] Run-all execution
- [x] Execution timing
- [x] Current test context

### Assertions

- [x] Professional failure messages
- [x] Actual value reporting
- [x] Expected value reporting
- [x] Correct assertion source-location reporting
- [x] User-facing assertion macros

### Reporting

- [x] Reporter abstraction
- [x] Console reporter
- [ ] Test summaries
- [ ] JSON reporter
- [ ] XML reporter
- [ ] Structured output

### Test Organization

- [x] Test filtering
- [x] Run single test by ID
- [x] Run failed tests
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

Test registration, execution, assertions, results, filtering, and reporting should remain independent components.

### Reusability

The core framework should be usable as a library rather than being tied to a single application or IDE.

### Minimal Dependencies

The framework should rely primarily on the C++ standard library.

### Extensibility

The architecture should allow features such as reporters, filtering, fixtures, and parameterized tests to be added without rewriting the core execution model.

### Clear Execution Model

A test definition should remain independent from the result of its execution.

### Explicit Test Selection

Test selection should remain separate from execution so that features such as tags, groups, failed-test reruns, and CLI filters can build on the same execution model.

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

The current implementation has established and validated the framework's initial execution, assertion, filtering, single-test execution, failed-test rerun, and reporting architecture.

The test execution core currently supports:

- Running all registered tests
- Filtering tests before execution
- Running an individual test by ID
- Rerunning tests that previously failed
- Collecting assertion failures
- Measuring execution time
- Reporting execution lifecycle events

The next development focus is expanding test execution and organization capabilities while keeping the core architecture small and maintainable.

---

## License

A project license will be added before the first public release.