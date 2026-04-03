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
| 8 | Connected Components | union-find (naive + path compression / union by rank) |

## Recommendations

All eight problems are implemented. The repo covers: arrays, deques, hash maps, linked lists, stacks, heaps, sorted maps, hash sets, sorted sets, tries, and union-find — a comprehensive tour of the container families and algorithmic data structures you reach for in practice.

### Suggested order (for new readers)

1. Moving Average Tracker — deque vs. circular buffer, simplest starting point
2. LRU Cache — hash map + linked list, introduces index-based node pools
3. Next Greater Element — monotonic stack pattern, values vs. indices insight
4. Merge K Sorted Lists — priority queue, cross-language comparator contrast
5. Time-Based Key-Value Store — sorted map, floor lookups, Python stdlib gaps
6. First Duplicate in a Stream — hash set vs. sorted set, API convergence
7. Prefix Trie — recursive ownership, Rust Box vs. index-based pools
8. Connected Components — union-find, path compression, Rust &mut self on queries
