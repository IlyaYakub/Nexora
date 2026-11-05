# Nexora

A Vulkan-based game engine, built incrementally from scratch. C++23, CMake, Conan.

## Layout

| Folder | Purpose |
| --- | --- |
| `Engine/` | Engine sources and its CMake target |
| `Automation/` | Build, clean and format scripts (Windows `.bat`) |
| `Config/` | Conan file and tool configs (`.clang-format`) |
| `.claude/` | Project rules, skills and sub-agents for AI-assisted development |
| `.github/workflows/` | CI (stub) |

## Requirements

CMake 3.25+, Conan 2, a C++23 compiler (MSVC 2022), clang-format.

## Build

```bat
Automation\build_debug.bat
Automation\build_release.bat
```

Other scripts: `generate_project_files.bat`, `clean.bat`, `format_all_files.bat`.
