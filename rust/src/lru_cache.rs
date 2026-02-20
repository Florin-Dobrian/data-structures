use std::collections::HashMap;

/// Implementation A: HashMap + Vec-based ordered list.
///
/// Uses a Vec of entries with a "generation" approach: the order vec stores
/// keys in access order. On access, the key is moved to the end.
/// This gives O(n) move but keeps things simple and safe.
/// For a true O(1) solution, see ManualLRUCache below.
pub struct SimpleVecLRUCache {
    capacity: usize,
    map: HashMap<i32, i32>,
    order: Vec<i32>,
}

impl SimpleVecLRUCache {
    pub fn new(capacity: usize) -> Self {
        Self {
            capacity,
            map: HashMap::with_capacity(capacity),
            order: Vec::with_capacity(capacity),
        }
    }

    fn touch(&mut self, key: i32) {
        if let Some(pos) = self.order.iter().position(|&k| k == key) {
            self.order.remove(pos);
        }
        self.order.push(key);
    }

    pub fn get(&mut self, key: i32) -> i32 {
        if let Some(&value) = self.map.get(&key) {
            self.touch(key);
            value
        } else {
            -1
        }
    }

    pub fn put(&mut self, key: i32, value: i32) {
        if self.map.contains_key(&key) {
            self.map.insert(key, value);
            self.touch(key);
        } else {
            if self.map.len() == self.capacity {
                let lru = self.order.remove(0);
                self.map.remove(&lru);
            }
            self.map.insert(key, value);
            self.order.push(key);
        }
    }
}

/// Implementation B: HashMap + index-based doubly linked list.
///
/// Nodes live in a Vec (pool). Sentinel head (index 0) and tail (index 1)
/// simplify edge cases. Free nodes are managed via a simple free list.
/// All operations are O(1).
struct Node {
    key: i32,
    value: i32,
    prev: usize,
    next: usize,
}

pub struct ManualLRUCache {
    capacity: usize,
    pool: Vec<Node>,
    map: HashMap<i32, usize>,  // key -> pool index
    free_head: usize,
}

impl ManualLRUCache {
    pub fn new(capacity: usize) -> Self {
        let mut pool = Vec::with_capacity(capacity + 2);

        // Index 0: head sentinel, index 1: tail sentinel
        pool.push(Node { key: 0, value: 0, prev: 0, next: 1 });
        pool.push(Node { key: 0, value: 0, prev: 0, next: 1 });

        // Data nodes: indices 2..capacity+1
        for i in 2..capacity + 2 {
            pool.push(Node {
                key: 0,
                value: 0,
                prev: 0,
                next: if i + 1 < capacity + 2 { i + 1 } else { usize::MAX },
            });
        }

        Self {
            capacity,
            pool,
            map: HashMap::with_capacity(capacity),
            free_head: 2,
        }
    }

    fn remove(&mut self, idx: usize) {
        let prev = self.pool[idx].prev;
        let next = self.pool[idx].next;
        self.pool[prev].next = next;
        self.pool[next].prev = prev;
    }

    fn add_before_tail(&mut self, idx: usize) {
        let prev = self.pool[1].prev;
        self.pool[prev].next = idx;
        self.pool[idx].prev = prev;
        self.pool[idx].next = 1;
        self.pool[1].prev = idx;
    }

    fn alloc_node(&mut self) -> usize {
        let idx = self.free_head;
        self.free_head = self.pool[idx].next;
        idx
    }

    fn free_node(&mut self, idx: usize) {
        self.pool[idx].next = self.free_head;
        self.free_head = idx;
    }

    pub fn get(&mut self, key: i32) -> i32 {
        if let Some(&idx) = self.map.get(&key) {
            self.remove(idx);
            self.add_before_tail(idx);
            self.pool[idx].value
        } else {
            -1
        }
    }

    pub fn put(&mut self, key: i32, value: i32) {
        if let Some(&idx) = self.map.get(&key) {
            self.pool[idx].value = value;
            self.remove(idx);
            self.add_before_tail(idx);
        } else {
            if self.map.len() == self.capacity {
                let lru = self.pool[0].next;
                self.remove(lru);
                self.map.remove(&self.pool[lru].key);
                self.free_node(lru);
            }
            let idx = self.alloc_node();
            self.pool[idx].key = key;
            self.pool[idx].value = value;
            self.map.insert(key, idx);
            self.add_before_tail(idx);
        }
    }
}
