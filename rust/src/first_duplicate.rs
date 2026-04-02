use std::collections::HashSet;
use std::collections::BTreeSet;

/// Implementation A: Using HashSet (hash set).
///
/// Iterate through the stream and insert each value into the hash set.
/// If insert returns false (the value was already present), we've found
/// the first duplicate. O(n) expected time, O(n) space.
///
/// HashSet::insert returns true if the value was added (new), false if
/// it already existed — no separate lookup needed.
pub fn first_duplicate_hash_set(data: &[i32]) -> i32 {
    let mut seen = HashSet::new();
    for &val in data {
        if !seen.insert(val) {
            return val;
        }
    }
    -1
}

/// Implementation B: Using BTreeSet (sorted set / B-tree).
///
/// Same algorithm as A, but the underlying container maintains sorted
/// order. Insert and lookup are O(log n) instead of O(1) amortized,
/// making overall time O(n log n). The tradeoff: iteration yields
/// elements in sorted order, and worst-case performance is more
/// predictable (no hash collisions).
///
/// BTreeSet::insert has the same boolean-return interface as HashSet,
/// so the code is nearly identical.
pub fn first_duplicate_sorted_set(data: &[i32]) -> i32 {
    let mut seen = BTreeSet::new();
    for &val in data {
        if !seen.insert(val) {
            return val;
        }
    }
    -1
}
