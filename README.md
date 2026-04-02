# Data Structures

A polyglot playground for exploring data structures and containers across four languages: C++, Python, Scala, and Rust. Each language lives in its own self-contained project with its own build system, so you can open any subfolder independently in its native IDE.

## Repository Layout

```
data-structures/
├── cpp/                 # CMake
│   ├── CMakeLists.txt
│   └── src/
│       ├── main.cpp
│       ├── avg_tracker.h
│       ├── lru_cache.h
│       ├── next_greater.h
│       ├── merge_k_sorted.h
│       ├── time_kv_store.h
│       └── first_duplicate.h
├── python/              # uv + src layout
│   ├── pyproject.toml
│   ├── main.py
│   └── src/datastructures/
│       ├── avg_tracker.py
│       ├── lru_cache.py
│       ├── next_greater.py
│       ├── merge_k_sorted.py
│       ├── time_kv_store.py
│       └── first_duplicate.py
├── scala/               # sbt
│   ├── build.sbt
│   └── src/main/scala/
│       ├── Main.scala
│       ├── AvgTracker.scala
│       ├── LruCache.scala
│       ├── NextGreater.scala
│       ├── MergeKSorted.scala
│       ├── TimeKvStore.scala
│       └── FirstDuplicate.scala
└── rust/                # Cargo
    ├── Cargo.toml
    └── src/
        ├── main.rs
        ├── avg_tracker.rs
        ├── lru_cache.rs
        ├── next_greater.rs
        ├── merge_k_sorted.rs
        ├── time_kv_store.rs
        └── first_duplicate.rs
```

## Problems

### 1. Moving Average Tracker

Track the running average of up to the last *n* numbers added. Two implementations per language:

**A — Deque.** Uses each language's standard double-ended queue (`std::deque`, `collections.deque`, `mutable.Queue`, `VecDeque`). When the window is full, pop the oldest value from the front. O(1) amortized.

**B — Circular Buffer.** A fixed-size array with a head index that wraps around via modulo. No allocation after init, no shifting. O(1) guaranteed with better cache locality.

Both maintain a running sum so `get_average` is always O(1).

### 2. LRU Cache

A fixed-capacity key-value store that evicts the least recently used entry when full. Two implementations per language:

**A — Standard library.** Uses each language's ordered map (`std::list` + `std::unordered_map` with iterator storage, `OrderedDict`, `mutable.LinkedHashMap`, `HashMap` + `Vec`). Leverages built-in ordering to track access recency.

**B — Manual doubly linked list.** A hash map for O(1) key lookup combined with an index-based doubly linked list (node pool in a vector/array) for O(1) reordering and eviction. Sentinel head and tail nodes eliminate edge cases.

### 3. Next Greater Element (Monotonic Stack)

Given an array, find the next element that is strictly greater for each position. If none exists, return -1. Four implementations per language (two directions × two stack types):

**A — Standard stack** (`std::stack` in C++, `list` in Python, `mutable.Stack` in Scala, `Vec` in Rust):

- **A1 — Right-to-left: "what is my next greater?"** Iterate from the end, maintaining a stack of *values*. For each element, pop anything smaller or equal, then the stack top is the answer. Values suffice because we can fill in each result immediately — we already know what's to the right.
- **A2 — Left-to-right: "whose answer am I?"** Iterate from the start, maintaining a stack of *indices* for elements whose answer hasn't been found yet. When the current element is greater than what's on top, pop and record. Indices are needed because we don't know an element's answer when we first see it.

**B — Manual array-backed stack** (pre-allocated fixed-size array with a top index, no dynamic resizing):

- **B1 — Right-to-left.** Same algorithm as A1, fixed-capacity stack.
- **B2 — Left-to-right.** Same algorithm as A2, fixed-capacity stack.

### 4. Merge K Sorted Lists (Priority Queue / Heap)

Given k sorted arrays, merge them into a single sorted array. Two implementations per language:

**A — Standard library heap.** Uses each language's priority queue (`std::priority_queue` with `std::greater` in C++, `heapq` in Python, `mutable.PriorityQueue` with reversed `Ordering` in Scala, `BinaryHeap` with `Reverse` in Rust). Push `(value, list_index, element_index)` tuples; the smallest value always comes out first. Each pop-and-push is O(log k).

**B — Manual binary min-heap.** A pre-allocated array with sift-up and sift-down operations. Same algorithm, fixed capacity of k (one entry per input list). Shows how a heap works under the hood: parent at `(i-1)/2`, children at `2i+1` and `2i+2`.

### 5. Time-Based Key-Value Store (Sorted Map / Binary Search)

Store key-value pairs with timestamps. Given a key and a timestamp, retrieve the value with the largest timestamp ≤ the query timestamp. Timestamps are inserted in strictly increasing order per key. Two implementations per language:

**A — Standard library sorted map.** Uses each language's sorted/tree map (`std::map` in C++, `bisect` on sorted lists in Python, `mutable.TreeMap` in Scala, `BTreeMap` in Rust). Leverages built-in ordering for O(log n) floor lookups — `upper_bound` and step back in C++, `bisect_right` in Python, `to(t).lastOption` in Scala, `range(..=t).next_back()` in Rust.

**B — Manual binary search.** A hash map where each key maps to a flat append-only list of `(timestamp, value)` pairs. Because timestamps arrive in strictly increasing order, the list stays sorted for free. A hand-written binary search finds the rightmost timestamp ≤ the query. O(1) amortized insert, O(log n) lookup.

### 6. First Duplicate in a Stream (Hash Set)

Given a stream of values, find and return the first value that appears a second time. If no duplicate exists, return -1. Two implementations per language:

**A — Hash set.** Uses each language's hash set (`std::unordered_set` in C++, `set` in Python, `mutable.HashSet` in Scala, `HashSet` in Rust). Insert each value; if it already exists, that's the answer. O(n) expected time. C++, Scala, and Rust all return a boolean from `insert` indicating whether the element was new — no separate lookup needed. Python uses `in` before `add`.

**B — Sorted set.** Uses each language's sorted/tree set (`std::set` in C++, `bisect` on a sorted list in Python, `mutable.TreeSet` in Scala, `BTreeSet` in Rust). Same algorithm, but the underlying container maintains sorted order. Insert and lookup are O(log n) instead of O(1) amortized, making overall time O(n log n). The tradeoff: iteration yields elements in sorted order, and worst-case performance is more predictable (no hash collisions).

## Initial Setup

See [SETUP.md](SETUP.md) for how the repo was initialized from scratch. This was a one-time step and does not need to be repeated.

## Prerequisites

All instructions assume macOS (Intel or Apple Silicon) with zsh.

- **Xcode Command Line Tools**: `xcode-select --install`
- **Homebrew**: `/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"`
- **C++**: CMake ≥ 3.20, a C++17 compiler — `brew install cmake`
- **Python**: [uv](https://docs.astral.sh/uv/) — `curl -LsSf https://astral.sh/uv/install.sh | sh` then `source $HOME/.local/bin/env`
- **Scala**: JDK 11+, [sbt](https://www.scala-sbt.org/) — `brew install openjdk@17 sbt` then:
  ```bash
  echo 'export PATH="/opt/homebrew/opt/openjdk@17/bin:$PATH"' >> ~/.zshrc
  echo 'export JAVA_HOME="/opt/homebrew/opt/openjdk@17"' >> ~/.zshrc
  source ~/.zshrc
  ```
- **Rust**: [rustup](https://rustup.rs/) — `curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh`

## Configuring

After cloning, run once from the repo root:

| Language | Configure |
|----------|-----------|
| C++      | `cd cpp && cmake -B build && cd ..` |
| Python   | `cd python && uv venv && uv pip install -e . && cd ..` |
| Scala    | — handled by `sbt run` |
| Rust     | — handled by `cargo run` |

## Building and Running

| Language | Directory | Run |
|----------|-----------|-----|
| C++      | `cpp/`    | `cd cpp && cmake --build build && ./build/datastructures_cpp && cd ..` |
| Python   | `python/` | `cd python && uv run main.py && cd ..` |
| Scala    | `scala/`  | `cd scala && sbt run && cd ..` |
| Rust     | `rust/`   | `cd rust && cargo run && cd ..` |

The Python project includes `ipykernel` as a dev dependency, so you can experiment interactively with any of the code in PyCharm notebooks, Jupyter, or JupyterLab — just create an `.ipynb` file in the `python/` directory and import from `datastructures`.

## Language Notes

Each implementation uses the same imperative, mutable style to keep comparisons fair across languages.

### C++ (Low-Level / Manual)

- **Hardware sensitivity (modulo).** `%` is an expensive arithmetic operation. If the circular buffer size is a power of two, `head & (size - 1)` avoids it entirely for a significant speed boost.
- **Memory layout.** `std::deque` is a sequence of non-contiguous blocks; the circular buffer over `std::vector` wins on cache locality.
- **Iterator storage.** C++ is the only language where the stdlib LRU cache (`std::list` + `std::unordered_map`) stores iterators directly into the linked list, giving true O(1) splice. The other languages either lack stable iterators or don't expose them.
- **Dedicated containers.** C++ has `std::stack` as a dedicated adapter, while Python and Rust just use their dynamic arrays. `std::priority_queue` requires a comparator template parameter (`std::greater`) to flip from max-heap to min-heap.
- **Sorted map floor lookups.** `std::map::upper_bound` returns the first element strictly greater than the query; stepping the iterator back one gives the floor entry. This two-step pattern is idiomatic but easy to get wrong at boundaries.
- **Insert-returns-boolean.** Both `std::unordered_set::insert` and `std::set::insert` return a `pair<iterator, bool>` where `.second` indicates whether the element was new — no separate `find` needed for the first-duplicate problem.
- **Toolchain.** Requires the most scaffolding (CMake) but allows for the most aggressive optimizations (`-O3`).

### Python (High-Level / Interpreted)

- **Ease of use.** `collections.deque` with built-in `maxlen` handles eviction automatically, making the deque-based moving average the most concise of all four languages.
- **OrderedDict.** Python's `OrderedDict` with `move_to_end` and `popitem(last=False)` makes the stdlib LRU cache a near-one-liner — the cleanest implementation in the repo.
- **No sorted containers in stdlib.** Python is the consistent outlier for sorted data structures. There is no sorted map or sorted set, so the time-based KV store uses `bisect` on parallel sorted lists, and the first-duplicate sorted-set variant uses `bisect_left` + `insort` on a plain list. The `insort` approach is O(n) per insertion due to shifting, giving O(n²) worst case — a real penalty at scale.
- **Tuple-based heap ordering.** `heapq` compares tuples lexicographically, so `(value, list_index, element_index)` gives natural min-heap behavior for free — no custom comparator needed.
- **Set uses `in` before `add`.** Unlike C++, Scala, and Rust where `insert` returns a boolean, Python's `set.add` returns `None`, so the first-duplicate solution checks `val in seen` before adding.
- **Performance paradox.** `(head + 1) % max_size` is relatively fast in Python because the overhead of the interpreter dwarfs the cost of a single modulo.

### Scala (JVM / Hybrid)

- **Style choice.** Uses `mutable.Queue`, `mutable.LinkedHashMap`, `mutable.Stack`, `mutable.PriorityQueue`, `mutable.TreeMap`, `mutable.HashSet`, and `mutable.TreeSet` rather than functional approaches so the comparison stays apples-to-apples across all four languages.
- **Reversed Ordering for min-heap.** Scala's `mutable.PriorityQueue` is a max-heap by default. Flipping to min-heap requires passing a reversed `Ordering`, which is more verbose than Rust's `Reverse` wrapper but more flexible for complex key types.
- **TreeMap floor lookups.** `tsMap.to(timestamp).lastOption` returns a view of all entries up to and including the key, then grabs the last one. Clean and readable, but creates an intermediate view.
- **Insert-returns-boolean.** Both `mutable.HashSet.add` and `mutable.TreeSet.add` return `true` if the element was new, `false` if it already existed — same pattern as C++ and Rust.
- **JVM lifecycle.** `sbt` has a long cold-start time, but the JIT compiler optimizes hot paths as the code runs.
- **Flexibility.** Scala offers the unique ability to switch to immutable collections if data persistence were a requirement.

### Rust (Safe / Explicit)

- **Memory tuning.** `VecDeque` lives on the heap and can grow, while the circular buffer can be tuned for zero-allocation performance once initialized, staying fixed in memory.
- **Allocation strategy.** `vec![0.0; size]` allocates once on the heap and stays that size forever — a fixed-size heap allocation that is extremely performant.
- **Index-based linked list.** Rust's ownership model makes pointer-based doubly linked lists painful (`Rc<RefCell<>>` or `unsafe`). The index-based node pool with sentinel nodes sidesteps this entirely — the same approach used in C++, but in Rust it's the *only* ergonomic option.
- **Reverse wrapper for min-heap.** `BinaryHeap` is a max-heap by default. Wrapping entries in `std::cmp::Reverse` is the idiomatic flip — simpler than C++'s template comparator or Scala's reversed `Ordering`.
- **BTreeMap floor lookups.** `range(..=timestamp).next_back()` returns an iterator over all entries up to and including the key, then grabs the last one. The range syntax `..=` (inclusive) is a Rust-specific feature that makes this very readable.
- **Insert-returns-boolean.** Both `HashSet::insert` and `BTreeSet::insert` return `true` if the value was new — the same pattern as C++ and Scala, making the first-duplicate code nearly identical across three of the four languages.
- **Safety and Option types.** The `if let Some(...)` pattern forces the developer to acknowledge that operations like `pop_front()` or `range().next_back()` could return `None`, preventing runtime crashes.
- **Explicit mutation.** Rust requires `&mut self`, making it crystal clear which methods modify internal state.

### Cross-Cutting Observations

- **All four languages default to max-heaps** (problem 4). The flip mechanisms differ: C++ uses a comparator template parameter (`std::greater`), Python's `heapq` is already a min-heap, Scala reverses the `Ordering`, and Rust wraps in `Reverse`. Python is the only one that defaults to min-heap.
- **Sorted-append trick** (problem 5). When timestamps arrive in strictly increasing order, the manual implementation across all four languages is nearly identical: append to a flat list and binary-search on lookup. The sorted-map implementation is where languages diverge — each spells the floor lookup differently.
- **API convergence on sets** (problem 6). C++, Scala, and Rust all offer `insert`-returns-boolean on both hash and sorted sets, making their first-duplicate implementations nearly identical. Python stands apart with its `in` + `add` pattern.
- **Python's stdlib gaps.** Python is the consistent outlier for sorted containers. Problems 5 and 6 both require workarounds (`bisect` on plain lists) where the other three languages have dedicated sorted map and sorted set types.

### Comparison Matrix

| Feature | C++ | Python | Scala | Rust |
|---------|-----|--------|-------|------|
| **Logic type** | Low-level / Pointer-like | Dynamic / Iterative | JVM / Object-Oriented | Safe / Explicit |
| **Modulo `%`** | Expensive (manual) | Efficiently hidden | Standard JVM | Strict |
| **Memory policy** | Manual / RAII | Automatic | Garbage collected | Ownership-based |
| **Heap default** | Max (flip via `std::greater`) | Min (`heapq`) | Max (flip via reversed `Ordering`) | Max (flip via `Reverse`) |
| **Sorted map** | `std::map` (red-black tree) | None — `bisect` on lists | `mutable.TreeMap` (red-black tree) | `BTreeMap` (B-tree) |
| **Sorted set** | `std::set` (red-black tree) | None — `bisect` on lists | `mutable.TreeSet` (red-black tree) | `BTreeSet` (B-tree) |
| **Set insert API** | `pair<iter, bool>` | `None` (`in` + `add`) | `Boolean` | `bool` |
| **Style used** | Imperative | Imperative | Mutable (for parity) | Imperative / Safe |
