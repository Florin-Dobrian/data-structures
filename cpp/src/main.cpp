#include <iostream>
#include <iomanip>
#include <vector>
#include "avg_tracker.h"
#include "lru_cache.h"
#include "next_greater.h"
#include "merge_k_sorted.h"
#include "time_kv_store.h"
#include "first_duplicate.h"
#include "prefix_trie.h"
#include "union_find.h"
#include "dense_matvec.h"
#include "sparse_matvec.h"
#include "dense_lu_factor.h"
#include "dense_lu_solve.h"
#include "dense_convert.h"
#include "sparse_convert.h"

void run_trackers() {
    std::cout << "=== Problem 1: Moving Average Tracker ===\n" << std::endl;
    int size = 3;
    std::vector<double> data = {10.0, 20.0, 30.0, 40.0, 50.0};

    DequeTracker dt(size);
    std::cout << "--- DequeTracker ---" << std::endl;
    for (double val : data) {
        dt.add(val);
        std::cout << "Added " << val << ": Average = " << dt.get_average() << std::endl;
    }

    std::cout << std::endl;

    CircularBufferTracker cbt(size);
    std::cout << "--- CircularBufferTracker ---" << std::endl;
    for (double val : data) {
        cbt.add(val);
        std::cout << "Added " << val << ": Average = " << cbt.get_average() << std::endl;
    }

    std::cout << std::endl;
}

void run_lru_cache() {
    std::cout << "=== Problem 2: LRU Cache ===\n" << std::endl;

    auto test = [](auto& cache, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        cache.put(1, 1);
        cache.put(2, 2);
        std::cout << "get(1) = " << cache.get(1) << std::endl;
        cache.put(3, 3);  // evicts key 2
        std::cout << "get(2) = " << cache.get(2) << std::endl;
        cache.put(4, 4);  // evicts key 1
        std::cout << "get(1) = " << cache.get(1) << std::endl;
        std::cout << "get(3) = " << cache.get(3) << std::endl;
        std::cout << "get(4) = " << cache.get(4) << std::endl;
        std::cout << std::endl;
    };

    ListLRUCache lc(2);
    test(lc, "ListLRUCache");

    ManualLRUCache mc(2);
    test(mc, "ManualLRUCache");
}

void run_next_greater() {
    std::cout << "=== Problem 3: Next Greater Element ===\n" << std::endl;
    std::vector<int> data = {4, 5, 2, 25, 7, 18};

    auto print_result = [&](const std::string& name, const std::vector<int>& result) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "Input:  [";
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
        std::cout << "Output: [";
        for (size_t i = 0; i < result.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << result[i];
        }
        std::cout << "]" << std::endl << std::endl;
    };

    print_result("next_greater_right_to_left", next_greater_right_to_left(data));
    print_result("next_greater_left_to_right", next_greater_left_to_right(data));
    print_result("next_greater_right_to_left_manual", next_greater_right_to_left_manual(data));
    print_result("next_greater_left_to_right_manual", next_greater_left_to_right_manual(data));
}

void run_merge_k_sorted() {
    std::cout << "=== Problem 4: Merge K Sorted Lists ===\n" << std::endl;
    std::vector<std::vector<int>> lists = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};

    auto print_result = [&](const std::string& name, const std::vector<int>& result) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "Input:  [";
        for (size_t i = 0; i < lists.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << "[";
            for (size_t j = 0; j < lists[i].size(); j++) {
                if (j > 0) std::cout << ", ";
                std::cout << lists[i][j];
            }
            std::cout << "]";
        }
        std::cout << "]" << std::endl;
        std::cout << "Output: [";
        for (size_t i = 0; i < result.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << result[i];
        }
        std::cout << "]" << std::endl << std::endl;
    };

    print_result("merge_k_priority_queue", merge_k_priority_queue(lists));
    print_result("merge_k_manual_heap", merge_k_manual_heap(lists));
}

void run_time_kv_store() {
    std::cout << "=== Problem 5: Time-Based Key-Value Store ===\n" << std::endl;

    auto test = [](auto& kv, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        kv.set("alice", "alice_v1", 1);
        kv.set("alice", "alice_v2", 4);
        kv.set("alice", "alice_v3", 7);
        kv.set("bob",   "bob_v1",   2);
        kv.set("bob",   "bob_v2",   5);

        std::cout << "get(alice, 0) = \"" << kv.get("alice", 0) << "\"" << std::endl;
        std::cout << "get(alice, 1) = \"" << kv.get("alice", 1) << "\"" << std::endl;
        std::cout << "get(alice, 3) = \"" << kv.get("alice", 3) << "\"" << std::endl;
        std::cout << "get(alice, 4) = \"" << kv.get("alice", 4) << "\"" << std::endl;
        std::cout << "get(alice, 6) = \"" << kv.get("alice", 6) << "\"" << std::endl;
        std::cout << "get(alice, 9) = \"" << kv.get("alice", 9) << "\"" << std::endl;
        std::cout << "get(bob,   3) = \"" << kv.get("bob",   3) << "\"" << std::endl;
        std::cout << "get(bob,   5) = \"" << kv.get("bob",   5) << "\"" << std::endl;
        std::cout << "get(carol, 1) = \"" << kv.get("carol", 1) << "\"" << std::endl;
        std::cout << std::endl;
    };

    StdMapTimeKV sm;
    test(sm, "StdMapTimeKV");

    ManualBinarySearchTimeKV mb;
    test(mb, "ManualBinarySearchTimeKV");
}

void run_first_duplicate() {
    std::cout << "=== Problem 6: First Duplicate in a Stream ===\n" << std::endl;

    struct TestCase { std::string label; std::vector<int> data; };
    std::vector<TestCase> cases = {
        {"has duplicate",    {2, 1, 3, 5, 3, 2}},
        {"immediate repeat", {7, 7, 1, 2}},
        {"no duplicate",     {1, 2, 3, 4, 5}},
        {"empty",            {}},
    };

    auto print_vec = [](const std::vector<int>& v) {
        std::cout << "[";
        for (size_t i = 0; i < v.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << v[i];
        }
        std::cout << "]";
    };

    auto run = [&](const std::string& name, int(*fn)(const std::vector<int>&)) {
        std::cout << "--- " << name << " ---" << std::endl;
        for (auto& tc : cases) {
            std::cout << tc.label << ": ";
            print_vec(tc.data);
            std::cout << " -> " << fn(tc.data) << std::endl;
        }
        std::cout << std::endl;
    };

    run("first_duplicate_hash_set", first_duplicate_hash_set);
    run("first_duplicate_sorted_set", first_duplicate_sorted_set);
}

void run_prefix_trie() {
    std::cout << "=== Problem 7: Prefix Trie ===\n" << std::endl;

    auto test = [](auto& trie, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        trie.insert("apple");
        trie.insert("app");
        trie.insert("apricot");
        trie.insert("banana");

        std::cout << "search(apple)   = " << std::boolalpha << trie.search("apple")   << std::endl;
        std::cout << "search(app)     = " << std::boolalpha << trie.search("app")     << std::endl;
        std::cout << "search(ap)      = " << std::boolalpha << trie.search("ap")      << std::endl;
        std::cout << "search(banana)  = " << std::boolalpha << trie.search("banana")  << std::endl;
        std::cout << "search(band)    = " << std::boolalpha << trie.search("band")    << std::endl;
        std::cout << "starts_with(ap) = " << std::boolalpha << trie.starts_with("ap") << std::endl;
        std::cout << "starts_with(ba) = " << std::boolalpha << trie.starts_with("ba") << std::endl;
        std::cout << "starts_with(ca) = " << std::boolalpha << trie.starts_with("ca") << std::endl;
        std::cout << std::endl;
    };

    HashMapTrie hm;
    test(hm, "HashMapTrie");

    ArrayTrie at;
    test(at, "ArrayTrie");
}

void run_union_find() {
    std::cout << "=== Problem 8: Connected Components (Union-Find) ===\n" << std::endl;

    // Graph: 5 nodes, edges form two components: {0,1,2} and {3,4}
    int n = 5;
    std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {3,4}};

    auto test = [&](auto& uf, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "Nodes: " << n << ", Edges: [";
        for (size_t i = 0; i < edges.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << "(" << edges[i].first << "," << edges[i].second << ")";
        }
        std::cout << "]" << std::endl;

        for (auto& [a, b] : edges) {
            uf.unite(a, b);
        }

        std::cout << "Components: " << uf.count() << std::endl;
        std::cout << "find(0) = " << uf.find(0) << ", find(2) = " << uf.find(2)
                  << " (same: " << std::boolalpha << (uf.find(0) == uf.find(2)) << ")" << std::endl;
        std::cout << "find(0) = " << uf.find(0) << ", find(3) = " << uf.find(3)
                  << " (same: " << std::boolalpha << (uf.find(0) == uf.find(3)) << ")" << std::endl;
        std::cout << std::endl;
    };

    NaiveUnionFind nuf(n);
    test(nuf, "NaiveUnionFind");

    RankedUnionFind ruf(n);
    test(ruf, "RankedUnionFind");
}

// Print a flat buffer as a bracketed list.
void print_val(const std::string& name, const std::vector<double>& val) {
    std::cout << name << " = [";
    for (std::size_t p = 0; p < val.size(); p++) {
        if (p > 0) std::cout << ", ";
        std::cout << val[p];
    }
    std::cout << "]" << std::endl;
}

// Print an m x n matrix as a grid, given its entries in row-major order.
void print_grid(const std::vector<double>& rowMajor, std::size_t m, std::size_t n) {
    for (std::size_t i = 0; i < m; i++) {
        for (std::size_t j = 0; j < n; j++) {
            std::cout << std::setw(5) << rowMajor[i * n + j];
        }
        std::cout << std::endl;
    }
}

void run_dense_matvec() {
    std::cout << "=== Problem 9: Dense Matrix-Vector Product ===\n" << std::endl;

    std::size_t m = 3, n = 4;
    std::vector<double> rowMajor = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<double> colMajor = {1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12};

    RowDenseMatrix ra(m, n, rowMajor);
    ColDenseMatrix ca(m, n, colMajor);
    Vector x(n, {1.0, 2.0, 3.0, 4.0});

    std::cout << "A is " << m << " x " << n << ":" << std::endl;
    print_grid(rowMajor, m, n);
    print_val("x", x.val);
    std::cout << std::endl;

    std::cout << "--- row_dense_matvec (gather) ---" << std::endl;
    print_val("val", ra.val);
    print_val("y", row_dense_matvec(ra, x).val);
    std::cout << std::endl;

    std::cout << "--- col_dense_matvec (scatter) ---" << std::endl;
    print_val("val", ca.val);
    print_val("y", col_dense_matvec(ca, x).val);
    std::cout << std::endl;
}

void run_sparse_matvec() {
    std::cout << "=== Problem 10: Sparse Matrix-Vector Product ===\n" << std::endl;

    std::size_t m = 3, n = 4;

    CsrMatrix ra(m, n, {0, 2, 3, 6}, {0, 2, 1, 0, 2, 3}, {1, 2, 3, 4, 5, 6});
    CscMatrix ca(m, n, {0, 2, 3, 5, 6}, {0, 2, 1, 0, 2, 2}, {1, 4, 3, 2, 5, 6});

    Vector x(n, {1.0, 2.0, 3.0, 4.0});

    // The dense picture of the same matrix, for the header only.
    std::vector<double> dense = {1, 0, 2, 0, 0, 3, 0, 0, 4, 0, 5, 6};

    auto print_idx = [](const std::string& name, const auto& idx) {
        std::cout << name << " = [";
        for (std::size_t p = 0; p < idx.size(); p++) {
            if (p > 0) std::cout << ", ";
            std::cout << idx[p];
        }
        std::cout << "]" << std::endl;
    };

    std::cout << "A is " << m << " x " << n << " with 6 nonzeros:" << std::endl;
    print_grid(dense, m, n);
    print_val("x", x.val);
    std::cout << std::endl;

    std::cout << "--- csr_sparse_matvec (gather) ---" << std::endl;
    print_idx("rowPtr", ra.rowPtr);
    print_idx("colIdx", ra.colIdx);
    print_val("val", ra.val);
    print_val("y", csr_sparse_matvec(ra, x).val);
    std::cout << std::endl;

    std::cout << "--- csc_sparse_matvec (scatter) ---" << std::endl;
    print_idx("colPtr", ca.colPtr);
    print_idx("rowIdx", ca.rowIdx);
    print_val("val", ca.val);
    print_val("y", csc_sparse_matvec(ca, x).val);
    std::cout << std::endl;
}

// Print a column-major m x n matrix as a grid.
void print_col_grid(const std::vector<double>& colMajor, std::size_t m, std::size_t n) {
    for (std::size_t i = 0; i < m; i++) {
        for (std::size_t j = 0; j < n; j++) {
            std::cout << std::setw(5) << colMajor[j * m + i];
        }
        std::cout << std::endl;
    }
}

void run_dense_lu_factor() {
    std::cout << "=== Problem 11: Dense LU Factorization ===\n" << std::endl;

    // A 4 x 4 built as the product of integer L and U, so the factor comes
    // out in exact integers and the pivots are 2, 3, 4, 5:
    //
    //    2   4  -2   6
    //    4  11  -3   9
    //   -2   5   9 -13
    //    6   6  -4  31
    //
    std::size_t n = 4;
    std::vector<double> colMajor = {2, 4, -2, 6, 4, 11, 5, 6, -2, -3, 9, -4, 6, 9, -13, 31};

    ColDenseMatrix a(n, n, colMajor);

    std::cout << "A is " << n << " x " << n << ":" << std::endl;
    print_col_grid(a.val, n, n);
    print_val("val", a.val);
    std::cout << std::endl;

    auto show = [&](const std::string& name, const ColDenseMatrix& lu) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "LU (U upper, L strict lower, unit diagonal implied):" << std::endl;
        print_col_grid(lu.val, n, n);
        print_val("val", lu.val);
        std::cout << std::endl;
    };

    show("left_dense_lu_factor (gather)", left_dense_lu_factor(a));
    show("right_dense_lu_factor (scatter)", right_dense_lu_factor(a));
}

void run_dense_lu_solve() {
    std::cout << "=== Problem 12: Dense LU Solve ===\n" << std::endl;

    // The factor problem 11 computes, in both layouts. U in the upper
    // triangle, L's multipliers in the strict lower, unit diagonal implied:
    //
    //    2   4  -2   6
    //    2   3   1  -3
    //   -1   3   4   2
    //    3  -2   1   5
    //
    std::size_t n = 4;
    std::vector<double> rowMajor = {2, 4, -2, 6, 2, 3, 1, -3, -1, 3, 4, 2, 3, -2, 1, 5};
    std::vector<double> colMajor = {2, 2, -1, 3, 4, 3, 3, -2, -2, 1, 4, 1, 6, -3, 2, 5};

    RowDenseMatrix rlu(n, n, rowMajor);
    ColDenseMatrix clu(n, n, colMajor);
    Vector b(n, {28.0, 53.0, -17.0, 130.0});

    std::cout << "LU is " << n << " x " << n << ":" << std::endl;
    print_grid(rowMajor, n, n);
    print_val("b", b.val);
    std::cout << std::endl;

    std::cout << "--- row_dense_lu_solve (gather) ---" << std::endl;
    print_val("val", rlu.val);
    print_val("x", row_dense_lu_solve(rlu, b).val);
    std::cout << std::endl;

    std::cout << "--- col_dense_lu_solve (scatter) ---" << std::endl;
    print_val("val", clu.val);
    print_val("x", col_dense_lu_solve(clu, b).val);
    std::cout << std::endl;
}

void run_dense_convert() {
    std::cout << "=== Problem 13: Dense Layout Conversion ===\n" << std::endl;

    // The 3 x 4 from problem 9:
    //
    //    1   2   3   4
    //    5   6   7   8
    //    9  10  11  12
    //
    std::size_t m = 3, n = 4;
    std::vector<double> rowMajor = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    std::vector<double> colMajor = {1, 5, 9, 2, 6, 10, 3, 7, 11, 4, 8, 12};

    RowDenseMatrix ra(m, n, rowMajor);
    ColDenseMatrix ca(m, n, colMajor);

    std::cout << "A is " << m << " x " << n << ":" << std::endl;
    print_grid(rowMajor, m, n);
    std::cout << std::endl;

    std::cout << "--- row_to_col_dense_convert ---" << std::endl;
    print_val("in  (row-major)", ra.val);
    const ColDenseMatrix toCol = row_to_col_dense_convert(ra);
    print_val("out (col-major)", toCol.val);
    print_val("round trip     ", col_to_row_dense_convert(toCol).val);
    std::cout << std::endl;

    std::cout << "--- col_to_row_dense_convert ---" << std::endl;
    print_val("in  (col-major)", ca.val);
    const RowDenseMatrix toRow = col_to_row_dense_convert(ca);
    print_val("out (row-major)", toRow.val);
    print_val("round trip     ", row_to_col_dense_convert(toRow).val);
    std::cout << std::endl;
}

void run_sparse_convert() {
    std::cout << "=== Problem 14: Sparse Format Conversion ===\n" << std::endl;

    // The 3 x 4 from problem 10, 6 nonzeros:
    //
    //    1   0   2   0
    //    0   3   0   0
    //    4   0   5   6
    //
    std::size_t m = 3, n = 4;
    CsrMatrix ra(m, n, {0, 2, 3, 6}, {0, 2, 1, 0, 2, 3}, {1, 2, 3, 4, 5, 6});
    CscMatrix ca(m, n, {0, 2, 3, 5, 6}, {0, 2, 1, 0, 2, 2}, {1, 4, 3, 2, 5, 6});

    std::vector<double> dense = {1, 0, 2, 0, 0, 3, 0, 0, 4, 0, 5, 6};

    auto print_idx = [](const std::string& name, const auto& idx) {
        std::cout << name << " = [";
        for (std::size_t p = 0; p < idx.size(); p++) {
            if (p > 0) std::cout << ", ";
            std::cout << idx[p];
        }
        std::cout << "]" << std::endl;
    };

    std::cout << "A is " << m << " x " << n << " with 6 nonzeros:" << std::endl;
    print_grid(dense, m, n);
    std::cout << std::endl;

    std::cout << "--- csr_to_csc_sparse_convert ---" << std::endl;
    print_idx("in  rowPtr", ra.rowPtr);
    print_idx("in  colIdx", ra.colIdx);
    print_val("in  val   ", ra.val);
    const CscMatrix toCsc = csr_to_csc_sparse_convert(ra);
    print_idx("out colPtr", toCsc.colPtr);
    print_idx("out rowIdx", toCsc.rowIdx);
    print_val("out val   ", toCsc.val);
    const CsrMatrix backCsr = csc_to_csr_sparse_convert(toCsc);
    print_idx("rt  rowPtr", backCsr.rowPtr);
    print_idx("rt  colIdx", backCsr.colIdx);
    print_val("rt  val   ", backCsr.val);
    std::cout << std::endl;

    std::cout << "--- csc_to_csr_sparse_convert ---" << std::endl;
    print_idx("in  colPtr", ca.colPtr);
    print_idx("in  rowIdx", ca.rowIdx);
    print_val("in  val   ", ca.val);
    const CsrMatrix toCsr = csc_to_csr_sparse_convert(ca);
    print_idx("out rowPtr", toCsr.rowPtr);
    print_idx("out colIdx", toCsr.colIdx);
    print_val("out val   ", toCsr.val);
    const CscMatrix backCsc = csr_to_csc_sparse_convert(toCsr);
    print_idx("rt  colPtr", backCsc.colPtr);
    print_idx("rt  rowIdx", backCsc.rowIdx);
    print_val("rt  val   ", backCsc.val);
    std::cout << std::endl;
}

int main() {
    run_trackers();
    run_lru_cache();
    run_next_greater();
    run_merge_k_sorted();
    run_time_kv_store();
    run_first_duplicate();
    run_prefix_trie();
    run_union_find();
    run_dense_matvec();
    run_sparse_matvec();
    run_dense_lu_factor();
    run_dense_lu_solve();
    run_dense_convert();
    run_sparse_convert();
    return 0;
}
