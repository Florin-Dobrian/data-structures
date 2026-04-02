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
│       └── time_kv_store.h
├── python/              # uv + src layout
│   ├── pyproject.toml
│   ├── main.py
│   └── src/datastructures/
│       ├── avg_tracker.py
│       ├── lru_cache.py
│       ├── next_greater.py
│       ├── merge_k_sorted.py
│       └── time_kv_store.py
├── scala/               # sbt
│   ├── build.sbt
│   └── src/main/scala/
│       ├── Main.scala
│       ├── AvgTracker.scala
│       ├── LruCache.scala
│       ├── NextGreater.scala
│       ├── MergeKSorted.scala
│       └── TimeKvStore.scala
└── rust/                # Cargo
    ├── Cargo.toml
    └── src/
        ├── main.rs
        ├── avg_tracker.rs
        ├── lru_cache.rs
        ├── next_greater.rs
        ├── merge_k_sorted.rs
        └── time_kv_store.rs
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

- **Hardware sensitivity (modulo).** `%` is an expensive arithmetic operation. If the buffer size is a power of two, `head & (size - 1)` avoids it entirely for a significant speed boost.
- **Memory layout.** `std::deque` is a sequence of non-contiguous blocks; the circular buffer over `std::vector` wins on cache locality.
- **Toolchain.** Requires the most scaffolding (CMake) but allows for the most aggressive optimizations (`-O3`).

### Python (High-Level / Interpreted)

- **Ease of use.** `collections.deque` with built-in `maxlen` handles eviction automatically, making the deque version the most concise of the four.
- **Abstraction.** Memory management is entirely hidden; the code focuses on readability.
- **Performance paradox.** `(head + 1) % max_size` is relatively fast in Python because the overhead of the interpreter dwarfs the cost of a single modulo.

### Scala (JVM / Hybrid)

- **Style choice.** Uses `mutable.Queue` rather than a functional approach so the comparison stays apples-to-apples across all four languages.
- **JVM lifecycle.** `sbt` has a long cold-start time, but the JIT compiler optimizes the code as it runs.
- **Flexibility.** Scala offers the unique ability to switch to immutable collections if data persistence were a requirement.

### Rust (Safe / Explicit)

- **Memory tuning.** `VecDeque` lives on the heap and can grow, while the circular buffer can be tuned for zero-allocation performance once initialized, staying fixed in memory.
- **Allocation strategy.** `vec![0.0; size]` allocates once on the heap and stays that size forever — a fixed-size heap allocation that is extremely performant.
- **Safety and Option types.** The `if let Some(old_val)` pattern forces the developer to acknowledge that `pop_front()` could return `None`, preventing runtime crashes.
- **Explicit mutation.** Rust requires `&mut self`, making it crystal clear which methods modify internal state.

### Comparison Matrix

| Feature | C++ | Python | Scala | Rust |
|---------|-----|--------|-------|------|
| **Logic type** | Low-level / Pointer-like | Dynamic / Iterative | JVM / Object-Oriented | Safe / Explicit |
| **Modulo `%`** | Expensive (manual) | Efficiently hidden | Standard JVM | Strict |
| **Memory policy** | Manual / RAII | Automatic | Garbage collected | Ownership-based |
| **Style used** | Imperative | Imperative | Mutable (for parity) | Imperative / Safe |
