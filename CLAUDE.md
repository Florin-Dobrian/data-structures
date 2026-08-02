# CLAUDE.md

## What this repo is

A personal learning and reference resource, not production code. Every problem is implemented in four languages so they can be compared side by side. Optimize for clarity and for surfacing language differences — not for performance, abstraction, or reuse.

See `README.md` for the full problem descriptions and language notes. Don't duplicate that content here.

## Hard conventions

- **Language order is always C++, Python, Scala, Rust.** Everywhere: docs, tables, mains, commit messages, the order you implement in.
- **Two implementations per problem per language.** A and B are the two contrasting implementations, and what the contrast *is* depends on the part. In the discrete problems A uses the standard library and B is the manual or lower-level version. In the numerical kernels it is layout and schedule instead: row against column, CSR against CSC, left-looking against right-looking. Doc comments open with `Implementation A:` / `Implementation B:` and a one-line summary either way.
- **In the numerical kernels the two implementations may take different inputs.** Each takes the layout its loop order walks with stride 1, so agreeing on the output is the check rather than sharing an input.
- **One file per problem per language.** Both implementations live in that single file.
- **Stdlib only.** No third-party dependencies in any of the four projects. `main` is the test harness; there is no test framework in any of them today.
- **Same imperative, mutable style across all four languages**, including Scala. Parity matters more than idiom purity here; the point is that the reader can diff the four files.

## Naming

| | |
|---|---|
| Files (C++, Python, Rust) | `snake_case` — `avg_tracker`, `lru_cache`, `merge_k_sorted` |
| Files (Scala) | `PascalCase` — `AvgTracker`, `LruCache`, `MergeKSorted` |
| Algorithm-shaped problems | Free functions: `first_duplicate_hash_set`. Scala uses `object HashSetFirstDuplicate { def solve(...) }` |
| Stateful problems | Classes/structs named for the mechanism: `OrderedDictLRUCache`, `RankedUnionFind` |
| Projects | `datastructures_cpp`, `datastructures`, `datastructures-scala`, `datastructures-rust` |

## Commands

Run from the repo root; each ends with `&& cd ..`.

```bash
cd cpp && cmake --build build && ./build/datastructures_cpp && cd ..
cd python && uv run main.py && cd ..
cd scala && sbt run && cd ..
cd rust && cargo run && cd ..
```

One-time configure: `cd cpp && cmake -B build && cd ..` and `cd python && uv venv && uv pip install -e . && cd ..`. Scala and Rust self-configure.

## Adding a problem

Work one problem at a time, and confirm all four languages build and run before moving on.

1. Implement in C++, Python, Scala, Rust — in that order.
2. Register the demo in each `main` (`run_xxx()` / `runXxx()`), appended in problem order.
3. Update `README.md`: the repository-layout tree, a new `### N. Problem Name` section under the right part (Discrete Problems or Numerical Kernels) with the two implementations and a **Why it's interesting** paragraph, any new bullets under Language Notes / Cross-Cutting Observations, and the comparison matrix if a new row applies.
4. Update `proposed_problems.md`: the table for that part, and the suggested-order list. Numbering runs straight through both parts.
5. Run all four and check the output.

**Documentation invariant:** all educational content lives in `README.md`. `proposed_problems.md` is a lightweight tracker only — a table and an ordering, no prose explanations. Blurbs have been lost before by living in the tracker and getting dropped during promotion.

## Known language gotchas

- **Scala 3:** non-local `return` inside a `for` comprehension is deprecated and warns. Use a `while` loop with an index instead — see `FirstDuplicate.scala`.
- **Rust:** a `find()` with path compression needs `&mut self`, so two `find()` calls can't appear in one expression. Split them into separate `let` bindings.
- **Heaps:** all four languages default to max-heap except Python's `heapq`. Each has a different flip mechanism (`std::greater`, reversed `Ordering`, `Reverse`).
- **Rust recursive structures:** `Box` when each child has exactly one parent; an index-based node pool when that isn't enough. Never reach for `Rc<RefCell<>>` or `unsafe` here.

## Style

- Prefer catching specific inconsistencies over wholesale rewrites. If something's off in one language, fix that thing.
- Keep documentation current in the same change as the code — not as a follow-up.
- `TODO.md` tracks small cleanup items; move finished ones to its Done section rather than deleting them.
