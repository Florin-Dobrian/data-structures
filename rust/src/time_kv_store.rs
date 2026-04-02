use std::collections::{HashMap, BTreeMap};

/// Implementation A: Using BTreeMap (standard library sorted map / B-tree).
///
/// Each key maps to a BTreeMap<i32, String> of timestamp -> value.
/// get() uses range(..=timestamp).next_back() to find the largest
/// timestamp <= the query. The range(..=t) method returns an iterator
/// over all entries up to and including t; next_back() grabs the last
/// one. O(log n) per operation.
pub struct BTreeMapTimeKV {
    store: HashMap<String, BTreeMap<i32, String>>,
}

impl BTreeMapTimeKV {
    pub fn new() -> Self {
        Self {
            store: HashMap::new(),
        }
    }

    pub fn set(&mut self, key: &str, value: &str, timestamp: i32) {
        self.store
            .entry(key.to_string())
            .or_insert_with(BTreeMap::new)
            .insert(timestamp, value.to_string());
    }

    pub fn get(&self, key: &str, timestamp: i32) -> String {
        match self.store.get(key) {
            None => String::new(),
            Some(ts_map) => {
                match ts_map.range(..=timestamp).next_back() {
                    Some((_, value)) => value.clone(),
                    None => String::new(),
                }
            }
        }
    }
}

/// Implementation B: Using manual binary search on a sorted Vec.
///
/// Each key maps to a flat Vec of (timestamp, value) pairs. Because
/// timestamps are inserted in strictly increasing order, the vector
/// stays sorted without any insertion cost — just push.
///
/// get() uses a hand-written binary search to find the rightmost
/// timestamp <= the query. If mid's timestamp fits, we record it as a
/// candidate and search right for a later one. If it's too large, we
/// search left.
pub struct ManualBinarySearchTimeKV {
    store: HashMap<String, Vec<(i32, String)>>,
}

impl ManualBinarySearchTimeKV {
    pub fn new() -> Self {
        Self {
            store: HashMap::new(),
        }
    }

    pub fn set(&mut self, key: &str, value: &str, timestamp: i32) {
        self.store
            .entry(key.to_string())
            .or_insert_with(Vec::new)
            .push((timestamp, value.to_string()));
    }

    pub fn get(&self, key: &str, timestamp: i32) -> String {
        match self.store.get(key) {
            None => String::new(),
            Some(entries) => {
                if entries.is_empty() {
                    return String::new();
                }

                // Binary search for rightmost timestamp <= query
                let mut lo: i32 = 0;
                let mut hi: i32 = entries.len() as i32 - 1;
                let mut best: i32 = -1;

                while lo <= hi {
                    let mid = lo + (hi - lo) / 2;
                    if entries[mid as usize].0 <= timestamp {
                        best = mid;
                        lo = mid + 1;
                    } else {
                        hi = mid - 1;
                    }
                }

                if best == -1 {
                    String::new()
                } else {
                    entries[best as usize].1.clone()
                }
            }
        }
    }
}
