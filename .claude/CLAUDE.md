# Nexora - Project Rules

## Language

1. If the user's prompt is in Russian, first translate it to English, then work with the English version.
2. Everything in the repository is in English: code, comments, identifiers, commit messages, docs.

## Project

Vulkan-based game engine, built incrementally. C++23, CMake, Conan. Style and structure follow https://github.com/life-exe/LifeExeEngine.

- Keep the repository root clean: only `CMakeLists.txt`, `README.md`, `.gitignore` (plus folders). Scripts go in `Automation/`, configs in `Config/`.
- Format code with `Automation/format_all_files.bat` (`Config/.clang-format`).
- Follow `.claude/skills/karpathy-guidelines/SKILL.md`.
- Include order in every file, groups separated by a blank line: the file's own header first (in a `.cpp`), then project includes with `""`, then standard and third-party includes with `<>`.

## Commands

Windows, MSVC 2022, Conan 2, CMake 3.25+. Everything builds into `Build/` (binaries in `Build/bin/<Config>/`).

- Build: `Automation\build_debug.bat` / `Automation\build_release.bat`
- Rebuild after configuring: `cmake --build Build --config Debug`
- Tests: `ctest --test-dir Build -C Debug --output-on-failure`

## Architecture

- `Engine/CMakeLists.txt`: `NexoraEngineLib` (all engine code), `NexoraEngine` (`main.cpp`), `NexoraEngineTestRunner` (GoogleTest, `Engine/Tests/`). Add new source and test files to the lists by hand.
- Include paths are relative to `Engine/Source`. Everything lives in `namespace Nexora`.
- Subsystems have a backend-agnostic interface and a backend in a subfolder (`Window/IWindow.h` -> `Window/GLFW/`, `Graphics/IGraphicsContext.h` -> `Graphics/Vulkan/`).
- Fallible construction uses static `create()` factories returning `std::expected<std::unique_ptr<T>, Error>`, not exceptions.
- Logging: `DEFINE_LOG_CATEGORY_STATIC(LogX)` and `NX_LOG(LogX, Verbosity, "fmt {}", args)`. `Fatal` traps.

## Workflow

- Before writing code, present the plan (what, which files, approach) and wait for approval.
- Change only what is needed. Mention unrelated problems instead of fixing them.

## Testing

- Few, small tests: the main happy path and the most important failure case.
- Tests that need a display or GPU must `GTEST_SKIP()` when it is unavailable, so CI stays green.

## Git and CI

- Keep git and GitHub Actions history minimal: no noisy commits, no failed or redundant runs.
- Check Actions results yourself with `gh run list` and `gh run view --log-failed` after pushing.
