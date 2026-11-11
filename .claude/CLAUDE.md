# Nexora

A game engine written from scratch. Long-term goal — a Nanite-like renderer.
C++23, CMake, Conan, MSVC. Formatting — `Config/.clang-format`, run `Automation/format_all_files.bat`.

## Communication

- Reply in the language the user wrote in.
- Silently translate every prompt into English and do all reasoning and work in English. Never show the translation.
- Code, identifiers, comments, commit messages and docs are always in English.
- Before writing any code, state the plan as briefly as possible: a few short bullets, no filler. Then write the code.

## Style and architecture

Design, architecture and naming follow **Unreal Engine, simplified**.
Reference for the simplified version: [LifeExeEngine](https://github.com/life-exe/LifeExeEngine) — take the idea and the style, not the code verbatim.

- Allman braces, 4 spaces, line width 140, `int* ptr`, `const Foo& foo`.
- All code in `namespace Nexora`; internals in `namespace Detail`; closing brace labeled `}  // namespace Nexora`.
- Naming: types `PascalCase`, interfaces `I` prefix (`IWindow`), methods `camelCase` verbs, fields `m_`, constants `c_`,
  booleans `is/has/should`, log categories `Log<Module>`, macros `NX_` prefix. Full words, no abbreviations.
- One main type per `.h/.cpp` pair, file name = type name. Headers declare, `.cpp` implements (except templates / header-only types).
- Include paths relative to `Source/`: `#include "Window/IWindow.h"`. Forward declarations in headers where possible.
- Third-party headers stay inside their backend folder (`Window/GLFW/`) and never leak into interfaces.
- Interface + `final` implementation, dependency injection through interfaces, managers own their objects by strong-typed ids.
- Project integer types (`uint32`, `int64`, ...), `enum class` with an explicit underlying type.
- RAII, `std::unique_ptr` by default, no bare `new`/`delete`. Resource owners are non-copyable.
- No exceptions: failures return `std::expected<T, Error>` and are logged.
- Comments only explain *why*. No commented-out code.

## Log

- Category + verbosity macros, like UE's `UE_LOG`: `NX_LOG(LogWindow, Warning, "...", args)`.
- Warning / Error / Fatal always show the source location (file and line) of the call.

## Layout

| Folder | Purpose |
| --- | --- |
| `Engine/Source/` | Engine sources, one folder per module (`Core/`, `Log/`, `Event/`, `Window/`) |
| `Automation/` | Build, clean and format scripts |
| `Config/` | Conan file, `.clang-format` |

Every new file is added to `ENGINE_SOURCES` in `Engine/CMakeLists.txt`.
