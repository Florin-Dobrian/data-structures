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
│       ├── first_duplicate.h
│       ├── prefix_trie.h
│       ├── union_find.h
│       ├── dense_matvec.h
│       ├── sparse_matvec.h
│       ├── dense_lu_factor.h
│       └── dense_lu_solve.h
├── python/              # uv + src layout
│   ├── pyproject.toml
│   ├── main.py
│   └── src/datastructures/
│       ├── avg_tracker.py
│       ├── lru_cache.py
│       ├── next_greater.py
│       ├── merge_k_sorted.py
│       ├── time_kv_store.py
│       ├── first_duplicate.py
│       ├── prefix_trie.py
│       ├── union_find.py
│       ├── dense_matvec.py
│       ├── sparse_matvec.py
│       ├── dense_lu_factor.py
│       └── dense_lu_solve.py
├── scala/               # sbt
│   ├── build.sbt
│   └── src/main/scala/
│       ├── Main.scala
│       ├── AvgTracker.scala
│       ├── LruCache.scala
│       ├── NextGreater.scala
│       ├── MergeKSorted.scala
│       ├── TimeKvStore.scala
│       ├── FirstDuplicate.scala
│       ├── PrefixTrie.scala
│       ├── UnionFind.scala
│       ├── DenseMatvec.scala
│       ├── SparseMatvec.scala
│       ├── DenseLuFactor.scala
│       └── DenseLuSolve.scala
└── rust/                # Cargo
    ├── Cargo.toml
    └── src/
        ├── main.rs
        ├── avg_tracker.rs
        ├── lru_cache.rs
        ├── next_greater.rs
        ├── merge_k_sorted.rs
        ├── time_kv_store.rs
        ├── first_duplicate.rs
        ├── prefix_trie.rs
        ├── union_find.rs
        ├── dense_matvec.rs
        ├── sparse_matvec.rs
        ├── dense_lu_factor.rs
        └── dense_lu_solve.rs
```

## Discrete Problems

### 1. Moving Average Tracker

Track the running average of up to the last *n* numbers added. Two implementations per language:

**A — Deque.** Uses each language's standard double-ended queue (`std::deque`, `collections.deque`, `mutable.Queue`, `VecDeque`). When the window is full, pop the oldest value from the front. O(1) amortized.

**B — Circular Buffer.** A fixed-size array with a head index that wraps around via modulo. No allocation after init, no shifting. O(1) guaranteed with better cache locality.

Both maintain a running sum so `get_average` is always O(1).

**Why it's interesting:** The deque and circular buffer solve the same problem with very different memory strategies. The deque is dynamically sized and handled by the standard library; the circular buffer is a fixed-size array with modulo arithmetic. This contrast surfaces hardware-level differences: cache locality, allocation cost, and whether `%` is worth worrying about (it is in C++, it isn't in Python).

### 2. LRU Cache

A fixed-capacity key-value store that evicts the least recently used entry when full. Two implementations per language:

**A — Standard library.** Uses each language's ordered map (`std::list` + `std::unordered_map` with iterator storage, `OrderedDict`, `mutable.LinkedHashMap`, `HashMap` + `Vec`). Leverages built-in ordering to track access recency.

**B — Manual doubly linked list.** A hash map for O(1) key lookup combined with an index-based doubly linked list (node pool in a vector/array) for O(1) reordering and eviction. Sentinel head and tail nodes eliminate edge cases.

**Why it's interesting:** The stdlib version reveals how differently each language handles ordered maps — C++ stores iterators, Python has `OrderedDict`, Scala has `LinkedHashMap`, and Rust has no ordered map at all (it falls back to `HashMap` + `Vec` with O(n) moves). The manual version is the repo's first index-based linked list, a pattern that becomes essential in Rust where pointer-based doubly linked lists hit ownership walls.

### 3. Next Greater Element (Monotonic Stack)

Given an array, find the next element that is strictly greater for each position. If none exists, return -1. Four implementations per language (two directions × two stack types):

**A — Standard stack** (`std::stack` in C++, `list` in Python, `mutable.Stack` in Scala, `Vec` in Rust):

- **A1 — Right-to-left: "what is my next greater?"** Iterate from the end, maintaining a stack of *values*. For each element, pop anything smaller or equal, then the stack top is the answer. Values suffice because we can fill in each result immediately — we already know what's to the right.
- **A2 — Left-to-right: "whose answer am I?"** Iterate from the start, maintaining a stack of *indices* for elements whose answer hasn't been found yet. When the current element is greater than what's on top, pop and record. Indices are needed because we don't know an element's answer when we first see it.

**B — Manual array-backed stack** (pre-allocated fixed-size array with a top index, no dynamic resizing):

- **B1 — Right-to-left.** Same algorithm as A1, fixed-capacity stack.
- **B2 — Left-to-right.** Same algorithm as A2, fixed-capacity stack.

**Why it's interesting:** The monotonic stack is a pattern that shows up everywhere (stock span, histogram area, temperature problems). Each language handles stack-like usage differently — Python and Rust just use their dynamic arrays, while C++ and Scala have dedicated stack types. The right-to-left vs. left-to-right contrast illuminates a deeper algorithmic insight: whether you push values or indices depends on whether you know the answer when you visit an element or only discover it later.

### 4. Merge K Sorted Lists (Priority Queue / Heap)

Given k sorted arrays, merge them into a single sorted array. Two implementations per language:

**A — Standard library heap.** Uses each language's priority queue (`std::priority_queue` with `std::greater` in C++, `heapq` in Python, `mutable.PriorityQueue` with reversed `Ordering` in Scala, `BinaryHeap` with `Reverse` in Rust). Push `(value, list_index, element_index)` tuples; the smallest value always comes out first. Each pop-and-push is O(log k).

**B — Manual binary min-heap.** A pre-allocated array with sift-up and sift-down operations. Same algorithm, fixed capacity of k (one entry per input list). Shows how a heap works under the hood: parent at `(i-1)/2`, children at `2i+1` and `2i+2`.

**Why it's interesting:** Custom comparators work very differently across the four languages. Python uses tuples for natural ordering, C++ uses a comparator template parameter, Rust wraps in `Reverse`, and Scala passes an `Ordering`. All four languages default to max-heaps except Python — the flip mechanisms are where the real language contrast lives.

### 5. Time-Based Key-Value Store (Sorted Map / Binary Search)

Store key-value pairs with timestamps. Given a key and a timestamp, retrieve the value with the largest timestamp ≤ the query timestamp. Timestamps are inserted in strictly increasing order per key. Two implementations per language:

**A — Standard library sorted map.** Uses each language's sorted/tree map (`std::map` in C++, `bisect` on sorted lists in Python, `mutable.TreeMap` in Scala, `BTreeMap` in Rust). Leverages built-in ordering for O(log n) floor lookups — `upper_bound` and step back in C++, `bisect_right` in Python, `to(t).lastOption` in Scala, `range(..=t).next_back()` in Rust.

**B — Manual binary search.** A hash map where each key maps to a flat append-only list of `(timestamp, value)` pairs. Because timestamps arrive in strictly increasing order, the list stays sorted for free. A hand-written binary search finds the rightmost timestamp ≤ the query. O(1) amortized insert, O(log n) lookup.

**Why it's interesting:** Exercises ordered map operations — specifically floor/ceiling lookups. C++, Rust, and Scala all have built-in tree maps; Python doesn't, so you use `bisect` on sorted lists. Good contrast between languages with rich stdlib sorted collections and those without. The manual version is nearly identical across all four languages because the sorted-append trick sidesteps the usual "keep it sorted" problem entirely.

### 6. First Duplicate in a Stream (Hash Set)

Given a stream of values, find and return the first value that appears a second time. If no duplicate exists, return -1. Two implementations per language:

**A — Hash set.** Uses each language's hash set (`std::unordered_set` in C++, `set` in Python, `mutable.HashSet` in Scala, `HashSet` in Rust). Insert each value; if it already exists, that's the answer. O(n) expected time. C++, Scala, and Rust all return a boolean from `insert` indicating whether the element was new — no separate lookup needed. Python uses `in` before `add`.

**B — Sorted set.** Uses each language's sorted/tree set (`std::set` in C++, `bisect` on a sorted list in Python, `mutable.TreeSet` in Scala, `BTreeSet` in Rust). Same algorithm, but the underlying container maintains sorted order. Insert and lookup are O(log n) instead of O(1) amortized, making overall time O(n log n). The tradeoff: iteration yields elements in sorted order, and worst-case performance is more predictable (no hash collisions).

**Why it's interesting:** The simplest exercise in the repo, but it earns its place because hash sets are used constantly and the APIs differ. C++, Scala, and Rust all return a boolean from `insert`; Python's `set.add` returns `None`. Pairing hash set with sorted set highlights the ordered-vs-unordered tradeoff and reveals Python's stdlib gap — no sorted set, so `bisect` + `insort` on a list gives O(n²) worst case.

### 7. Prefix Trie (Custom Tree Structure)

Implement insert, search (exact match), and starts-with (prefix matching) on a collection of strings. No standard trie exists in any of the four languages — both implementations are built from scratch. Two implementations per language:

**A — Hash map trie (flexible alphabet).** Each node stores children in a hash map (`std::unordered_map<char, unique_ptr<Node>>` in C++, `dict` in Python, `mutable.HashMap` in Scala, `HashMap<u8, Box<Node>>` in Rust). Supports any character set without wasting memory on unused slots. O(L) per operation where L is the word length.

**B — Fixed-array trie (lowercase ASCII only).** Each node stores children in a fixed array of 26 slots, one per letter a–z. Lookup is a direct index (`c - 'a'`), trading memory (26 slots per node regardless of usage) for speed (no hashing, no collisions). In Rust, the array version uses an index-based node pool (the same pattern as the LRU cache) rather than `Box`, sidestepping ownership constraints entirely.

**Why it's interesting:** Forces you to build a recursive/nested mutable structure from scratch — no language has a standard trie. This is trivial in Python (nested dicts) and Scala (nested mutable maps), straightforward in C++ (`unique_ptr`), and most educational in Rust: the hash map version uses `Box` for exclusive ownership, while the array version uses an index-based node pool. The trie is the clearest example in the repo of how Rust's ownership model shapes data structure design.

### 8. Connected Components (Union-Find / Disjoint Set)

Given n nodes and a list of edges, count the number of connected components. Two implementations per language:

**A — Naive union-find (simple parent tracking).** Each node points to its parent; the root points to itself. `find()` walks up the parent chain. `unite()` links one root to the other with no balancing. Worst case: the tree degenerates into a linked list, making `find()` O(n).

**B — Optimized union-find (path compression + union by rank).** Path compression repoints every node along the `find()` path directly to the root, flattening the tree for future lookups. Union by rank attaches the shorter tree under the taller one, preventing degenerate chains. Together, these give nearly O(1) amortized per operation — formally O(α(n)) where α is the inverse Ackermann function.

**Why it's interesting:** The data structure is just a flat array, so the implementation is nearly identical across all four languages. The value is in seeing how each language handles the mutable array manipulation — especially Rust, where `find()` with path compression requires `&mut self` even though it's logically a query, because it mutates the parent array for performance. This is the clearest example of how Rust makes internal mutation explicit.

## Numerical Kernels

### 9. Dense Matrix-Vector Product (Memory Layout)

Compute `y = A x` for a dense m x n matrix. The arithmetic is fixed and trivial; what varies is the order the entries are visited in and the layout they are stored in. Unlike problems 1-8, the two implementations do not share an input: each takes the matrix in the layout its loop order wants, and the check is that both produce the same `y`.

**A — Row-oriented (gather).** `RowDenseMatrix` stores `val` row by row, so row `i` occupies a contiguous run beginning at `rp = i * nSize`. Each `y[i]` is the dot product of row `i` with `x`, accumulated in a local and written exactly once. The inner loop reads all of `x`.

**B — Column-oriented (scatter).** `ColDenseMatrix` stores `val` column by column, so column `j` begins at `cp = j * mSize`. Each column, scaled by `x[j]`, is added into the whole of `y`. `y` starts at zero and accumulates, so no entry is final until the last column is processed. The inner loop reads `x[j]` once and touches all of `y`.

Both walk their storage with stride 1. `rp` and `cp` are named for the arrays they would index into in the sparse version: `rowPtr` and `colPtr`.

**Why it's interesting:** The first problem where the container is a flat buffer and the interesting choice is what the indices *mean* rather than which container to reach for. Two things come out of it. The gather/scatter pair is the same distinction that runs through numerical linear algebra generally, where it appears as left-looking versus right-looking factorization, so meeting it on a two-line kernel is worth doing. And it exposes an integer-type question the earlier problems never raised: an index that gets *stored* wants to be 32-bit, since an index array is large and the cap is acceptable, while a loop counter is ephemeral and should just be whatever the language subscripts with. Dense matvec stores no indices at all, so all four languages come out with no casts anywhere; problem 10 is where the distinction bites. C++ and Rust also diverge on a smaller point: Rust has no implicit numeric conversion, so any convention forcing a non-`usize` counter would cost `for i in 0..n` and turn every loop into a hand-driven `while`, where C++ would pay only one cast in the loop condition.

### 10. Sparse Matrix-Vector Product (CSR / CSC)

The same `y = A x`, with the zeros no longer stored. Only the nonzeros are held, in three flat arrays: a pointer array marking where each row or column begins, an index array giving the row or column of each stored entry, and the values. As in problem 9, the two implementations take different inputs and agreeing on `y` is the check.

**A — CSR, Compressed Sparse Row (gather).** `rowPtr` has `mSize + 1` entries and row `i` occupies positions `rowPtr[i]` up to `rowPtr[i + 1]`; `colIdx` gives each entry's column. Same shape as the dense gather, with one change: the row begins at `rowPtr[i]` rather than `i * nSize`, and its entries are scattered across `x`, so each column has to be read from `colIdx`. That indirection, `x.val[colIdx[rp]]`, is the whole difference between a dense and a sparse kernel.

**B — CSC, Compressed Sparse Column (scatter).** The mirror. `colPtr` bounds each column, `rowIdx` gives each entry's row, and the indirection moves from the read to the write: `y.val[rowIdx[cp]] += ...` jumps, while `x[j]` is read once per column. This is why a scatter is the harder one to parallelize — two columns can write the same entry of `y`.

The same matrix in the two formats holds the same nonzeros in a different order, so `val` differs between them.

**Why it's interesting:** This is where the index/position distinction stops being vocabulary and starts costing something. `rowPtr` and `colPtr` hold positions and are `std::size_t` / `usize`; `colIdx` and `rowIdx` hold indices and are `std::int32_t` / `i32`, because an index array is as long as the matrix has nonzeros and its width is a first-order memory cost. Decoupling the two also buys range: 32-bit indices cap the dimension at 2^31, but nnz is bounded by n², so positions need roughly 2^62 to cover what those indices can address. See `NOTES.md` for the full argument, including why Scala cannot express this at all and what that costs.

The four languages then diverge on what the crossing between the two types costs. C++ subscripts with an `int32_t` silently, so the line carries no cast. Rust has no implicit numeric conversion, so it reads `x.val[a.col_idx[rp] as usize]` — one cast, in exactly the place where the distinction is real. Scala and Python have one integer type each doing both jobs, so the split survives only in the naming.

It is also the first problem with a cross-problem dependency: `Vector` is defined in problem 9 and reused here rather than redefined, which each language spells differently — an `#include` in C++, a module import in Python, nothing at all in Scala (same package), and a crate path in Rust.


### 11. Dense LU Factorization (Left-Looking / Right-Looking)

Factor a square dense `A` into `A = L U`, with `L` unit lower triangular and `U` upper triangular. No pivoting: the demo matrix is built as the product of integer `L` and `U`, so its pivots are exactly `U`'s diagonal and the whole factorization comes out in exact integers. Column-oriented storage only.

Both implementations write into one buffer. The factor is a copy of `A` overwritten in place, leaving `U` in the upper triangle and `L`'s multipliers in the strict lower triangle; `L`'s unit diagonal is implied and never stored. So the factor's type is just `ColDenseMatrix`, reused from problem 9 — the two triangles need no separate storage, and the return type says nothing a variable name doesn't.

**A — Left-looking (gather).** Column `k` waits until it is reached, then pulls in every earlier column's contribution in one pass, and only then divides its `L` part by the pivot. When the pass begins nothing to the right of column `k` has been touched; when it ends, column `k` is finished.

**B — Right-looking (scatter).** The moment column `k`'s multipliers are known they push a rank-1 update into the entire trailing submatrix. Column `k` finishes early and everything right of it is partially updated. That update is the outer product of column `k`'s multipliers with row `k` of `U`, which is where a real implementation calls a dense GEMM.

**Why it's interesting:** The same gather and scatter pair as the matvec, on an operation where it actually changes the shape of the program. In the matvec, gather and scatter differ only in loop order. Here they differ in *when a value becomes final*: left-looking finishes one column at a time and leaves the rest untouched, right-looking finishes a column immediately and leaves partial results everywhere. Both compute the same `L` and `U`, and on this matrix they agree bitwise because every value is an exactly representable integer; on real input they would differ in the last bits, since the same contributions are summed in different orders.

This is also the scheduling distinction that runs through sparse direct solvers, where left-looking and right-looking factorizations are two of the standard traversals. Meeting it on a dense 4x4 with no pivoting and no symbolic phase is the cheapest place to see it.

The languages barely diverge here, which is itself informative: the kernel is index arithmetic on a flat buffer, so all four read nearly the same. The one visible difference is the copy that preserves `A`. C++ needs no helper at all, since `ColDenseMatrix lu = a;` copies; Python, Scala and Rust each need an explicit one, because assignment binds a reference.


### 12. Dense LU Solve (Forward / Back Substitution)

With `A = L U`, solving `A x = b` is two triangular passes: forward `L y = b`, then back `U x = y`. Both run against the combined factor problem 11 produces, so `L`'s unit diagonal is implied and never stored — the forward pass therefore never divides, and the back pass divides by the diagonal, which belongs to `U`.

**A — Row-oriented (gather).** Each unknown is finished the moment it is reached, by subtracting the dot product of the unknowns already solved. Nothing is written twice. Takes the factor row-major, so both inner loops scan a row with stride 1.

**B — Column-oriented (scatter).** Each unknown, once solved, is pushed into every equation that still needs it. The target starts as a copy of the right-hand side and is written many times; no entry is final until its own column is reached. Takes the factor column-major.

Problem 11 produces only the column-major factor, so the row-major one is taken as given here — the subject is the solve, not the factorization. A row-oriented factorization would be a natural addition later.

**Why it's interesting:** Four loop nests, two per implementation, and they close the gather/scatter pair that opened with the matvec. The asymmetry worth noticing is where the division sits: the gather divides `x[i]` *after* its dot product is complete, while the scatter divides `x[j]` *before* scattering it, because a value has to be final before it can be pushed. Everything else about the two is a clean mirror image; that one step is not, and it is the easiest thing here to get wrong.

It is also the problem where the four languages differ most visibly on something entirely mundane: the descending loop. Python writes `range(n - 1, -1, -1)`, Rust `(0..n).rev()`, C++ `for (std::size_t i = n; i-- > 0;)` because an unsigned counter cannot test against zero from above, and Scala decrements a `var` by hand. One loop, four spellings, and each follows from what the language takes a range to be.


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
- **Recursive ownership via `unique_ptr`.** The trie uses `unique_ptr<Node>` for children, giving automatic recursive cleanup when a node is destroyed. This is the most natural C++ approach — straightforward, no manual `delete`, and no shared ownership needed.
- **Descending loops with an unsigned counter.** `i >= 0` is always true for a `std::size_t`, so a downward loop uses `for (std::size_t i = n; i-- > 0;)`, which enters with `i = n-1` and exits after `i = 0`. This is the one place the unsigned choice costs an idiom rather than nothing, and it is exactly why some codebases prefer signed indices throughout.
- **Copying is a declaration.** `ColDenseMatrix lu = a;` copies the whole object, buffer included, because the implicitly generated copy constructor does memberwise copy and `std::vector` copies its contents. C++ is the only one of the four where preserving `A` before factoring needs no helper function and no method call.
- **Subscripting with a stored index needs no cast.** In `x.val[a.colIdx[rp]]` the `colIdx` entry is an `std::int32_t` and `operator[]` wants a `std::size_t`, and the conversion is implicit and silent under `-Wall -Wextra`. This is what makes the index/position split cheap to carry in C++: the types differ where it matters, and the crossing costs nothing to write.
- **Implicit numeric conversion.** In `a.val[rp + j]` the `std::size_t` and the loop counter mix freely: the usual arithmetic conversions promote silently, so index arithmetic needs no cast even when the two operands have different integer types. This is what makes a signed-index convention cheap in C++ and expensive in Rust.
- **Toolchain.** Requires the most scaffolding (CMake) but allows for the most aggressive optimizations (`-O3`).

### Python (High-Level / Interpreted)

- **Ease of use.** `collections.deque` with built-in `maxlen` handles eviction automatically, making the deque-based moving average the most concise of all four languages.
- **OrderedDict.** Python's `OrderedDict` with `move_to_end` and `popitem(last=False)` makes the stdlib LRU cache a near-one-liner — the cleanest implementation in the repo.
- **No sorted containers in stdlib.** Python is the consistent outlier for sorted data structures. There is no sorted map or sorted set, so the time-based KV store uses `bisect` on parallel sorted lists, and the first-duplicate sorted-set variant uses `bisect_left` + `insort` on a plain list. The `insort` approach is O(n) per insertion due to shifting, giving O(n²) worst case — a real penalty at scale.
- **Tuple-based heap ordering.** `heapq` compares tuples lexicographically, so `(value, list_index, element_index)` gives natural min-heap behavior for free — no custom comparator needed.
- **Set uses `in` before `add`.** Unlike C++, Scala, and Rust where `insert` returns a boolean, Python's `set.add` returns `None`, so the first-duplicate solution checks `val in seen` before adding.
- **Nested dicts for tries.** The hash map trie is arguably the most Pythonic implementation in the repo — nesting `dict[str, Node]` is natural and concise. No memory management, no pointers, no ownership — just dicts all the way down.
- **`range` counts down directly.** `range(n - 1, -1, -1)` is a descending loop with no idiom and no method call, the simplest of the four spellings. The exclusive stop of `-1` is the only awkward part.
- **Assignment binds a reference.** `lu = a` would alias, so the LU factorization needs an explicit `list(a.val)` to copy the buffer before overwriting it. Python, Scala and Rust all need this; only C++ copies on assignment.
- **Unbounded integers.** Python's ints are arbitrary-precision, so neither the index cap nor the position range exists. It is the only one of the four where the sparse formats have no size limit of any kind — the opposite extreme from Scala.
- **No integer types to distinguish.** The index/position split that C++ and Rust express through `std::int32_t` versus `std::size_t` has nowhere to live in Python, so it survives only as naming: `i` and `j` are indices, `rp` and `cp` are positions. Python is also the only one of the four where `Vector`'s optional buffer is expressible as a default argument (`val: list[float] | None = None`) rather than as a second constructor.
- **Performance paradox.** `(head + 1) % max_size` is relatively fast in Python because the overhead of the interpreter dwarfs the cost of a single modulo.

### Scala (JVM / Hybrid)

- **Style choice.** Uses `mutable.Queue`, `mutable.LinkedHashMap`, `mutable.Stack`, `mutable.PriorityQueue`, `mutable.TreeMap`, `mutable.HashSet`, `mutable.TreeSet`, and `mutable.HashMap` (for trie children) rather than functional approaches so the comparison stays apples-to-apples across all four languages.
- **Reversed Ordering for min-heap.** Scala's `mutable.PriorityQueue` is a max-heap by default. Flipping to min-heap requires passing a reversed `Ordering`, which is more verbose than Rust's `Reverse` wrapper but more flexible for complex key types.
- **TreeMap floor lookups.** `tsMap.to(timestamp).lastOption` returns a view of all entries up to and including the key, then grabs the last one. Clean and readable, but creates an intermediate view.
- **Insert-returns-boolean.** Both `mutable.HashSet.add` and `mutable.TreeSet.add` return `true` if the element was new, `false` if it already existed — same pattern as C++ and Rust.
- **JVM lifecycle.** `sbt` has a long cold-start time, but the JIT compiler optimizes hot paths as the code runs.
- **No unsigned type, and arrays indexed by `Int`.** Scala has `Byte`, `Short`, `Int`, `Long`, all signed, and `Array` subscripts only with `Int`. So the *index* half of the split is fine (`Int` is exactly a 32-bit signed integer) but the *position* half has nowhere to go, which caps nnz at 2^31 where C++ and Rust reach 2^62. `Array[Long]` would not help, since the JVM caps any array at `Int.MaxValue` elements regardless of element type: the fix is chunked storage, a different layout rather than a different declaration. See `NOTES.md`.
- **A `while` counter outlives its loop.** `var i = 0` is scoped to the enclosing block, not to the loop, so a function with several passes cannot reuse the name. Across problems 11 and 12 this forced three renames (`r` in the right-looking factorization, then `r` and `c` in the two solves) that C++, Python and Rust never need, since each of their loop variables is scoped to its own loop. Wrapping each pass in a `{ }` block would restore scoping at the cost of two braces per pass. This is the clearest cost of the `while` choice described below.
- **`while` in the numerical kernels is deliberate.** General Scala would write `for (rp <- a.rowPtr(i) until a.rowPtr(i + 1))`, and that is idiomatic Scala. Numeric Scala libraries (Breeze, Spire) conventionally use `while` in inner loops, because `for` builds a `Range` and passes the body as a function object, leaving the elimination of both to the JIT at run time rather than to the compiler. The kernels follow the numeric convention; problems 1-8 use `while` for an unrelated reason (Scala 3 deprecates non-local `return` inside a `for`).
- **`val` is a reserved word.** The value buffer is named `val` in the other three languages; Scala forbids it, so the field is `values` there. The only place in the repo where shared vocabulary bends to a host language. `Vector` also shadows `scala.Vector` within its own file, harmless since the collection is unused there.
- **Flexibility.** Scala offers the unique ability to switch to immutable collections if data persistence were a requirement.

### Rust (Safe / Explicit)

- **Memory tuning.** `VecDeque` lives on the heap and can grow, while the circular buffer can be tuned for zero-allocation performance once initialized, staying fixed in memory.
- **Allocation strategy.** `vec![0.0; size]` allocates once on the heap and stays that size forever — a fixed-size heap allocation that is extremely performant.
- **Index-based linked list.** Rust's ownership model makes pointer-based doubly linked lists painful (`Rc<RefCell<>>` or `unsafe`). The index-based node pool with sentinel nodes sidesteps this entirely — the same approach used in C++, but in Rust it's the *only* ergonomic option.
- **Two approaches to recursive structures.** The trie shows both Rust strategies: the hash map version uses `Box<Node>` for exclusive ownership (clean, idiomatic, no shared mutation needed), while the array version uses an index-based node pool (the same pattern as the LRU cache). `Box` works here because each child has exactly one parent — no `Rc<RefCell<>>` needed.
- **Reverse wrapper for min-heap.** `BinaryHeap` is a max-heap by default. Wrapping entries in `std::cmp::Reverse` is the idiomatic flip — simpler than C++'s template comparator or Scala's reversed `Ordering`.
- **BTreeMap floor lookups.** `range(..=timestamp).next_back()` returns an iterator over all entries up to and including the key, then grabs the last one. The range syntax `..=` (inclusive) is a Rust-specific feature that makes this very readable.
- **Insert-returns-boolean.** Both `HashSet::insert` and `BTreeSet::insert` return `true` if the value was new — the same pattern as C++ and Scala, making the first-duplicate code nearly identical across three of the four languages.
- **A range is an iterator, so reversing it is a method call.** `(0..n).rev()` is a downward loop with no restructuring: `Range` implements `DoubleEndedIterator`, so `rev()` yields the same values backwards and the loop body is untouched. The cleanest of the four spellings, and it follows directly from ranges being first-class objects rather than syntax.
- **`dead_code` catches an unused field.** The compiler flagged `CsrMatrix.n_size` as never read, which is true and informative: a CSR matvec never needs `n`, since the column bound lives in `colIdx`. None of the other three languages says anything. The lint fires here partly because this is a binary crate, where `pub` exempts nothing since nothing is exported.
- **Range loops are free.** `for i in 0..n` monomorphizes and inlines at compile time, producing the same code as a hand-written loop and often better, since known bounds let the compiler drop the per-iteration bounds check. The idiomatic form is the fast one, which is exactly the contrast with Scala, where the same abstraction is eliminated only if the JIT gets to it.
- **No implicit numeric conversion, and `Vec` subscripts only with `usize`.** Every crossing between integer types is written out with `as`. This is why the index-type convention is a statement about *storage* rather than about loop counters: forcing a non-`usize` counter would give up `for i in 0..n` and turn every loop into a hand-driven `while`, where C++ pays only a cast in the condition. Rust also has no constructor overloading or default arguments, so `Vector` needs two named constructors (`new` and `from_val`) where C++ and Scala use two overloads and Python one default.
- **Safety and Option types.** The `if let Some(...)` pattern forces the developer to acknowledge that operations like `pop_front()` or `range().next_back()` could return `None`, preventing runtime crashes.
- **Explicit mutation.** Rust requires `&mut self`, making it crystal clear which methods modify internal state. The union-find's `find()` with path compression is the sharpest example: it's logically a query, but because it rewrites parent pointers for performance, Rust requires `&mut self` — the naive version without path compression can use `&self` instead.

### Cross-Cutting Observations

- **All four languages default to max-heaps** (problem 4). The flip mechanisms differ: C++ uses a comparator template parameter (`std::greater`), Python's `heapq` is already a min-heap, Scala reverses the `Ordering`, and Rust wraps in `Reverse`. Python is the only one that defaults to min-heap.
- **Sorted-append trick** (problem 5). When timestamps arrive in strictly increasing order, the manual implementation across all four languages is nearly identical: append to a flat list and binary-search on lookup. The sorted-map implementation is where languages diverge — each spells the floor lookup differently.
- **API convergence on sets** (problem 6). C++, Scala, and Rust all offer `insert`-returns-boolean on both hash and sorted sets, making their first-duplicate implementations nearly identical. Python stands apart with its `in` + `add` pattern.
- **Python's stdlib gaps.** Python is the consistent outlier for sorted containers. Problems 5 and 6 both require workarounds (`bisect` on plain lists) where the other three languages have dedicated sorted map and sorted set types.
- **Recursive structure ergonomics** (problem 7). The trie is trivially expressed in Python (nested dicts) and Scala (nested mutable maps). C++ uses `unique_ptr` for clean ownership. Rust offers two paths: `Box` for exclusive ownership (hash map trie) and index-based pools for arena-style allocation (array trie). The trie is the clearest example in the repo of how Rust's ownership model shapes data structure design.
- **Near-identical implementations** (problem 8). Union-find is just index arithmetic on a flat array, so the code looks almost the same in all four languages. The one divergence is Rust's `&mut self` on `find()` with path compression — a query that mutates for performance, which Rust forces you to acknowledge at the type level.

- **Layout versus schedule** (problem 9). The first problem where the container is a flat buffer and the design question is what the indices mean. All four implementations are nearly identical, and the divergence is entirely in integer typing: C++ and Rust can express the index/position distinction in the type system, Scala has one `Int` for both, Python has neither. Since dense matvec stores no indices, none of the four needs a cast; problem 10 is where the distinction has consequences.

- **Where the index/position split has teeth** (problem 10). Dense matvec stores no indices, so all four languages come out identical. Sparse stores them, and the four separate cleanly: C++ and Rust express the split in the type system and pay for the crossing differently (no cast against one `as usize`), Python has unbounded integers so no cap exists at all, and Scala has no unsigned type, capping nnz at 2^31 where the others reach 2^62. This is the widest four-way divergence in the repo, and it is about integer types rather than containers.
- **Zero-cost abstraction, guaranteed against hoped-for** (problems 9 and 10). Rust, Python and Scala all express an inner loop as iteration over a range object. Rust's is eliminated at compile time by monomorphization, Python's is the fastest construct the interpreter offers, and Scala's depends on the JIT inlining `Range.foreach`, which is why numeric Scala conventionally writes `while` instead. C++ has only the counter loop and the question does not arise.
- **Cross-problem reuse** (problem 10). `Vector` is defined once in problem 9 and reused, and each language spells the dependency differently: `#include` in C++, a module import in Python, nothing at all in Scala since both files share a package, and a crate path in Rust.

- **Copy semantics, four ways** (problem 11). Preserving `A` before factoring is one line in C++, where assignment copies the object outright. The other three need an explicit copy: `list(a.val)` in Python, `a.values.clone()` in Scala, `a.val.clone()` in Rust. The reason differs — Python and Scala bind references, Rust would move — but the outcome is that C++ is alone in not needing a helper.
- **Gather and scatter, twice** (problems 9 and 11). In the matvec the two differ only in loop order. In the factorization they differ in when a value becomes final: left-looking finishes a column and touches nothing else, right-looking finishes a column and immediately updates everything after it. The same distinction, once where it is cosmetic and once where it shapes the algorithm.

- **One descending loop, four spellings** (problem 12). Python `range(n - 1, -1, -1)`, Rust `(0..n).rev()`, C++ `for (std::size_t i = n; i-- > 0;)`, Scala a hand-decremented `var`. The differences follow from what each language takes a range to be: a lazy sequence, an iterator object, nothing at all, and nothing at all again once `while` is chosen. C++ is the only one where the awkwardness comes from the *type* of the counter rather than the shape of the loop.
- **The gather/scatter pair, start to finish** (problems 9, 11 and 12). It appears three times at increasing weight: in the matvec as pure loop order, in the factorization as a schedule that changes when a value becomes final, and in the solve as the direction each unknown's information travels. The same distinction underlies left-looking and right-looking factorization in sparse direct solvers.

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
| **Integer conversion** | Implicit (usual arithmetic conversions) | Untyped | Implicit widening (`Int` to `Long`) | None — every `as` written out |
| **Index / position types** | `int32_t` / `size_t` | Unbounded ints | `Int` for both (no unsigned) | `i32` / `usize` |
| **Sparse nnz ceiling** | ~2^62 | None | 2^31 (JVM array limit) | ~2^62 |
| **Style used** | Imperative | Imperative | Mutable (for parity) | Imperative / Safe |
