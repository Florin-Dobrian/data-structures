/// Implementation A: Naive union-find (simple parent tracking).
///
/// Each node points to its parent; the root points to itself. find()
/// walks up the parent chain to the root. unite() links one root to
/// the other with no balancing.
///
/// Worst case: the tree degenerates into a linked list, making find()
/// O(n). This is the baseline to show how much path compression and
/// union by rank improve performance.
pub struct NaiveUnionFind {
    parent: Vec<usize>,
    components: usize,
}

impl NaiveUnionFind {
    pub fn new(n: usize) -> Self {
        Self {
            parent: (0..n).collect(),
            components: n,
        }
    }

    pub fn find(&self, mut x: usize) -> usize {
        while self.parent[x] != x {
            x = self.parent[x];
        }
        x
    }

    pub fn unite(&mut self, a: usize, b: usize) {
        let ra = self.find(a);
        let rb = self.find(b);
        if ra != rb {
            self.parent[ra] = rb;
            self.components -= 1;
        }
    }

    pub fn count(&self) -> usize {
        self.components
    }
}

/// Implementation B: Optimized union-find (path compression + union by rank).
///
/// Path compression: during find(), every node along the path is
/// repointed directly to the root, flattening the tree for future
/// lookups. This is done iteratively with a two-pass approach.
///
/// Union by rank: when merging two trees, the shorter tree is attached
/// under the taller one. Rank is an upper bound on tree height. This
/// prevents the degenerate linked-list case.
///
/// Together, these give nearly O(1) amortized per operation — formally
/// O(α(n)) where α is the inverse Ackermann function, which is ≤ 4
/// for any practical input size.
///
/// Note: find() takes &mut self because path compression mutates the
/// parent array. This is the correct Rust signature — even though
/// find() is logically a "query", it modifies internal state for
/// performance. Rust makes this mutation explicit.
pub struct RankedUnionFind {
    parent: Vec<usize>,
    rank: Vec<usize>,
    components: usize,
}

impl RankedUnionFind {
    pub fn new(n: usize) -> Self {
        Self {
            parent: (0..n).collect(),
            rank: vec![0; n],
            components: n,
        }
    }

    pub fn find(&mut self, x: usize) -> usize {
        // Find root
        let mut root = x;
        while self.parent[root] != root {
            root = self.parent[root];
        }
        // Path compression: point every node on the path to root
        let mut current = x;
        while self.parent[current] != root {
            let next = self.parent[current];
            self.parent[current] = root;
            current = next;
        }
        root
    }

    pub fn unite(&mut self, a: usize, b: usize) {
        let ra = self.find(a);
        let rb = self.find(b);
        if ra != rb {
            // Union by rank: attach shorter tree under taller
            if self.rank[ra] < self.rank[rb] {
                self.parent[ra] = rb;
            } else if self.rank[ra] > self.rank[rb] {
                self.parent[rb] = ra;
            } else {
                self.parent[rb] = ra;
                self.rank[ra] += 1;
            }
            self.components -= 1;
        }
    }

    pub fn count(&self) -> usize {
        self.components
    }
}
