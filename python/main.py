from datastructures.avg_tracker import DequeTracker, CircularBufferTracker
from datastructures.lru_cache import OrderedDictLRUCache, ManualLRUCache
from datastructures.next_greater import (
    next_greater_right_to_left, next_greater_left_to_right,
    next_greater_right_to_left_manual, next_greater_left_to_right_manual,
)
from datastructures.merge_k_sorted import merge_k_heapq, merge_k_manual_heap
from datastructures.time_kv_store import BisectTimeKV, ManualBinarySearchTimeKV
from datastructures.first_duplicate import first_duplicate_hash_set, first_duplicate_sorted_set
from datastructures.prefix_trie import HashMapTrie, ArrayTrie
from datastructures.union_find import NaiveUnionFind, RankedUnionFind
from datastructures.dense_matvec import (
    Vector, RowDenseMatrix, ColDenseMatrix,
    row_dense_matvec, col_dense_matvec,
)
from datastructures.sparse_matvec import (
    CsrMatrix, CscMatrix, csr_sparse_matvec, csc_sparse_matvec,
)
from datastructures.dense_lu_factor import (
    left_dense_lu_factor, right_dense_lu_factor,
)
from datastructures.dense_lu_solve import (
    row_dense_lu_solve, col_dense_lu_solve,
)
from datastructures.dense_convert import (
    row_to_col_dense_convert, col_to_row_dense_convert,
)
from datastructures.sparse_convert import (
    csr_to_csc_sparse_convert, csc_to_csr_sparse_convert,
)


def run_trackers():
    print("=== Problem 1: Moving Average Tracker ===\n")
    size = 3
    data = [10, 20, 30, 40, 50]

    for tracker in [DequeTracker(size), CircularBufferTracker(size)]:
        name = tracker.__class__.__name__
        print(f"--- {name} ---")
        for val in data:
            tracker.add(val)
            print(f"Added {val:2}: Average = {tracker.get_average():.2f}")
        print()


def run_lru_cache():
    print("=== Problem 2: LRU Cache ===\n")

    for cache in [OrderedDictLRUCache(2), ManualLRUCache(2)]:
        name = cache.__class__.__name__
        print(f"--- {name} ---")
        cache.put(1, 1)
        cache.put(2, 2)
        print(f"get(1) = {cache.get(1)}")   # 1
        cache.put(3, 3)                      # evicts key 2
        print(f"get(2) = {cache.get(2)}")   # -1
        cache.put(4, 4)                      # evicts key 1
        print(f"get(1) = {cache.get(1)}")   # -1
        print(f"get(3) = {cache.get(3)}")   # 3
        print(f"get(4) = {cache.get(4)}")   # 4
        print()


def run_next_greater():
    print("=== Problem 3: Next Greater Element ===\n")
    data = [4, 5, 2, 25, 7, 18]

    for fn in [next_greater_right_to_left, next_greater_left_to_right,
               next_greater_right_to_left_manual, next_greater_left_to_right_manual]:
        name = fn.__name__
        result = fn(data)
        print(f"--- {name} ---")
        print(f"Input:  {data}")
        print(f"Output: {result}")
        print()


def run_merge_k_sorted():
    print("=== Problem 4: Merge K Sorted Lists ===\n")
    lists = [[1, 4, 7], [2, 5, 8], [3, 6, 9]]

    for fn in [merge_k_heapq, merge_k_manual_heap]:
        name = fn.__name__
        result = fn(lists)
        print(f"--- {name} ---")
        print(f"Input:  {lists}")
        print(f"Output: {result}")
        print()


def run_time_kv_store():
    print("=== Problem 5: Time-Based Key-Value Store ===\n")

    for kv in [BisectTimeKV(), ManualBinarySearchTimeKV()]:
        name = kv.__class__.__name__
        print(f"--- {name} ---")
        kv.set("alice", "alice_v1", 1)
        kv.set("alice", "alice_v2", 4)
        kv.set("alice", "alice_v3", 7)
        kv.set("bob",   "bob_v1",   2)
        kv.set("bob",   "bob_v2",   5)

        print(f'get(alice, 0) = "{kv.get("alice", 0)}"')
        print(f'get(alice, 1) = "{kv.get("alice", 1)}"')
        print(f'get(alice, 3) = "{kv.get("alice", 3)}"')
        print(f'get(alice, 4) = "{kv.get("alice", 4)}"')
        print(f'get(alice, 6) = "{kv.get("alice", 6)}"')
        print(f'get(alice, 9) = "{kv.get("alice", 9)}"')
        print(f'get(bob,   3) = "{kv.get("bob",   3)}"')
        print(f'get(bob,   5) = "{kv.get("bob",   5)}"')
        print(f'get(carol, 1) = "{kv.get("carol", 1)}"')
        print()


def run_first_duplicate():
    print("=== Problem 6: First Duplicate in a Stream ===\n")

    cases = [
        ("has duplicate",    [2, 1, 3, 5, 3, 2]),
        ("immediate repeat", [7, 7, 1, 2]),
        ("no duplicate",     [1, 2, 3, 4, 5]),
        ("empty",            []),
    ]

    for fn in [first_duplicate_hash_set, first_duplicate_sorted_set]:
        name = fn.__name__
        print(f"--- {name} ---")
        for label, data in cases:
            print(f"{label}: {data} -> {fn(data)}")
        print()


def run_prefix_trie():
    print("=== Problem 7: Prefix Trie ===\n")

    for trie in [HashMapTrie(), ArrayTrie()]:
        name = trie.__class__.__name__
        print(f"--- {name} ---")
        trie.insert("apple")
        trie.insert("app")
        trie.insert("apricot")
        trie.insert("banana")

        print(f"search(apple)   = {trie.search('apple')}")
        print(f"search(app)     = {trie.search('app')}")
        print(f"search(ap)      = {trie.search('ap')}")
        print(f"search(banana)  = {trie.search('banana')}")
        print(f"search(band)    = {trie.search('band')}")
        print(f"starts_with(ap) = {trie.starts_with('ap')}")
        print(f"starts_with(ba) = {trie.starts_with('ba')}")
        print(f"starts_with(ca) = {trie.starts_with('ca')}")
        print()


def run_union_find():
    print("=== Problem 8: Connected Components (Union-Find) ===\n")

    n = 5
    edges = [(0, 1), (1, 2), (3, 4)]

    for uf in [NaiveUnionFind(n), RankedUnionFind(n)]:
        name = uf.__class__.__name__
        print(f"--- {name} ---")
        print(f"Nodes: {n}, Edges: {edges}")

        for a, b in edges:
            uf.unite(a, b)

        print(f"Components: {uf.count()}")
        print(f"find(0) = {uf.find(0)}, find(2) = {uf.find(2)}"
              f" (same: {uf.find(0) == uf.find(2)})")
        print(f"find(0) = {uf.find(0)}, find(3) = {uf.find(3)}"
              f" (same: {uf.find(0) == uf.find(3)})")
        print()


def print_grid(row_major, m, n):
    """Print an m x n matrix as a grid, given its entries in row-major order."""
    for i in range(m):
        print("".join(f"{row_major[i * n + j]:5g}" for j in range(n)))


def run_dense_matvec():
    print("=== Problem 9: Dense Matrix-Vector Product ===\n")

    m, n = 3, 4
    row_major = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0]
    col_major = [1.0, 5.0, 9.0, 2.0, 6.0, 10.0, 3.0, 7.0, 11.0, 4.0, 8.0, 12.0]

    ra = RowDenseMatrix(m, n, row_major)
    ca = ColDenseMatrix(m, n, col_major)
    x = Vector(n, [1.0, 2.0, 3.0, 4.0])

    print(f"A is {m} x {n}:")
    print_grid(row_major, m, n)
    print(f"x = {x.val}\n")

    print("--- row_dense_matvec (gather) ---")
    print(f"val = {ra.val}")
    print(f"y = {row_dense_matvec(ra, x).val}\n")

    print("--- col_dense_matvec (scatter) ---")
    print(f"val = {ca.val}")
    print(f"y = {col_dense_matvec(ca, x).val}\n")


def run_sparse_matvec():
    print("=== Problem 10: Sparse Matrix-Vector Product ===\n")

    m, n = 3, 4

    ra = CsrMatrix(m, n, [0, 2, 3, 6], [0, 2, 1, 0, 2, 3],
                   [1.0, 2.0, 3.0, 4.0, 5.0, 6.0])
    ca = CscMatrix(m, n, [0, 2, 3, 5, 6], [0, 2, 1, 0, 2, 2],
                   [1.0, 4.0, 3.0, 2.0, 5.0, 6.0])

    x = Vector(n, [1.0, 2.0, 3.0, 4.0])

    # The dense picture of the same matrix, for the header only.
    dense = [1.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0.0, 5.0, 6.0]

    print(f"A is {m} x {n} with 6 nonzeros:")
    print_grid(dense, m, n)
    print(f"x = {x.val}\n")

    print("--- csr_sparse_matvec (gather) ---")
    print(f"row_ptr = {ra.row_ptr}")
    print(f"col_idx = {ra.col_idx}")
    print(f"val = {ra.val}")
    print(f"y = {csr_sparse_matvec(ra, x).val}\n")

    print("--- csc_sparse_matvec (scatter) ---")
    print(f"col_ptr = {ca.col_ptr}")
    print(f"row_idx = {ca.row_idx}")
    print(f"val = {ca.val}")
    print(f"y = {csc_sparse_matvec(ca, x).val}\n")


def print_col_grid(col_major, m, n):
    """Print a column-major m x n matrix as a grid."""
    for i in range(m):
        print("".join(f"{col_major[j * m + i]:5g}" for j in range(n)))


def run_dense_lu_factor():
    print("=== Problem 11: Dense LU Factorization ===\n")

    # A 4 x 4 built as the product of integer L and U, so the factor comes
    # out in exact integers and the pivots are 2, 3, 4, 5:
    #
    #    2   4  -2   6
    #    4  11  -3   9
    #   -2   5   9 -13
    #    6   6  -4  31
    #
    n = 4
    col_major = [2.0, 4.0, -2.0, 6.0, 4.0, 11.0, 5.0, 6.0,
                 -2.0, -3.0, 9.0, -4.0, 6.0, 9.0, -13.0, 31.0]

    a = ColDenseMatrix(n, n, col_major)

    print(f"A is {n} x {n}:")
    print_col_grid(a.val, n, n)
    print(f"val = {a.val}\n")

    def show(name, lu):
        print(f"--- {name} ---")
        print("LU (U upper, L strict lower, unit diagonal implied):")
        print_col_grid(lu.val, n, n)
        print(f"val = {lu.val}\n")

    show("left_dense_lu_factor (gather)", left_dense_lu_factor(a))
    show("right_dense_lu_factor (scatter)", right_dense_lu_factor(a))


def run_dense_lu_solve():
    print("=== Problem 12: Dense LU Solve ===\n")

    # The factor problem 11 computes, in both layouts. U in the upper
    # triangle, L's multipliers in the strict lower, unit diagonal implied:
    #
    #    2   4  -2   6
    #    2   3   1  -3
    #   -1   3   4   2
    #    3  -2   1   5
    #
    n = 4
    row_major = [2.0, 4.0, -2.0, 6.0, 2.0, 3.0, 1.0, -3.0,
                 -1.0, 3.0, 4.0, 2.0, 3.0, -2.0, 1.0, 5.0]
    col_major = [2.0, 2.0, -1.0, 3.0, 4.0, 3.0, 3.0, -2.0,
                 -2.0, 1.0, 4.0, 1.0, 6.0, -3.0, 2.0, 5.0]

    rlu = RowDenseMatrix(n, n, row_major)
    clu = ColDenseMatrix(n, n, col_major)
    b = Vector(n, [28.0, 53.0, -17.0, 130.0])

    print(f"LU is {n} x {n}:")
    print_grid(row_major, n, n)
    print(f"b = {b.val}\n")

    print("--- row_dense_lu_solve (gather) ---")
    print(f"val = {rlu.val}")
    print(f"x = {row_dense_lu_solve(rlu, b).val}\n")

    print("--- col_dense_lu_solve (scatter) ---")
    print(f"val = {clu.val}")
    print(f"x = {col_dense_lu_solve(clu, b).val}\n")


def run_dense_convert():
    print("=== Problem 13: Dense Layout Conversion ===\n")

    # The 3 x 4 from problem 9:
    #
    #    1   2   3   4
    #    5   6   7   8
    #    9  10  11  12
    #
    m, n = 3, 4
    row_major = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0]
    col_major = [1.0, 5.0, 9.0, 2.0, 6.0, 10.0, 3.0, 7.0, 11.0, 4.0, 8.0, 12.0]

    ra = RowDenseMatrix(m, n, row_major)
    ca = ColDenseMatrix(m, n, col_major)

    print(f"A is {m} x {n}:")
    print_grid(row_major, m, n)
    print()

    print("--- row_to_col_dense_convert ---")
    print(f"in  (row-major) = {ra.val}")
    to_col = row_to_col_dense_convert(ra)
    print(f"out (col-major) = {to_col.val}")
    print(f"round trip      = {col_to_row_dense_convert(to_col).val}\n")

    print("--- col_to_row_dense_convert ---")
    print(f"in  (col-major) = {ca.val}")
    to_row = col_to_row_dense_convert(ca)
    print(f"out (row-major) = {to_row.val}")
    print(f"round trip      = {row_to_col_dense_convert(to_row).val}\n")


def run_sparse_convert():
    print("=== Problem 14: Sparse Format Conversion ===\n")

    # The 3 x 4 from problem 10, 6 nonzeros:
    #
    #    1   0   2   0
    #    0   3   0   0
    #    4   0   5   6
    #
    m, n = 3, 4
    ra = CsrMatrix(m, n, [0, 2, 3, 6], [0, 2, 1, 0, 2, 3],
                   [1.0, 2.0, 3.0, 4.0, 5.0, 6.0])
    ca = CscMatrix(m, n, [0, 2, 3, 5, 6], [0, 2, 1, 0, 2, 2],
                   [1.0, 4.0, 3.0, 2.0, 5.0, 6.0])

    dense = [1.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0.0, 5.0, 6.0]

    print(f"A is {m} x {n} with 6 nonzeros:")
    print_grid(dense, m, n)
    print()

    print("--- csr_to_csc_sparse_convert ---")
    print(f"in  row_ptr = {ra.row_ptr}")
    print(f"in  col_idx = {ra.col_idx}")
    print(f"in  val     = {ra.val}")
    to_csc = csr_to_csc_sparse_convert(ra)
    print(f"out col_ptr = {to_csc.col_ptr}")
    print(f"out row_idx = {to_csc.row_idx}")
    print(f"out val     = {to_csc.val}")
    back_csr = csc_to_csr_sparse_convert(to_csc)
    print(f"rt  row_ptr = {back_csr.row_ptr}")
    print(f"rt  col_idx = {back_csr.col_idx}")
    print(f"rt  val     = {back_csr.val}\n")

    print("--- csc_to_csr_sparse_convert ---")
    print(f"in  col_ptr = {ca.col_ptr}")
    print(f"in  row_idx = {ca.row_idx}")
    print(f"in  val     = {ca.val}")
    to_csr = csc_to_csr_sparse_convert(ca)
    print(f"out row_ptr = {to_csr.row_ptr}")
    print(f"out col_idx = {to_csr.col_idx}")
    print(f"out val     = {to_csr.val}")
    back_csc = csr_to_csc_sparse_convert(to_csr)
    print(f"rt  col_ptr = {back_csc.col_ptr}")
    print(f"rt  row_idx = {back_csc.row_idx}")
    print(f"rt  val     = {back_csc.val}\n")


if __name__ == "__main__":
    run_trackers()
    run_lru_cache()
    run_next_greater()
    run_merge_k_sorted()
    run_time_kv_store()
    run_first_duplicate()
    run_prefix_trie()
    run_union_find()
    run_dense_matvec()
    run_sparse_matvec()
    run_dense_lu_factor()
    run_dense_lu_solve()
    run_dense_convert()
    run_sparse_convert()
