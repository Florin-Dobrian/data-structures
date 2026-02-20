# Data Structures

A polyglot playground for exploring data structures and containers across four languages: Python, C++, Rust, and Scala. Each language lives in its own self-contained project with its own build system, so you can open any subfolder independently in its native IDE.

## Repository Layout

```
data-structures/
├── python/          # uv + src layout
│   ├── pyproject.toml
│   ├── main.py
│   └── src/datastructures/
│       └── trackers.py
├── cpp/             # CMake
│   ├── CMakeLists.txt
│   └── src/main.cpp
├── rust/            # Cargo
│   ├── Cargo.toml
│   └── src/main.rs
└── scala/           # sbt
    ├── build.sbt
    └── src/main/scala/
        └── Main.scala
```

## Problems

### 1. Moving Average Tracker

Track the running average of up to the last *n* numbers added. Two implementations per language:

**A — Deque.** Uses each language's standard double-ended queue (`collections.deque`, `std::deque`, `VecDeque`, `mutable.Queue`). When the window is full, pop the oldest value from the front. O(1) amortized.

**B — Circular Buffer.** A fixed-size array with a head index that wraps around via modulo. No allocation after init, no shifting. O(1) guaranteed with better cache locality.

Both maintain a running sum so `get_average` is always O(1).

## Setup

### 1. Create the directory layout

```bash
mkdir -p data-structures/{python,cpp/src,rust,scala/src/main/scala}
```

### 2. Initialize each project

```bash
# Python (uv)
cd data-structures/python
uv init --lib              # creates pyproject.toml + src/ layout
uv venv                    # creates .venv
mv src/python src/datastructures  # rename package
uv pip install -e .                # link src/datastructures into the venv

# Rust (Cargo)
cd ../rust
cargo init --bin           # creates Cargo.toml + src/main.rs

# C++ (CMake)
cd ../cpp
touch src/main.cpp
cat <<EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(datastructures_cpp)
set(CMAKE_CXX_STANDARD 17)
add_executable(datastructures_cpp src/main.cpp)
EOF

# Scala (sbt)
cd ../scala
cat <<EOF > build.sbt
name := "datastructures-scala"
version := "0.1"
scalaVersion := "3.3.0"
EOF
```

## Building and Running

| Language | Directory | Run |
|----------|-----------|-----|
| Python   | `python/` | `cd python && uv run main.py` |
| C++      | `cpp/`    | `cd cpp && cmake -B build && cmake --build build && ./build/datastructures_cpp` |
| Rust     | `rust/`   | `cd rust && cargo run` |
| Scala    | `scala/`  | `cd scala && sbt run` |

### Prerequisites

- **Python**: [uv](https://docs.astral.sh/uv/)
- **C++**: CMake ≥ 3.20, a C++17 compiler
- **Rust**: [rustup](https://rustup.rs/) (edition 2024)
- **Scala**: JDK 11+, [sbt](https://www.scala-sbt.org/)

## Language Notes

Each implementation uses the same imperative, mutable style to keep comparisons fair across languages.

### Python (High-Level / Interpreted)

- **Ease of use.** `collections.deque` with built-in `maxlen` handles eviction automatically, making the deque version the most concise of the four.
- **Abstraction.** Memory management is entirely hidden; the code focuses on readability.
- **Performance paradox.** `(head + 1) % max_size` is relatively fast in Python because the overhead of the interpreter dwarfs the cost of a single modulo.

### C++ (Low-Level / Manual)

- **Hardware sensitivity (modulo).** `%` is an expensive arithmetic operation. If the buffer size is a power of two, `head & (size - 1)` avoids it entirely for a significant speed boost.
- **Memory layout.** `std::deque` is a sequence of non-contiguous blocks; the circular buffer over `std::vector` wins on cache locality.
- **Toolchain.** Requires the most scaffolding (CMake) but allows for the most aggressive optimizations (`-O3`).

### Rust (Safe / Explicit)

- **Memory tuning.** `VecDeque` lives on the heap and can grow, while the circular buffer can be tuned for zero-allocation performance once initialized, staying fixed in memory.
- **Allocation strategy.** `vec![0.0; size]` allocates once on the heap and stays that size forever — a fixed-size heap allocation that is extremely performant.
- **Safety and Option types.** The `if let Some(old_val)` pattern forces the developer to acknowledge that `pop_front()` could return `None`, preventing runtime crashes.
- **Explicit mutation.** Rust requires `&mut self`, making it crystal clear which methods modify internal state.

### Scala (JVM / Hybrid)

- **Style choice.** Uses `mutable.Queue` rather than a functional approach so the comparison stays apples-to-apples across all four languages.
- **JVM lifecycle.** `sbt` has a long cold-start time, but the JIT compiler optimizes the code as it runs.
- **Flexibility.** Scala offers the unique ability to switch to immutable collections if data persistence were a requirement.

### Comparison Matrix

| Feature | Python | C++ | Rust | Scala |
|---------|--------|-----|------|-------|
| **Logic type** | Dynamic / Iterative | Low-level / Pointer-like | Safe / Explicit | JVM / Object-Oriented |
| **Modulo `%`** | Efficiently hidden | Expensive (manual) | Strict | Standard JVM |
| **Memory policy** | Automatic | Manual / RAII | Ownership-based | Garbage collected |
| **Style used** | Imperative | Imperative | Imperative / Safe | Mutable (for parity) |
