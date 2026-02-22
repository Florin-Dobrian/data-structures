# Project Setup

How this repo was initialized from scratch. These steps have already been done — if you're cloning the repo, you don't need any of this. Just follow the "Building and Running" section in the README.

## 1. Create the directory layout

```bash
mkdir -p data-structures/{python,cpp/src,rust,scala/src/main/scala}
```

## 2. Initialize each project

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
