# Project Setup

How this repo was initialized from scratch. These steps have already been done — if you're cloning the repo, you don't need any of this. Just follow the "Configuring" and "Building and Running" sections in the README.

## 1. Create the directory layout

```bash
mkdir -p data-structures/{cpp/src,python,scala/src/main/scala,rust}
```

## 2. Initialize each project

```bash
# C++ (CMake)
cd data-structures/cpp
touch src/main.cpp
cat <<EOF > CMakeLists.txt
cmake_minimum_required(VERSION 3.20)
project(datastructures_cpp)
set(CMAKE_CXX_STANDARD 17)
add_executable(datastructures_cpp src/main.cpp)
EOF

# Python (uv)
cd ../python
uv init --lib              # creates pyproject.toml + src/ layout
uv venv                    # creates .venv
mv src/python src/datastructures  # rename package
uv pip install -e .                # link src/datastructures into the venv
uv add --dev ipykernel             # enable notebook support

# Scala (sbt)
cd ../scala
cat <<EOF > build.sbt
name := "datastructures-scala"
version := "0.1"
scalaVersion := "3.3.0"
EOF

# Rust (Cargo)
cd ../rust
cargo init --bin           # creates Cargo.toml + src/main.rs
```
