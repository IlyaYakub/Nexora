# Nexora - Project Rules

## Language (apply before anything else)

1. If the user's prompt is in Russian, first translate it to English, then work with the English version. Do not start work before translating.
2. Everything must be in English, always: replies, code comments, identifiers, commit messages, docs, and file contents.

## Project

Vulkan-based game engine, built incrementally. C++23, CMake, Conan. Style and structure follow https://github.com/life-exe/LifeExeEngine.

- Keep the repository root clean: only `CMakeLists.txt`, `README.md`, `.gitignore` (plus folders). Scripts go in `Automation/`, configs in `Config/`.
- Format code with `Automation/format_all_files.bat` (`Config/.clang-format`).