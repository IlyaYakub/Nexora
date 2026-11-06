---
name: design-before-code
description: Mandatory design pass before writing any non-trivial Nexora engine code. Sketch the simplest solution, check whether a design pattern really pays off, check fit with the engine's existing style, and check testability. Use before adding or changing C++ code in Engine/.
---

# Design Before Code

A short, ordered thinking pass that runs **before any code is written**. It produces a small decision record that becomes the plan the user approves (see `CLAUDE.md` > Workflow). Complements `karpathy-guidelines` (simplicity, surgical changes) with an explicit order of decisions.

**Skip it** for trivial edits (typo, rename, one-line fix). Say "trivial, no design pass needed" and continue.

During steps 1-4 write **no code**. Sketch in words or at most a 3-5 line signature outline.

## 1. Simplest solution

- What is the dumbest thing that works? A free function, a plain struct, the standard library, something that already exists in the engine.
- Write 2-4 sentences: what it is, which files it touches.
- This sketch is the **baseline**. It wins unless steps 2-4 give a clear, concrete reason to change it.

## 2. Pattern applicability

Consider a pattern only if it solves a problem that exists **now**, not a hypothetical future one.

- Name the pattern and the concrete pain it removes (duplication, ownership, a swap point with 2+ real implementations today).
- Compare against the baseline: does the code get meaningfully more complex (extra types, files, indirection, virtual calls)?
- **Pattern is a good fit** when it removes real pain and the code stays about as simple as the baseline.
- **Otherwise keep the baseline.** If unsure, keep the baseline.
- Typical fits for this engine: RAII wrapper for owned handles (Vulkan objects), Pimpl to hide heavy headers (as in `Log`), Builder for Vulkan create-info setup. Strategy, Factory, Observer need 2+ real users today. Avoid new singletons.

## 3. Engine fit

Read the neighbouring code first (`Engine/Source/`), then match it. New code should look like it was written by the same author.

- Layout: `Engine/Source/<Module>/Name.h|.cpp`, tests in `Engine/Tests/`. Everything in `namespace Nexora`, `#pragma once`.
- Naming: `PascalCase` types, `camelCase` functions, `m_` members, `c_` constants, `enum class`. Reuse `Core/Types.h` and `Core/Utility.h` (for example `NonCopyable`) instead of redefining them.
- Idioms: C++23, concepts and `static_assert` for compile-time checks, `std::unique_ptr` for ownership, `pch.h` conventions, formatting via `Config/.clang-format`.
- Reuse before creating. If a similar thing exists, extend it or follow its shape.
- If the best design differs from the existing style, say so explicitly and let the user decide.

## 4. Testability

- Can the core logic be tested with GoogleTest without a window, GPU, or global state?
- Prefer pure logic separated from side effects. Pass dependencies in rather than reaching for globals (the `Log` singleton is existing code, not a model for new code).
- State the verification: which test proves it works (for a bug: the test that reproduces it first).
- If something is hard to test, either restructure it or say why not and how it will be verified otherwise.

## 5. Present, then implement

Present a decision record to the user and **wait for approval** before writing code:

```
Goal:        <one line + how it is verified>
Chosen:      <the design in 2-4 sentences>
Rejected:    <alternatives, one line why each>
Pattern:     <name, or "none, baseline kept">
Files:       <added / changed>
Verification: <tests / build commands>
Assumptions: <anything uncertain, questions for the user>
```

After approval, implement carefully:

- Write only what the approved record covers. Do not add features, options, or abstractions that were not agreed.
- Change only what is needed (surgical). Every changed line traces to the plan.
- Mention unrelated problems you notice instead of fixing them.
- Format with `Automation/format_all_files.bat`, build with `Automation/build_debug.bat`, run the tests, and report the real result. If something fails, say so with the output.
- If the work turns out to need a different design than approved, stop and re-present instead of silently deviating.
