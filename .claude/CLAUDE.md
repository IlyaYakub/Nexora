# Nexora

A game engine written from scratch. Long-term goal — a Nanite-like renderer.
C++23, CMake, Conan, MSVC. Formatting — `Config/.clang-format`, run `Automation/format_all_files.bat`.
Warnings are errors: `/W4 /WX`, third-party headers are external (`/external:anglebrackets /external:W0`).
Never silence a warning in engine code — fix it.

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
  File-local helpers go into an anonymous namespace at global scope, before `namespace Nexora`, never nested inside it.
- Naming: types `PascalCase`, interfaces `I` prefix (`IWindow`), methods `camelCase` verbs, fields `m_`, constants `c_`,
  booleans `is/has/should`, log categories `Log<Module>`, macros `NX_` prefix. Full words, no abbreviations.
  Names say what they control, not where the value comes from (`c_isValidationEnabled`, not `c_isDebugBuild`).
- One main type per `.h/.cpp` pair, file name = type name. Headers declare, `.cpp` implements (except templates / header-only types).
- Include paths relative to `Source/`: `#include "Window/IWindow.h"`. Forward declarations in headers where possible,
  with the same keyword as the definition (`class IWindowManager;` next to `struct InputEvent;` is fine).
- Standard headers needed by `.cpp` files go into `Source/pch.h` (force-included), not into the `.cpp`.
  Third-party headers stay in the `.cpp` that uses them. Headers (`.h`) keep their own includes.
- Third-party types never leak into interfaces (`.h`); only `.cpp` files use them.
- Interface + `final` implementation, dependency injection through interfaces.
- Managers store their objects as `std::shared_ptr<IObject>` in a `std::unordered_map` keyed by a strong-typed id
  (`WindowId` with a `std::hash` specialization), and hand them out as `std::shared_ptr`, never as a raw pointer from `.get()`.
- Project integer types (`uint32`, `int64`, ...), `enum class` with an explicit underlying type.
  Every value enum starts with `Unknown`, which is also the default of fields of that type and the fallback of mappers.
- RAII, `std::unique_ptr` for single ownership (e.g. `Engine` owning its managers and `Timer`), no bare `new`/`delete`.
  Keep one style within a context: if a class holds its parts as `std::unique_ptr`, every new part follows. Resource owners are non-copyable.
- Each distinct step of a method's logic gets its own named private method instead of an inline expression
  (`removeClosedWindows()` rather than `std::erase_if(...)` inside `update()`).
- Gather first, then use: a method that works with values from members or getters fetches them once into named `const`
  locals in one block at its top (`const VkDevice logicalDevice = m_device.device();`, `const VkFence inFlightFence = frame.inFlightFence();`),
  followed by an empty line, and its steps use only those locals. Values produced by a step (an acquired image index)
  are taken where they appear. A short method that uses each value once needs no such block.
- A type's own resource is created inline in its constructor and released inline in its destructor: no private
  `build()`/`init()`/`destroyX()` helpers for it. A check that is not part of the creation itself stays a private method
  (`checkPresentSupport()`). To recreate such an object, construct a new one (passing the old handle where the API
  reuses it, e.g. `oldSwapchain`) instead of adding a `recreate()` that repeats the constructor.
- Keep headers small: do not grow a class with private helpers or static callbacks just to split code.
  Third-party callbacks are inline lambdas where they are registered (GLFW callbacks in the window constructor),
  working with the raw pointer from the user pointer directly. More code in `.cpp` is better than a bigger `.h`.
- No exceptions. Unrecoverable failures (the engine cannot continue, e.g. window creation) log `Fatal`, which aborts.
  `std::expected<T, ErrorEnum>` only where the caller can actually handle the failure (e.g. a missing asset).
- Store values in the types the consuming API takes (e.g. `int32` for GLFW sizes), so no casts are needed.
- A constructor or factory that takes many parameters takes a settings struct instead (`WindowSettings` for `createWindow`).
- Platform-specific code lives in its own small type and file (`VulkanSurface`), under `#if defined(_WIN32)` etc.,
  so the types using it stay platform-independent.
- Comments in `.h` files: only the required `}  // namespace Nexora` and section labels in the public part of a class.
  After the constructors and destructor, methods are grouped as `// core` (the main functionality), then `// getters`,
  then `// setters`; each group starts with its label, groups are separated by an empty line, empty groups are omitted.
  Method names in a group are aligned in one column (`AlignFunctionDeclarations` in `.clang-format`).
- In `.cpp` files only step labels in the form
  `// <lowercase step> ->` before a block of several lines (`// init ->`, `// destroy window and shutdown ->`).
  Labels do not repeat the type or module the file is about: `// create ->` in `VulkanSwapchain.cpp`,
  not `// create swapchain ->`; no `GLFW` in `GLFWWindow.cpp`.
  One label per logical step, not per statement group: configuring a builder and building it is one step
  (`// create instance ->`), with its groups separated by an empty line.
  Short methods get no labels. No other comments, no commented-out code.
- Empty trailing `//` keep line breaks that clang-format would otherwise join (aligned in one column by `AlignTrailingComments`):
  - a lambda that does not fit on one line gets `//` on every line of its header and body (and after the call's
    preceding argument, so the lambda starts on its own line); lambdas in one call are separated by an empty line;
  - a method chain (builders) is one call per line, `//` after every line but the last:
    `builder` / `.setAppName("Nexora")  //` / `.requireApiVersion(1, 3, 0);`;
  - a constructor initializer list with more than one member is one initializer per line, `//` after every line but the last:
    `: m_windowManager(...),  //` / `m_timer(...),  //` / `m_rhiDevice(...)`;
  - a constructor with 3 or more parameters (declaration and definition, `.h` included) is one parameter
    per line, `//` after every line but the last; clang-format aligns the parameter names in one column
    (other methods keep their parameters on one line while they fit):
    `VulkanSwapchain(const VulkanRHIDevice& device,   //` / `VkSurfaceKHR_T*  surface,  //` / `int32  height);`.
- Any other parameter list that does not fit on one line has one parameter per line, lambdas included
  (enforced by `BinPackParameters: OnePerLine` in `.clang-format`).
- A designated initializer directly in a function body is one field per line with a trailing comma and no `//`
  (`const WindowSettings settings{` / `.width = 1600,` / `};`). Nested deeper (inside a lambda, as an argument)
  it stays on one line: `const InputEvent event{.windowId = self->m_id, .data = data};  //`.

## Simplicity

- Write the simplest code that covers the normal use case. Do not design for edge cases nobody asked for.
- Checks only where skipping them breaks what follows (e.g. a failed resource creation). No defensive checks for misuse.
- No extra overloads or variants "for completeness" (const versions, convenience wrappers) until there is real code that needs them.
- No hidden safety machinery (copies, deferred removal, reentrancy guards) for rare scenarios. Keep the obvious implementation.
- Write as if inputs are (almost) always valid: no range checks or arithmetic tricks, a plain flat `switch` for mappings.

## Log

- Category + verbosity macros, like UE's `UE_LOG`: `NX_LOG(LogWindow, Warning, "...", args)`.
- Warning / Error / Fatal always show the source location (file and line) of the call.
- Messages from third-party libraries (e.g. Vulkan validation) are logged at every severity, each severity mapped to a verbosity
  in a flat `switch`.

## Tests

- GoogleTest, `Engine/Tests/<Module>Tests.cpp`, added to `NexoraEngineTestRunner` in `Engine/CMakeLists.txt`.
- Tests are as short as possible and check only the essentials: one behavior per test, a few lines each.
  Only the normal use case, usually one test per function; no tests for unknown, unsupported or boundary inputs.
- Short does not mean dense: one action per line, intermediate results go into named variables instead of nested calls
  (`auto handle = event.add(...); event.remove(handle);`, not `event.remove(event.add(...));`).
- Naming: `TEST(LogTest, ShouldAcceptOnlyLogCategory)`.

## Layout

| Folder | Purpose |
| --- | --- |
| `Engine/Source/` | Engine sources, one folder per module (`Core/`, `Log/`, `Event/`, `Window/`, `RHI/`, `Renderer/`) |
| `Automation/` | Build, clean and format scripts |
| `Config/` | Conan file, `.clang-format` |

Every new file is added to `ENGINE_SOURCES` in `Engine/CMakeLists.txt`.
