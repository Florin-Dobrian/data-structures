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
| 7 | Prefix Trie | hash map trie, fixed-array trie |

## Candidates

### 8. Connected Components (Union-Find / Disjoint Set)

Given n nodes and a list of edges, count the number of connected components.

**Containers:** A flat array/vector with path compression and union by rank. Minimal container usage but a fundamental algorithmic data structure.

**Why it's interesting:** The data structure itself is just an array, so the implementation is nearly identical across all four languages. The value is in seeing how each language handles the mutable array manipulation (index arithmetic, bounds checking in Rust). Two implementations: one with path compression + union by rank (optimized), one naive with simple parent tracking (to show the performance difference).

## Recommendations

Problems 1 through 7 are implemented. The repo covers: arrays, deques, hash maps, linked lists, stacks, heaps, sorted maps, hash sets, sorted sets, and tries.

Problem 8 (union-find) is the sole remaining candidate. It's useful as a fundamental algorithmic structure but doesn't exercise new containers — it's just a flat array with index arithmetic.

### Suggested order

1. ~~Next Greater Element (monotonic stack)~~ — done
2. ~~Merge K Sorted Lists (priority queue)~~ — done
3. ~~Time-Based Key-Value Store (sorted map)~~ — done
4. ~~First Duplicate in a Stream (hash set)~~ — done
5. ~~Prefix Trie (custom tree structure)~~ — done
6. Connected Components — as needed
