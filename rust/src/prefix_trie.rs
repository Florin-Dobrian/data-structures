use std::collections::HashMap;

/// Implementation A: Hash map trie (flexible alphabet).
///
/// Each node stores children in a HashMap<u8, Box<Node>>. Box provides
/// heap allocation for the recursive structure — without it, Node would
/// have infinite size. This supports any byte value as a character.
///
/// Rust's ownership model makes this straightforward: each node owns its
/// children exclusively via Box, so the entire trie is freed when the
/// root is dropped. No Rc<RefCell<>> needed because there is no shared
/// ownership — each child has exactly one parent.
pub struct HashMapTrie {
    children: HashMap<u8, Box<HashMapTrie>>,
    is_end: bool,
}

impl HashMapTrie {
    pub fn new() -> Self {
        Self {
            children: HashMap::new(),
            is_end: false,
        }
    }

    pub fn insert(&mut self, word: &str) {
        let mut current = self;
        for b in word.bytes() {
            current = current.children
                .entry(b)
                .or_insert_with(|| Box::new(HashMapTrie::new()));
        }
        current.is_end = true;
    }

    pub fn search(&self, word: &str) -> bool {
        match self.traverse(word) {
            Some(node) => node.is_end,
            None => false,
        }
    }

    pub fn starts_with(&self, prefix: &str) -> bool {
        self.traverse(prefix).is_some()
    }

    fn traverse(&self, s: &str) -> Option<&HashMapTrie> {
        let mut current = self;
        for b in s.bytes() {
            match current.children.get(&b) {
                Some(child) => current = child,
                None => return None,
            }
        }
        Some(current)
    }
}

/// Implementation B: Index-based trie with fixed array (lowercase ASCII only).
///
/// Nodes live in a Vec (pool), referenced by index rather than pointer.
/// Each node has a fixed array of 26 child indices (one per letter a-z),
/// where usize::MAX means "no child". This is the same index-based
/// pattern used in the LRU cache's manual linked list — it sidesteps
/// Rust's ownership constraints entirely since all nodes are owned by
/// the single Vec.
///
/// New nodes are allocated by pushing onto the pool. No free list is
/// needed because trie nodes are never removed in this implementation.
pub struct ArrayTrie {
    pool: Vec<ArrayTrieNode>,
}

struct ArrayTrieNode {
    children: [usize; 26],
    is_end: bool,
}

impl ArrayTrieNode {
    fn new() -> Self {
        Self {
            children: [usize::MAX; 26],
            is_end: false,
        }
    }
}

impl ArrayTrie {
    pub fn new() -> Self {
        Self {
            pool: vec![ArrayTrieNode::new()], // index 0 = root
        }
    }

    pub fn insert(&mut self, word: &str) {
        let mut current = 0;
        for b in word.bytes() {
            let idx = (b - b'a') as usize;
            if self.pool[current].children[idx] == usize::MAX {
                let new_idx = self.pool.len();
                self.pool.push(ArrayTrieNode::new());
                self.pool[current].children[idx] = new_idx;
            }
            current = self.pool[current].children[idx];
        }
        self.pool[current].is_end = true;
    }

    pub fn search(&self, word: &str) -> bool {
        match self.traverse(word) {
            Some(idx) => self.pool[idx].is_end,
            None => false,
        }
    }

    pub fn starts_with(&self, prefix: &str) -> bool {
        self.traverse(prefix).is_some()
    }

    fn traverse(&self, s: &str) -> Option<usize> {
        let mut current = 0;
        for b in s.bytes() {
            let idx = (b - b'a') as usize;
            if self.pool[current].children[idx] == usize::MAX {
                return None;
            }
            current = self.pool[current].children[idx];
        }
        Some(current)
    }
}
