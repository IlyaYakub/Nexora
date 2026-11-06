# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

# Nexora - Project Rules

## Language (apply before anything else)

1. If the user's prompt is in Russian, first translate it to English, then work with the English version. Do not start work before translating.
2. Everything must be in English, always: replies, code comments, identifiers, commit messages, docs, and file contents.

## Project

Vulkan-based game engine, built incrementally. C++23, CMake, Conan. Style and structure follow https://github.com/life-exe/LifeExeEngine.

- Keep the repository root clean: only `CMakeLists.txt`, `README.md`, `.gitignore` (plus folders). Scripts go in `Automation/`, configs in `Config/`.
- Format code with `Automation/format_all_files.bat` (`Config/.clang-format`).
- Follow `.claude/skills/karpathy-guidelines/SKILL.md`.
- Include order in every file, each group separated by a blank line (clang-format does not sort includes, `SortIncludes: false`):
  1. In a `.cpp`, its own header first (`GLFWWindow.cpp` starts with `#include "Window/GLFW/GLFWWindow.h"`).
  2. Project includes with `""`.
  3. Standard and third-party includes with `<>`.

## Commands

Windows, MSVC 2022, Conan 2, CMake 3.25+. Everything builds into `Build/` (multi-config, binaries in `Build/bin/<Config>/`).

- Full build (Conan install + configure + build): `Automation\build_debug.bat` / `Automation\build_release.bat`
- Configure only (generate the VS solution): `Automation\generate_project_files.bat`
- Rebuild after configuring: `cmake --build Build --config Debug`
- Clean: `Automation\clean.bat` (deletes `Build/`)
- All tests: `ctest --test-dir Build -C Debug --output-on-failure`
- Single test: `Build\bin\Debug\NexoraEngineTestRunner.exe --gtest_filter=GLFWWindowManagerTest.CreatedWindowShouldBeFoundById` (or `ctest --test-dir Build -C Debug -R <regex>`)
- CI (`.github/workflows/build.yml`) runs the same Conan/CMake/ctest steps for Debug and Release on `windows-latest`.

## Architecture

- `Engine/CMakeLists.txt` defines three targets: `NexoraEngineLib` (static library with all engine code), `NexoraEngine` (executable, just `main.cpp`) and `NexoraEngineTestRunner` (GoogleTest, tests in `Engine/Tests/`). New source files must be added to `ENGINE_SOURCES` by hand; new test files to the test runner list.
- Precompiled header `Source/pch.h` is force-included into every engine `.cpp` via `setup_precompiled_headers` (`Automation/CMAKE/CMakeHelpers.cmake`). Headers still include what they use.
- Include paths are relative to `Engine/Source` (`#include "Window/IWindow.h"`). Everything lives in `namespace Nexora`.
- `Engine` owns the managers and the main loop; `main.cpp` creates the `Engine`, creates a window, and calls `run()`.
- Subsystems are split into a backend-agnostic interface and a backend implementation in a subfolder (`Window/IWindowManager.h` -> `Window/GLFW/GLFWWindowManager`). Code outside the backend depends only on the interface.
- Fallible construction uses static factories returning `std::expected<std::unique_ptr<T>, Error>` (e.g. `GLFWWindowManager::create()`), not exceptions. Windows are referred to by `WindowId`; `getWindow()` returns a non-owning pointer valid until the next `update()`.
- Logging: declare a category with `DEFINE_LOG_CATEGORY_STATIC(LogX)` and log with `NX_LOG(LogX, Verbosity, "fmt {}", args)` (`NX_LOG_DEBUG` adds source location). Arguments use `std::format`. `Fatal` breaks into the debugger/traps. Do not call `Log::log` directly.
- Shared helpers in `Core/`: fixed-width aliases (`uint8`, `uint32`) and `NonCopyable`.

## Workflow

- Before writing any code, always present your plan of action first (what you will add or change, which files, the approach). Write code only after the user approves the plan.
- Build that plan with `.claude/skills/design-before-code/SKILL.md` (simplest solution, patterns, engine fit, testability).

## Testing

- Test only the key functionality of a unit, not everything. Keep tests few and small.
- Cover the main happy path and the most important failure case. Skip trivial getters, defaults, and variations that add no new information.
- Example for the window system: create the manager, create a window and find it by id, unknown id returns null, closed window is removed. Nothing more.
- Tests that need a display, GPU, or other environment must skip themselves (`GTEST_SKIP()`) when it is unavailable, so CI stays green.

## Git and CI

- Keep the git history and the GitHub Actions history minimal. Do not create noisy commits or leave failed or redundant workflow runs.
- If history gets bloated, squash it into a single clean commit (force-push) and delete old workflow runs, so the remaining run is green.
- Check Actions results yourself with the `gh` CLI after pushing (`gh run list`, `gh run view --log-failed`).
