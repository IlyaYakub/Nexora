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
- Standard headers needed by `.cpp` files go into `Source/pch.h` (force-included), not into the `.cpp`.
  Third-party headers stay in the `.cpp` that uses them. Headers (`.h`) keep their own includes.
- Third-party types never leak into interfaces (`.h`); only `.cpp` files use them.
- Interface + `final` implementation, dependency injection through interfaces.
- Managers store their objects as `std::shared_ptr<IObject>` in a `std::unordered_map` keyed by a strong-typed id
  (`WindowId` with a `std::hash` specialization), and hand them out as `std::shared_ptr`, never as a raw pointer from `.get()`.
- Project integer types (`uint32`, `int64`, ...), `enum class` with an explicit underlying type.
- RAII, `std::unique_ptr` for single ownership (e.g. `main` owning a manager), no bare `new`/`delete`. Resource owners are non-copyable.
- Each distinct step gets its own named private method instead of an inline lambda or expression
  (`removeClosedWindows()` rather than `std::erase_if(...)` inside `update()`).
- No exceptions. Unrecoverable failures (the engine cannot continue, e.g. window creation) log `Fatal`, which aborts.
  `std::expected<T, ErrorEnum>` only where the caller can actually handle the failure (e.g. a missing asset).
- Store values in the types the consuming API takes (e.g. `int32` for GLFW sizes), so no casts are needed.
- Comments: none in `.h` files (only the required `}  // namespace Nexora`). In `.cpp` files only step labels in the form
  `// <lowercase step> ->` before a block of several lines (`// init GLFW ->`, `// destroy windows and shutdown GLFW ->`).
  Short methods get no labels. No other comments, no commented-out code.

## Simplicity

- Write the simplest code that covers the normal use case. Do not design for edge cases nobody asked for.
- Checks only where skipping them breaks what follows (e.g. a failed resource creation). No defensive checks for misuse.
- No extra overloads or variants "for completeness" (const versions, convenience wrappers) until there is real code that needs them.
- No hidden safety machinery (copies, deferred removal, reentrancy guards) for rare scenarios. Keep the obvious implementation.

## Log

- Category + verbosity macros, like UE's `UE_LOG`: `NX_LOG(LogWindow, Warning, "...", args)`.
- Warning / Error / Fatal always show the source location (file and line) of the call.

## Tests

- GoogleTest, `Engine/Tests/<Module>Tests.cpp`, added to `NexoraEngineTestRunner` in `Engine/CMakeLists.txt`.
- Tests are as short as possible and check only the essentials: one behavior per test, a few lines each.
- Short does not mean dense: one action per line, intermediate results go into named variables instead of nested calls
  (`auto handle = event.add(...); event.remove(handle);`, not `event.remove(event.add(...));`).
- Naming: `TEST(LogTest, ShouldAcceptOnlyLogCategory)`.

## Layout

| Folder | Purpose |
| --- | --- |
| `Engine/Source/` | Engine sources, one folder per module (`Core/`, `Log/`, `Event/`, `Window/`) |
| `Automation/` | Build, clean and format scripts |
| `Config/` | Conan file, `.clang-format` |

Every new file is added to `ENGINE_SOURCES` in `Engine/CMakeLists.txt`.
