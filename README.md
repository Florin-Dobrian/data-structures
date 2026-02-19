Exactly. The goal is four distinct, self-contained projects sitting inside your `data-structures` root. This allows you to open `python/` in PyCharm, `rust/` in RustRover, etc., without the IDEs getting confused by each other's configuration files.

Here is the step-by-step checklist to build the structure from your root directory: `/Users/florin/research/git/data-structures`.

---

### 1. Python (uv)
Python is now shifting toward the "src layout" to match compiled languages.

1.  **Create folder:** `mkdir python && cd python`
2.  **Initialize:** `uv init --lib` (The `--lib` flag sets up a package structure).
3.  **Setup Environment:** `uv venv`
4.  **Structure Check:** Ensure your code goes in `python/src/avg_tracker/` (or similar).
5.  **Clean up:** `rm hello.py` (if `uv` generated it in the root).

---

### 2. Rust (Cargo)
Cargo is the most automated of the four.

1.  **Create folder:** `mkdir rust && cd rust`
2.  **Initialize:** `cargo init --bin` (The `--bin` flag creates an executable with `src/main.rs`).
3.  **Structure Check:** Your code lives in `rust/src/main.rs`.
4.  **Build check:** Run `cargo build` just to ensure the toolchain is happy.

---

### 3. Scala (sbt)
Since there is no "sbt init" command, we do this manually or via IntelliJ.

1.  **Create folders:** `mkdir -p scala/src/main/scala`
2.  **Create config:** Inside `scala/`, create a file named `build.sbt`:
    ```scala
    name := "scala-data-structures"
    version := "0.1"
    scalaVersion := "3.3.1"
    ```
3.  **Create file:** `touch scala/src/main/scala/Main.scala`

---

### 4. C++ (CMake)
Like Scala, this is a manual file-based setup.

1.  **Create folders:** `mkdir -p cpp/src`
2.  **Create config:** Inside `cpp/`, create a file named `CMakeLists.txt`:
    ```cmake
    cmake_minimum_required(VERSION 3.20)
    project(cpp_data_structures)
    set(CMAKE_CXX_STANDARD 17)
    add_executable(avg_tracker src/main.cpp)
    ```
3.  **Create file:** `touch cpp/src/main.cpp`

---

### The Final Folder Tree
Once you finish these tasks, your research repo will look like this:



```text
data-structures/
├── cpp/
│   ├── CMakeLists.txt
│   └── src/
│       └── main.cpp
├── python/
│   ├── pyproject.toml
│   ├── .venv/
│   └── src/
│       └── (your_package)/
├── rust/
│   ├── Cargo.toml
│   └── src/
│       └── main.rs
└── scala/
    ├── build.sbt
    └── src/
        └── main/
            └── scala/
                └── Main.scala
```

### Next Steps for you:
1.  Run the commands above.
2.  Open each folder in its respective IDE.
3.  Paste the **8 implementations** we wrote earlier into the corresponding `src` files.

Would you like me to provide a **global `.gitignore`** for the root of this repo so you don't accidentally commit `target/`, `.venv/`, `.idea/`, and `cmake-build-debug/` folders?

# --- IDEs and Editor metadata ---
.idea/
.vscode/
*.swp
*.swo
.DS_Store

# --- Python (uv / General) ---
.venv/
__pycache__/
*.pyc
.pytest_cache/
.python-version

# --- Rust (Cargo) ---
/rust/target/
# If you run cargo inside the rust folder, this covers it:
target/
Cargo.lock

# --- Scala (sbt) ---
/scala/target/
/scala/project/project/
/scala/project/target/
.bsp/
.scala-build/
*.class

# --- C++ (CMake) ---
/cpp/cmake-build-debug/
/cpp/cmake-build-release/
/cpp/build/
CMakeFiles/
CMakeCache.txt
cmake_install.cmake

# Data Structures: Environment Setup

### 🐍 Python (uv)
cd python
uv init --lib           # Creates project and src/ layout
uv venv                 # Creates the local virtual environment
mv src/python src/ds    # Rename package to 'ds'
# Ensure 'name = "ds"' in pyproject.toml
uv pip install -e .     # Links src/ds to the venv

### 🦀 Rust (Cargo)
cd ../rust
cargo init --bin        # Creates Cargo.toml and src/main.rs

### 🛠️ C++ (CMake)
cd ../cpp
mkdir src
touch src/main.cpp

# Create CMakeLists.txt in /cpp/ with this content:
cat <<EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(ds_research_cpp)
set(CMAKE_CXX_STANDARD 17)
add_executable(ds_cpp src/main.cpp)
EOF

### 📜 Scala (sbt)
cd ../scala
mkdir -p src/main/scala

# Create build.sbt in /scala/ with this content:
cat <<EOF > build.sbt
name := "ds-research-scala"
version := "0.1"
scalaVersion := "3.3.0"
EOF

---

### Final Repository Layout
data-structures/
├── .gitignore          # Global ignore (IDEs, venv, target, build)
├── cpp/
│   ├── CMakeLists.txt
│   └── src/main.cpp
├── python/
│   ├── pyproject.toml
│   ├── .venv/
│   └── src/ds/
├── rust/
│   ├── Cargo.toml
│   └── src/main.rs
└── scala/
    ├── build.sbt
    └── src/main/scala/

# Data Structures Research: Master Setup & Execution Guide

## 1. Environment Initialization

### 🐍 Python (uv)
cd python
uv init --lib
uv venv
mv src/python src/ds
# Set 'name = "ds"' in pyproject.toml
uv pip install -e .

### 🦀 Rust (Cargo)
cd ../rust
cargo init --bin

### 🛠️ C++ (CMake)
cd ../cpp
mkdir src
# Create CMakeLists.txt:
echo 'cmake_minimum_required(VERSION 3.20)
project(ds_research_cpp)
set(CMAKE_CXX_STANDARD 17)
add_executable(ds_cpp src/main.cpp)' > CMakeLists.txt

### 📜 Scala (sbt)
cd ../scala
mkdir -p src/main/scala
# Create build.sbt:
echo 'name := "ds-research-scala"
version := "0.1"
scalaVersion := "3.3.0"' > build.sbt

---

## 2. Implementation & Running

| Language | File Path | Run Command |
| :--- | :--- | :--- |
| **Python** | `python/src/ds/trackers.py` | `uv run main.py` |
| **Rust** | `rust/src/main.rs` | `cargo run` |
| **C++** | `cpp/src/main.cpp` | `cmake -B build && cmake --build build && ./build/ds_cpp` |
| **Scala** | `scala/src/main/scala/Main.scala` | `sbt run` |

---

## 3. The Core Algorithms (Logic Reference)

### Implementation A: Deque (Dynamic)
- **Python**: `collections.deque(maxlen=size)`
- **Rust**: `std::collections::VecDeque`
- **C++**: `std::deque<double>`
- **Scala**: `scala.collection.mutable.Queue`
- **Logic**: Push to back; if size > limit, pop from front. $O(1)$ amortized.

### Implementation B: Circular Buffer (Fixed)
- **Data Structure**: Fixed-size Array/Vector.
- **Logic**: Maintain a `head` index.
- **Update**: `buffer[head] = val; head = (head + 1) % size;`
- **Logic**: No shifting or resizing. Highest cache locality. $O(1)$ guaranteed.

---

## 4. Directory Structure Recap
data-structures/
├── cpp/
│   ├── CMakeLists.txt
│   └── src/main.cpp
├── python/
│   ├── pyproject.toml
│   ├── main.py
│   └── src/ds/trackers.py
├── rust/
│   ├── Cargo.toml
│   └── src/main.rs
└── scala/
    ├── build.sbt
    └── src/main/scala/Main.scala

# Data Structures Research: Comparative Observations (Final)

### 🐍 Python (High-Level / Interpretive)
* **Ease of Use:** Fastest implementation via `collections.deque` with built-in `maxlen`.
* **Abstraction:** Memory management is entirely hidden; logic focuses on readability.
* **Performance Paradox:** Surprisingly, `(head + 1) % max_size` is relatively fast in Python's high-level implementation compared to the overhead of other operations.

### 🛠️ C++ (Low-Level / Manual)
* **Hardware Sensitivity (Modulo):** In C++, the `%` operator is an "expensive" arithmetic operation. For high-performance research, if the buffer size is a power of 2, developers often use bitwise `&` to handle wrap-around for a significant speed boost.
* **Memory Layout:** `std::deque` is a sequence of blocks; our manual **Circular Buffer** (using `std::vector`) is faster due to contiguous memory and better **cache locality**.
* **Toolchain:** Requires the most scaffolding (CMake) but allows for the most aggressive optimizations (`-O3`).

### 🦀 Rust (Safe / Explicit)
* **Memory Tuning:** `VecDeque` lives on the heap and can grow, while our manual **Circular Buffer** can be tuned for zero-allocation performance once initialized, staying fixed in memory.
* **Allocation Strategy:** `vec![0.0; size]` allocates once on the heap and stays that size forever. This "fixed-size heap allocation" is extremely performant.
* **Safety & Option Types:** The `if let Some(old_val)` pattern in the Deque forces the developer to acknowledge that `pop_front()` could return `None`, preventing runtime crashes.
* **Explicit Mutation:** Rust requires `&mut self`, making it crystal clear which methods modify the internal state.

### 📜 Scala (JVM / Hybrid)
* **Style Choice:** We chose a **"Java-like" mutable style** rather than a functional approach. This ensures the research comparison is fair (apples-to-apples) across all four languages by using the same underlying logic.
* **JVM Lifecycle:** `sbt` has a long "cold-start" time, but the JIT (Just-In-Time) compiler optimizes the code as it runs.
* **Flexibility:** While we used a mutable approach for performance parity, Scala offers the unique ability to switch to immutable collections if data persistence were a requirement.

---

### Implementation Matrix Summary

| Feature | Python | C++ | Rust | Scala |
| :--- | :--- | :--- | :--- | :--- |
| **Logic Type** | Dynamic/Iterative | Low-level/Pointer-like | Safe/Explicit | JVM/Object-Oriented |
| **Modulo %** | Efficiently hidden | Expensive (Manual) | Strict | Standard JVM |
| **Memory Policy** | Automatic | Manual/RAII | Ownership-based | Garbage Collected |
| **Style Used** | Imperative | Imperative | Imperative/Safe | Mutable (for Parity) |

