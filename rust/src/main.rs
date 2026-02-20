mod avg_tracker;
mod lru_cache;

use avg_tracker::{DequeTracker, CircularBufferTracker};
use lru_cache::{SimpleVecLRUCache, ManualLRUCache};

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

fn main() {
    run_trackers();
    run_lru_cache();
}
