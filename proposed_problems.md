# Proposed Problems

A catalog of candidate problems for the data-structures repo. Each targets a specific container family and is small enough to implement in a single file per language.

## Current Problems

| # | Problem | Containers Exercised |
|---|---------|---------------------|
| 1 | Moving Average Tracker | deque, circular buffer (array) |
| 2 | LRU Cache | hash map + doubly linked list, ordered map |
| 3 | Next Greater Element | monotonic stack (stdlib + manual array-backed) |
| 4 | Merge K Sorted Lists | priority queue / binary min-heap |
| 5 | Time-Based Key-Value Store | sorted map, binary search on sorted vector |

## Candidates

### 3. Next Greater Element (Monotonic Stack)

Given an array, find the next element that is strictly greater for each position. If none exists, return -1.

**Containers:** `list` used as stack (Python), `std::stack` (C++), `Vec` as stack (Rust), `mutable.Stack` (Scala).

**Why it's interesting:** The monotonic stack is a pattern that shows up everywhere (stock span, histogram area, temperature problems). Each language handles stack-like usage differently — Python and Rust just use their dynamic arrays, while C++ and Scala have dedicated stack types. Good place to compare when a specialized container is worth it versus just using a vector/list.

### 4. Merge K Sorted Lists (Priority Queue / Heap)

Given k sorted arrays, merge them into a single sorted array.

**Containers:** `heapq` (Python), `std::priority_queue` (C++), `BinaryHeap` (Rust), `mutable.PriorityQueue` (Scala).

**Why it's interesting:** Custom comparators work very differently across the four languages. Python uses tuples for natural ordering, C++ uses a comparator template parameter, Rust requires implementing `Ord` (and the default is a max-heap so you wrap in `Reverse`), Scala passes an `Ordering`. Two implementations: one using the standard heap, one using a manual binary heap on an array.

### 5. First Duplicate in a Stream (Hash Set)

Given a stream of values, find and return the first value that appears a second time.

**Containers:** `set` (Python), `std::unordered_set` (C++), `HashSet` (Rust), `mutable.HashSet` (Scala).

**Why it's interesting:** The simplest possible hash set exercise, but worth having because hash sets are used constantly and the APIs differ. Can pair implementation A (hash set) with implementation B (sorted set: `BTreeSet`, `TreeSet`, `std::set`) to compare ordered vs unordered performance and iteration guarantees.

### 6. Prefix Trie (Custom Tree Structure)

Implement insert, search, and starts-with (prefix matching) on a collection of strings.

**Containers:** No standard trie in any of the four languages — built from scratch using hash maps or fixed-size arrays at each node.

**Why it's interesting:** Forces you to build a recursive/nested mutable structure from scratch. This is trivial in Python and Scala (just nest dicts/maps), painful in Rust (recursive types need `Box`, shared mutation needs `Rc<RefCell<>>` or index-based approaches), and straightforward but verbose in C++ (raw pointers or `unique_ptr`). Two implementations: one using a hash map at each node (flexible, any alphabet), one using a fixed-size array (faster, ASCII only).

### 7. Connected Components (Union-Find / Disjoint Set)

Given n nodes and a list of edges, count the number of connected components.

**Containers:** A flat array/vector with path compression and union by rank. Minimal container usage but a fundamental algorithmic data structure.

**Why it's interesting:** The data structure itself is just an array, so the implementation is nearly identical across all four languages. The value is in seeing how each language handles the mutable array manipulation (index arithmetic, bounds checking in Rust). Two implementations: one with path compression + union by rank (optimized), one naive with simple parent tracking (to show the performance difference).

## Recommendations

Problems 3, 4, and 5 are now implemented. The repo covers: arrays, deques, hash maps, linked lists, stacks, heaps, and sorted maps — which is most of what you reach for in practice.

Problems 5 (hash set), 6 (trie), and 7 (union-find) are good follow-ups. Problem 5 (hash set) is almost too simple on its own — it might work better folded into another problem. Problem 6 (trie) is the most educational for Rust ownership but is a bigger lift. Problem 7 (union-find) is useful but doesn't exercise new containers.

### Suggested order

1. ~~Next Greater Element (monotonic stack)~~ — done
2. ~~Merge K Sorted Lists (priority queue)~~ — done
3. ~~Time-Based Key-Value Store (sorted map)~~ — done
4. Prefix Trie — deeper Rust ownership exercise
5. First Duplicate / Connected Components — as needed
