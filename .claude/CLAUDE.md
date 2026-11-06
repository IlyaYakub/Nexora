# Nexora - Project Rules

## Language (apply before anything else)

1. If the user's prompt is in Russian, first translate it to English, then work with the English version. Do not start work before translating.
2. Everything must be in English, always: replies, code comments, identifiers, commit messages, docs, and file contents.

## Project

Vulkan-based game engine, built incrementally. C++23, CMake, Conan. Style and structure follow https://github.com/life-exe/LifeExeEngine.

- Keep the repository root clean: only `CMakeLists.txt`, `README.md`, `.gitignore` (plus folders). Scripts go in `Automation/`, configs in `Config/`.
- Format code with `Automation/format_all_files.bat` (`Config/.clang-format`).
- Follow `.claude/skills/karpathy-guidelines/SKILL.md`.

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
