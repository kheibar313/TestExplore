# Building TestExplorer

This document describes how to configure, build, test, and clean the TestExplorer project.

## Requirements

TestExplorer currently requires:

- CMake 3.20 or newer
- A C++20-compatible compiler
- Git

The project is currently developed and tested with:

- Windows 11
- MSYS2 UCRT64
- GCC 16.2.0
- CMake 4.1.2
- C++20
- MinGW Makefiles

Other platforms and toolchains may work, but are not currently part of the project's tested configuration.

---

## Configure the Project

From the repository root:

```bash
cmake -S . -B build -G "MinGW Makefiles"
```

This creates the `build/` directory and generates the build system.

---

## Build

Build the project with:

```bash
cmake --build build --parallel
```

This builds the TestExplorer library, self-tests, CLI, and example targets.

---

## Run Self Tests

After a successful build:

### Windows

```powershell
.\build\TestExplorerSelfTests.exe
```

The self-tests are used to validate the TestExplorer framework itself.

---

## Run the Example

The project also contains a basic example application.

```powershell
.\build\TestExplorerExample.exe
```

---

## Run the CLI

The TestExplorer CLI can be run with:

```powershell
.\build\TestExplorerCLI.exe
```

> The CLI is currently under development and will gain more functionality as the framework evolves.

---

## Clean Build

A clean build is normally **not required** after every source-code change.

If a clean build is needed, remove the `build/` directory and configure the project again.

### Windows — Command Prompt

```cmd
rmdir /s /q build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --parallel
```

### Windows — PowerShell

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --parallel
```

---

## When to Clean the Build

A clean build may be useful when:

- changing the CMake generator
- changing the compiler or toolchain
- making significant changes to `CMakeLists.txt`
- changing the project target structure
- changing major build configuration
- the CMake cache becomes inconsistent
- the build directory appears to be in an invalid state

For normal `.cpp`, `.hpp`, and test changes, simply run:

```bash
cmake --build build --parallel
```

---

## Typical Development Workflow

### First Build

```bash
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --parallel
```

### After Source Changes

```bash
cmake --build build --parallel
```

### Run Self Tests

```powershell
.\build\TestExplorerSelfTests.exe
```

### Run Example

```powershell
.\build\TestExplorerExample.exe
```

---

## Build Directory

The `build/` directory contains generated build files and should not be committed to the repository.

It is normally excluded through `.gitignore`.

---

## Troubleshooting

If the build fails after significant configuration or toolchain changes, try a clean build:

```powershell
Remove-Item -Recurse -Force build
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build --parallel
```

If the problem persists, verify that your compiler supports C++20 and that CMake can detect it correctly.

---

## Quick Reference

| Action | Command |
|---|---|
| Configure | `cmake -S . -B build -G "MinGW Makefiles"` |
| Build | `cmake --build build --parallel` |
| Run Self Tests | `.\build\TestExplorerSelfTests.exe` |
| Run Example | `.\build\TestExplorerExample.exe` |
| Run CLI | `.\build\TestExplorerCLI.exe` |
| Clean | `rmdir /s /q build` |
| Clean — PowerShell | `Remove-Item -Recurse -Force build` |

---

## Development Status

TestExplorer is currently under active development.

Build commands and supported toolchains may change as the project evolves.