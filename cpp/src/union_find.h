#pragma once

#include <vector>
#include <numeric>

/**
 * Implementation A: Naive union-find (simple parent tracking).
 *
 * Each node points to its parent; the root points to itself. find()
 * walks up the parent chain to the root. unite() links one root to
 * the other with no balancing.
 *
 * Worst case: the tree degenerates into a linked list, making find()
 * O(n). This is the baseline to show how much path compression and
 * union by rank improve performance.
 */
class NaiveUnionFind {
private:
    std::vector<int> parent;
    int components;

public:
    NaiveUnionFind(int n) : parent(n), components(n) {
        std::iota(parent.begin(), parent.end(), 0);  // parent[i] = i
    }

    int find(int x) {
        while (parent[x] != x) {
            x = parent[x];
        }
        return x;
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            parent[ra] = rb;
            components--;
        }
    }

    int count() const { return components; }
};

/**
 * Implementation B: Optimized union-find (path compression + union by rank).
 *
 * Path compression: during find(), every node along the path is
 * repointed directly to the root, flattening the tree for future
 * lookups. This is done iteratively with a two-pass approach.
 *
 * Union by rank: when merging two trees, the shorter tree is attached
 * under the taller one. Rank is an upper bound on tree height. This
 * prevents the degenerate linked-list case.
 *
 * Together, these give nearly O(1) amortized per operation — formally
 * O(α(n)) where α is the inverse Ackermann function, which is ≤ 4
 * for any practical input size.
 */
class RankedUnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;
    int components;

public:
    RankedUnionFind(int n) : parent(n), rank(n, 0), components(n) {
        std::iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        // Find root
        int root = x;
        while (parent[root] != root) {
            root = parent[root];
        }
        // Path compression: point every node on the path to root
        while (parent[x] != root) {
            int next = parent[x];
            parent[x] = root;
            x = next;
        }
        return root;
    }

    void unite(int a, int b) {
        int ra = find(a);
        int rb = find(b);
        if (ra != rb) {
            // Union by rank: attach shorter tree under taller
            if (rank[ra] < rank[rb]) {
                parent[ra] = rb;
            } else if (rank[ra] > rank[rb]) {
                parent[rb] = ra;
            } else {
                parent[rb] = ra;
                rank[ra]++;
            }
            components--;
        }
    }

    int count() const { return components; }
};
