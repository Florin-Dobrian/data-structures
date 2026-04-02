mod avg_tracker;
mod lru_cache;
mod next_greater;
mod merge_k_sorted;
mod time_kv_store;
mod first_duplicate;

use avg_tracker::{DequeTracker, CircularBufferTracker};
use lru_cache::{SimpleVecLRUCache, ManualLRUCache};
use next_greater::{
    next_greater_right_to_left, next_greater_left_to_right,
    next_greater_right_to_left_manual, next_greater_left_to_right_manual,
};
use merge_k_sorted::{merge_k_binary_heap, merge_k_manual_heap};
use time_kv_store::{BTreeMapTimeKV, ManualBinarySearchTimeKV};
use first_duplicate::{first_duplicate_hash_set, first_duplicate_sorted_set};

fn run_trackers() {
    println!("=== Problem 1: Moving Average Tracker ===\n");
    let size = 3;
    let data = vec![10.0, 20.0, 30.0, 40.0, 50.0];

    println!("--- DequeTracker ---");
    let mut dt = DequeTracker::new(size);
    for &val in &data {
        dt.add(val);
        println!("Added {}: Average = {:.2}", val, dt.get_average());
    }

    println!("\n--- CircularBufferTracker ---");
    let mut cbt = CircularBufferTracker::new(size);
    for &val in &data {
        cbt.add(val);
        println!("Added {}: Average = {:.2}", val, cbt.get_average());
    }
    println!();
}

macro_rules! test_lru {
    ($name:expr, $cache:expr) => {{
        let cache = $cache;
        println!("--- {} ---", $name);
        cache.put(1, 1);
        cache.put(2, 2);
        println!("get(1) = {}", cache.get(1));
        cache.put(3, 3);  // evicts key 2
        println!("get(2) = {}", cache.get(2));
        cache.put(4, 4);  // evicts key 1
        println!("get(1) = {}", cache.get(1));
        println!("get(3) = {}", cache.get(3));
        println!("get(4) = {}", cache.get(4));
        println!();
    }};
}

fn run_lru_cache() {
    println!("=== Problem 2: LRU Cache ===\n");
    test_lru!("SimpleVecLRUCache", &mut SimpleVecLRUCache::new(2));
    test_lru!("ManualLRUCache", &mut ManualLRUCache::new(2));
}

fn run_next_greater() {
    println!("=== Problem 3: Next Greater Element ===\n");
    let data = vec![4, 5, 2, 25, 7, 18];

    let fns: Vec<(&str, fn(&[i32]) -> Vec<i32>)> = vec![
        ("next_greater_right_to_left", next_greater_right_to_left),
        ("next_greater_left_to_right", next_greater_left_to_right),
        ("next_greater_right_to_left_manual", next_greater_right_to_left_manual),
        ("next_greater_left_to_right_manual", next_greater_left_to_right_manual),
    ];

    for (name, f) in &fns {
        let result = f(&data);
        println!("--- {} ---", name);
        println!("Input:  {:?}", data);
        println!("Output: {:?}", result);
        println!();
    }
}

fn run_merge_k_sorted() {
    println!("=== Problem 4: Merge K Sorted Lists ===\n");
    let lists = vec![vec![1, 4, 7], vec![2, 5, 8], vec![3, 6, 9]];

    let fns: Vec<(&str, fn(&[Vec<i32>]) -> Vec<i32>)> = vec![
        ("merge_k_binary_heap", merge_k_binary_heap),
        ("merge_k_manual_heap", merge_k_manual_heap),
    ];

    for (name, f) in &fns {
        let result = f(&lists);
        println!("--- {} ---", name);
        println!("Input:  {:?}", lists);
        println!("Output: {:?}", result);
        println!();
    }
}

fn run_time_kv_store() {
    println!("=== Problem 5: Time-Based Key-Value Store ===\n");

    macro_rules! test_kv {
        ($name:expr, $kv:expr) => {{
            let kv = $kv;
            println!("--- {} ---", $name);
            kv.set("alice", "alice_v1", 1);
            kv.set("alice", "alice_v2", 4);
            kv.set("alice", "alice_v3", 7);
            kv.set("bob",   "bob_v1",   2);
            kv.set("bob",   "bob_v2",   5);

            println!("get(alice, 0) = \"{}\"", kv.get("alice", 0));
            println!("get(alice, 1) = \"{}\"", kv.get("alice", 1));
            println!("get(alice, 3) = \"{}\"", kv.get("alice", 3));
            println!("get(alice, 4) = \"{}\"", kv.get("alice", 4));
            println!("get(alice, 6) = \"{}\"", kv.get("alice", 6));
            println!("get(alice, 9) = \"{}\"", kv.get("alice", 9));
            println!("get(bob,   3) = \"{}\"", kv.get("bob",   3));
            println!("get(bob,   5) = \"{}\"", kv.get("bob",   5));
            println!("get(carol, 1) = \"{}\"", kv.get("carol", 1));
            println!();
        }};
    }

    test_kv!("BTreeMapTimeKV", &mut BTreeMapTimeKV::new());
    test_kv!("ManualBinarySearchTimeKV", &mut ManualBinarySearchTimeKV::new());
}

fn run_first_duplicate() {
    println!("=== Problem 6: First Duplicate in a Stream ===\n");

    let cases: Vec<(&str, Vec<i32>)> = vec![
        ("has duplicate",    vec![2, 1, 3, 5, 3, 2]),
        ("immediate repeat", vec![7, 7, 1, 2]),
        ("no duplicate",     vec![1, 2, 3, 4, 5]),
        ("empty",            vec![]),
    ];

    let fns: Vec<(&str, fn(&[i32]) -> i32)> = vec![
        ("first_duplicate_hash_set", first_duplicate_hash_set),
        ("first_duplicate_sorted_set", first_duplicate_sorted_set),
    ];

    for (name, f) in &fns {
        println!("--- {} ---", name);
        for (label, data) in &cases {
            println!("{}: {:?} -> {}", label, data, f(data));
        }
        println!();
    }
}

fn main() {
    run_trackers();
    run_lru_cache();
    run_next_greater();
    run_merge_k_sorted();
    run_time_kv_store();
    run_first_duplicate();
}
