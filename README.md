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
