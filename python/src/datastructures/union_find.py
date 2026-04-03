class NaiveUnionFind:
    """Implementation A: Naive union-find (simple parent tracking).

    Each node points to its parent; the root points to itself. find()
    walks up the parent chain to the root. unite() links one root to
    the other with no balancing.

    Worst case: the tree degenerates into a linked list, making find()
    O(n). This is the baseline to show how much path compression and
    union by rank improve performance.
    """

    def __init__(self, n: int):
        self.parent = list(range(n))
        self.components = n

    def find(self, x: int) -> int:
        while self.parent[x] != x:
            x = self.parent[x]
        return x

    def unite(self, a: int, b: int):
        ra = self.find(a)
        rb = self.find(b)
        if ra != rb:
            self.parent[ra] = rb
            self.components -= 1

    def count(self) -> int:
        return self.components


class RankedUnionFind:
    """Implementation B: Optimized union-find (path compression + union by rank).

    Path compression: during find(), every node along the path is
    repointed directly to the root, flattening the tree for future
    lookups. This is done iteratively with a two-pass approach.

    Union by rank: when merging two trees, the shorter tree is attached
    under the taller one. Rank is an upper bound on tree height. This
    prevents the degenerate linked-list case.

    Together, these give nearly O(1) amortized per operation — formally
    O(α(n)) where α is the inverse Ackermann function, which is ≤ 4
    for any practical input size.
    """

    def __init__(self, n: int):
        self.parent = list(range(n))
        self.rank = [0] * n
        self.components = n

    def find(self, x: int) -> int:
        # Find root
        root = x
        while self.parent[root] != root:
            root = self.parent[root]
        # Path compression: point every node on the path to root
        while self.parent[x] != root:
            next_x = self.parent[x]
            self.parent[x] = root
            x = next_x
        return root

    def unite(self, a: int, b: int):
        ra = self.find(a)
        rb = self.find(b)
        if ra != rb:
            # Union by rank: attach shorter tree under taller
            if self.rank[ra] < self.rank[rb]:
                self.parent[ra] = rb
            elif self.rank[ra] > self.rank[rb]:
                self.parent[rb] = ra
            else:
                self.parent[rb] = ra
                self.rank[ra] += 1
            self.components -= 1

    def count(self) -> int:
        return self.components
