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
| 6 | First Duplicate in a Stream | hash set, sorted/tree set |

## Candidates

### 7. Prefix Trie (Custom Tree Structure)

Implement insert, search, and starts-with (prefix matching) on a collection of strings.

**Containers:** No standard trie in any of the four languages — built from scratch using hash maps or fixed-size arrays at each node.

**Why it's interesting:** Forces you to build a recursive/nested mutable structure from scratch. This is trivial in Python and Scala (just nest dicts/maps), painful in Rust (recursive types need `Box`, shared mutation needs `Rc<RefCell<>>` or index-based approaches), and straightforward but verbose in C++ (raw pointers or `unique_ptr`). Two implementations: one using a hash map at each node (flexible, any alphabet), one using a fixed-size array (faster, ASCII only).

### 8. Connected Components (Union-Find / Disjoint Set)

Given n nodes and a list of edges, count the number of connected components.

**Containers:** A flat array/vector with path compression and union by rank. Minimal container usage but a fundamental algorithmic data structure.

**Why it's interesting:** The data structure itself is just an array, so the implementation is nearly identical across all four languages. The value is in seeing how each language handles the mutable array manipulation (index arithmetic, bounds checking in Rust). Two implementations: one with path compression + union by rank (optimized), one naive with simple parent tracking (to show the performance difference).

## Recommendations

Problems 1 through 6 are implemented. The repo covers: arrays, deques, hash maps, linked lists, stacks, heaps, sorted maps, hash sets, and sorted sets — which is most of what you reach for in practice.

Problems 7 (trie) and 8 (union-find) are the remaining candidates. Problem 7 (trie) is the most educational for Rust ownership but is a bigger lift. Problem 8 (union-find) is useful but doesn't exercise new containers.

### Suggested order

1. ~~Next Greater Element (monotonic stack)~~ — done
2. ~~Merge K Sorted Lists (priority queue)~~ — done
3. ~~Time-Based Key-Value Store (sorted map)~~ — done
4. ~~First Duplicate in a Stream (hash set)~~ — done
5. Prefix Trie — deeper Rust ownership exercise
6. Connected Components — as needed
