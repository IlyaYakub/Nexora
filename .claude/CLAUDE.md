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

## Git and CI

- Keep the git history and the GitHub Actions history minimal. Do not create noisy commits or leave failed or redundant workflow runs.
- If history gets bloated, squash it into a single clean commit (force-push) and delete old workflow runs, so the remaining run is green.
- Check Actions results yourself with the `gh` CLI after pushing (`gh run list`, `gh run view --log-failed`).
