# TODO

A running list of small cleanup items and loose ends. Nothing here blocks new problems — these are tidy-up tasks to fold into a future pass.

## Cleanup

| # | Item | Location |
|---|------|----------|
| 1 | Remove dead `solvers` val — an empty `List()` with a structural type annotation, never used, left over from an abandoned approach | `scala/src/main/scala/Main.scala`, `runUnionFind` |
| 2 | Replace the `hello()` stub left over from `uv init --lib` (either empty the file or use it to re-export the package's public names) | `python/src/datastructures/__init__.py` |
| 3 | Delete leftover scratch file from problem 1 — not gitignored, unlike the `.ipynb` experiments | `python/test_run.py` |

## Done

| # | Item | Location |
|---|------|----------|
| 4 | `-Wreorder` warning: the initializer list read `max_size(size), buffer(size, 0.0)` while `buffer` was declared first. Fixed by reordering the list to match the declarations. `-Wall -Wextra` added to `cpp/CMakeLists.txt` at the same time, so the next one surfaces when it is introduced | `cpp/src/avg_tracker.h`, `cpp/CMakeLists.txt` |
