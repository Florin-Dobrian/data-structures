#pragma once

#include <vector>
#include <unordered_set>
#include <set>

/**
 * Implementation A: Using std::unordered_set (hash set).
 *
 * Iterate through the stream and insert each value into the hash set.
 * If insert fails (the value is already present), we've found the first
 * duplicate. O(n) expected time, O(n) space.
 *
 * std::unordered_set::insert returns a pair where .second is false if
 * the element already existed — no separate lookup needed.
 */
inline int first_duplicate_hash_set(const std::vector<int>& data) {
    std::unordered_set<int> seen;
    for (int val : data) {
        if (!seen.insert(val).second) {
            return val;
        }
    }
    return -1;
}

/**
 * Implementation B: Using std::set (sorted set / red-black tree).
 *
 * Same algorithm as A, but the underlying container maintains sorted
 * order. Insert and lookup are O(log n) instead of O(1) amortized,
 * making overall time O(n log n). The tradeoff: iteration yields
 * elements in sorted order, and worst-case performance is more
 * predictable (no hash collisions).
 *
 * std::set::insert has the same return-pair interface as unordered_set,
 * so the code is nearly identical.
 */
inline int first_duplicate_sorted_set(const std::vector<int>& data) {
    std::set<int> seen;
    for (int val : data) {
        if (!seen.insert(val).second) {
            return val;
        }
    }
    return -1;
}
