# Proposed Problems

A catalog of candidate problems for the data-structures repo. Each is small enough to implement in a single file per language: the discrete problems target a specific container family, the numerical kernels a specific layout or schedule.

## Current Problems

### Discrete Problems

| # | Problem | Structures Exercised |
|---|---------|---------------------|
| 1 | Moving Average Tracker | deque, circular buffer (array) |
| 2 | LRU Cache | hash map + doubly linked list, ordered map |
| 3 | Next Greater Element | monotonic stack (stdlib + manual array-backed) |
| 4 | Merge K Sorted Lists | priority queue / binary min-heap |
| 5 | Time-Based Key-Value Store | sorted map, binary search on sorted vector |
| 6 | First Duplicate in a Stream | hash set, sorted/tree set |
| 7 | Prefix Trie | hash map trie, fixed-array trie |
| 8 | Connected Components | union-find (naive + path compression / union by rank) |

### Numerical Kernels

| # | Problem | Structures Exercised |
|---|---------|---------------------|
| 9 | Dense Matrix-Vector Product | flat value buffer, row-major and column-major |

## Recommendations

The eight discrete problems are implemented, and the numerical kernels have started. The discrete half covers arrays, deques, hash maps, linked lists, stacks, heaps, sorted maps, hash sets, sorted sets, tries, and union-find — a comprehensive tour of the container families and algorithmic data structures you reach for in practice. The numerical half turns to flat buffers, where the design question is what the indices mean rather than which container to reach for.

### Suggested order (for new readers)

1. Moving Average Tracker — deque vs. circular buffer, simplest starting point
2. LRU Cache — hash map + linked list, introduces index-based node pools
3. Next Greater Element — monotonic stack pattern, values vs. indices insight
4. Merge K Sorted Lists — priority queue, cross-language comparator contrast
5. Time-Based Key-Value Store — sorted map, floor lookups, Python stdlib gaps
6. First Duplicate in a Stream — hash set vs. sorted set, API convergence
7. Prefix Trie — recursive ownership, Rust Box vs. index-based pools
8. Connected Components — union-find, path compression, Rust &mut self on queries
9. Dense Matrix-Vector Product — gather vs. scatter, row-major vs. column-major, index versus position
