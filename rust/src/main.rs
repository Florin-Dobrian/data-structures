mod avg_tracker;
mod lru_cache;
mod next_greater;
mod merge_k_sorted;
mod time_kv_store;
mod first_duplicate;
mod prefix_trie;
mod union_find;
mod dense_matvec;
mod sparse_matvec;
mod dense_lu_factor;
mod dense_lu_solve;
mod dense_convert;
mod sparse_convert;

use avg_tracker::{DequeTracker, CircularBufferTracker};
use lru_cache::{SimpleVecLRUCache, ManualLRUCache};
use next_greater::{
    next_greater_right_to_left, next_greater_left_to_right,
    next_greater_right_to_left_manual, next_greater_left_to_right_manual,
};
use merge_k_sorted::{merge_k_binary_heap, merge_k_manual_heap};
use time_kv_store::{BTreeMapTimeKV, ManualBinarySearchTimeKV};
use first_duplicate::{first_duplicate_hash_set, first_duplicate_sorted_set};
use prefix_trie::{HashMapTrie, ArrayTrie};
use dense_matvec::{
    Vector, RowDenseMatrix, ColDenseMatrix,
    row_dense_matvec, col_dense_matvec,
};
use sparse_matvec::{
    CsrMatrix, CscMatrix, csr_sparse_matvec, csc_sparse_matvec,
};
use dense_lu_factor::{left_dense_lu_factor, right_dense_lu_factor};
use dense_lu_solve::{row_dense_lu_solve, col_dense_lu_solve};
use dense_convert::{row_to_col_dense_convert, col_to_row_dense_convert};
use sparse_convert::{csr_to_csc_sparse_convert, csc_to_csr_sparse_convert};
use union_find::{NaiveUnionFind, RankedUnionFind};

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

fn run_prefix_trie() {
    println!("=== Problem 7: Prefix Trie ===\n");

    macro_rules! test_trie {
        ($name:expr, $trie:expr) => {{
            let trie = $trie;
            println!("--- {} ---", $name);
            trie.insert("apple");
            trie.insert("app");
            trie.insert("apricot");
            trie.insert("banana");

            println!("search(apple)   = {}", trie.search("apple"));
            println!("search(app)     = {}", trie.search("app"));
            println!("search(ap)      = {}", trie.search("ap"));
            println!("search(banana)  = {}", trie.search("banana"));
            println!("search(band)    = {}", trie.search("band"));
            println!("starts_with(ap) = {}", trie.starts_with("ap"));
            println!("starts_with(ba) = {}", trie.starts_with("ba"));
            println!("starts_with(ca) = {}", trie.starts_with("ca"));
            println!();
        }};
    }

    test_trie!("HashMapTrie", &mut HashMapTrie::new());
    test_trie!("ArrayTrie", &mut ArrayTrie::new());
}

fn run_union_find() {
    println!("=== Problem 8: Connected Components (Union-Find) ===\n");

    let n = 5;
    let edges = vec![(0, 1), (1, 2), (3, 4)];

    // NaiveUnionFind: find() takes &self (no path compression)
    {
        let mut uf = NaiveUnionFind::new(n);
        println!("--- NaiveUnionFind ---");
        println!("Nodes: {}, Edges: {:?}", n, edges);
        for &(a, b) in &edges {
            uf.unite(a, b);
        }
        println!("Components: {}", uf.count());
        let (f0, f2) = (uf.find(0), uf.find(2));
        println!("find(0) = {}, find(2) = {} (same: {})", f0, f2, f0 == f2);
        let (f0, f3) = (uf.find(0), uf.find(3));
        println!("find(0) = {}, find(3) = {} (same: {})", f0, f3, f0 == f3);
        println!();
    }

    // RankedUnionFind: find() takes &mut self (path compression mutates)
    {
        let mut uf = RankedUnionFind::new(n);
        println!("--- RankedUnionFind ---");
        println!("Nodes: {}, Edges: {:?}", n, edges);
        for &(a, b) in &edges {
            uf.unite(a, b);
        }
        println!("Components: {}", uf.count());
        let f0 = uf.find(0);
        let f2 = uf.find(2);
        println!("find(0) = {}, find(2) = {} (same: {})", f0, f2, f0 == f2);
        let f0 = uf.find(0);
        let f3 = uf.find(3);
        println!("find(0) = {}, find(3) = {} (same: {})", f0, f3, f0 == f3);
        println!();
    }
}

fn fmt_f64(v: &[f64]) -> String {
    v.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(", ")
}

/// A Vector prints itself over its own size rather than its buffer length.
fn fmt_vector(v: &Vector) -> String {
    (0..v.size).map(|i| v.val[i].to_string()).collect::<Vec<_>>().join(", ")
}

fn fmt_usize(v: &[usize]) -> String {
    v.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(", ")
}

fn fmt_i32(v: &[i32]) -> String {
    v.iter().map(|x| x.to_string()).collect::<Vec<_>>().join(", ")
}

/// Print an m x n matrix as a grid, given its entries in row-major order.
fn print_grid(row_major: &[f64], m: usize, n: usize) {
    for i in 0..m {
        let row: String = (0..n).map(|j| format!("{:5}", row_major[i * n + j])).collect();
        println!("{}", row);
    }
}

fn run_dense_matvec() {
    println!("=== Problem 9: Dense Matrix-Vector Product ===\n");

    let m = 3;
    let n = 4;
    let row_major = vec![1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0];
    let col_major = vec![1.0, 5.0, 9.0, 2.0, 6.0, 10.0, 3.0, 7.0, 11.0, 4.0, 8.0, 12.0];

    let ra = RowDenseMatrix { m_size: m, n_size: n, val: row_major.clone() };
    let ca = ColDenseMatrix { m_size: m, n_size: n, val: col_major };
    let x = Vector::from_val(n, vec![1.0, 2.0, 3.0, 4.0]);

    println!("A is {} x {}:", m, n);
    print_grid(&row_major, m, n);
    println!("x = [{}]\n", fmt_vector(&x));

    println!("--- row_dense_matvec (gather) ---");
    println!("val = [{}]", fmt_f64(&ra.val));
    println!("y = [{}]\n", fmt_vector(&row_dense_matvec(&ra, &x)));

    println!("--- col_dense_matvec (scatter) ---");
    println!("val = [{}]", fmt_f64(&ca.val));
    println!("y = [{}]\n", fmt_vector(&col_dense_matvec(&ca, &x)));
}

fn run_sparse_matvec() {
    println!("=== Problem 10: Sparse Matrix-Vector Product ===\n");

    let m = 3;
    let n = 4;

    let ra = CsrMatrix {
        m_size: m,
        n_size: n,
        row_ptr: vec![0, 2, 3, 6],
        col_idx: vec![0, 2, 1, 0, 2, 3],
        val: vec![1.0, 2.0, 3.0, 4.0, 5.0, 6.0],
    };

    let ca = CscMatrix {
        m_size: m,
        n_size: n,
        col_ptr: vec![0, 2, 3, 5, 6],
        row_idx: vec![0, 2, 1, 0, 2, 2],
        val: vec![1.0, 4.0, 3.0, 2.0, 5.0, 6.0],
    };

    let x = Vector::from_val(n, vec![1.0, 2.0, 3.0, 4.0]);

    // The dense picture of the same matrix, for the header only.
    let dense = vec![1.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0.0, 5.0, 6.0];

    println!("A is {} x {} with 6 nonzeros:", ra.m_size, ra.n_size);
    print_grid(&dense, m, n);
    println!("x = [{}]\n", fmt_vector(&x));

    println!("--- csr_sparse_matvec (gather) ---");
    println!("rowPtr = [{}]", fmt_usize(&ra.row_ptr));
    println!("colIdx = [{}]", fmt_i32(&ra.col_idx));
    println!("val = [{}]", fmt_f64(&ra.val));
    println!("y = [{}]\n", fmt_vector(&csr_sparse_matvec(&ra, &x)));

    println!("--- csc_sparse_matvec (scatter) ---");
    println!("colPtr = [{}]", fmt_usize(&ca.col_ptr));
    println!("rowIdx = [{}]", fmt_i32(&ca.row_idx));
    println!("val = [{}]", fmt_f64(&ca.val));
    println!("y = [{}]\n", fmt_vector(&csc_sparse_matvec(&ca, &x)));
}

/// Print a column-major m x n matrix as a grid.
fn print_col_grid(col_major: &[f64], m: usize, n: usize) {
    for i in 0..m {
        let row: String = (0..n).map(|j| format!("{:5}", col_major[j * m + i])).collect();
        println!("{}", row);
    }
}

fn run_dense_lu_factor() {
    println!("=== Problem 11: Dense LU Factorization ===\n");

    // A 4 x 4 built as the product of integer L and U, so the factor comes
    // out in exact integers and the pivots are 2, 3, 4, 5:
    //
    //    2   4  -2   6
    //    4  11  -3   9
    //   -2   5   9 -13
    //    6   6  -4  31
    //
    let n = 4;
    let col_major = vec![2.0, 4.0, -2.0, 6.0, 4.0, 11.0, 5.0, 6.0,
                         -2.0, -3.0, 9.0, -4.0, 6.0, 9.0, -13.0, 31.0];

    let a = ColDenseMatrix { m_size: n, n_size: n, val: col_major };

    println!("A is {} x {}:", n, n);
    print_col_grid(&a.val, n, n);
    println!("val = [{}]\n", fmt_f64(&a.val));

    let show = |name: &str, lu: ColDenseMatrix| {
        println!("--- {} ---", name);
        println!("LU (U upper, L strict lower, unit diagonal implied):");
        print_col_grid(&lu.val, n, n);
        println!("val = [{}]\n", fmt_f64(&lu.val));
    };

    show("left_dense_lu_factor (gather)", left_dense_lu_factor(&a));
    show("right_dense_lu_factor (scatter)", right_dense_lu_factor(&a));
}

fn run_dense_lu_solve() {
    println!("=== Problem 12: Dense LU Solve ===\n");

    // The factor problem 11 computes, in both layouts. U in the upper
    // triangle, L's multipliers in the strict lower, unit diagonal implied:
    //
    //    2   4  -2   6
    //    2   3   1  -3
    //   -1   3   4   2
    //    3  -2   1   5
    //
    let n = 4;
    let row_major = vec![2.0, 4.0, -2.0, 6.0, 2.0, 3.0, 1.0, -3.0,
                         -1.0, 3.0, 4.0, 2.0, 3.0, -2.0, 1.0, 5.0];
    let col_major = vec![2.0, 2.0, -1.0, 3.0, 4.0, 3.0, 3.0, -2.0,
                         -2.0, 1.0, 4.0, 1.0, 6.0, -3.0, 2.0, 5.0];

    let rlu = RowDenseMatrix { m_size: n, n_size: n, val: row_major.clone() };
    let clu = ColDenseMatrix { m_size: n, n_size: n, val: col_major };
    let b = Vector::from_val(n, vec![28.0, 53.0, -17.0, 130.0]);

    println!("LU is {} x {}:", n, n);
    print_grid(&row_major, n, n);
    println!("b = [{}]\n", fmt_vector(&b));

    println!("--- row_dense_lu_solve (gather) ---");
    println!("val = [{}]", fmt_f64(&rlu.val));
    println!("x = [{}]\n", fmt_vector(&row_dense_lu_solve(&rlu, &b)));

    println!("--- col_dense_lu_solve (scatter) ---");
    println!("val = [{}]", fmt_f64(&clu.val));
    println!("x = [{}]\n", fmt_vector(&col_dense_lu_solve(&clu, &b)));
}

fn run_dense_convert() {
    println!("=== Problem 13: Dense Layout Conversion ===\n");

    // The 3 x 4 from problem 9:
    //
    //    1   2   3   4
    //    5   6   7   8
    //    9  10  11  12
    //
    let m = 3;
    let n = 4;
    let row_major = vec![1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0];
    let col_major = vec![1.0, 5.0, 9.0, 2.0, 6.0, 10.0, 3.0, 7.0, 11.0, 4.0, 8.0, 12.0];

    let ra = RowDenseMatrix { m_size: m, n_size: n, val: row_major.clone() };
    let ca = ColDenseMatrix { m_size: m, n_size: n, val: col_major };

    println!("A is {} x {}:", m, n);
    print_grid(&row_major, m, n);
    println!();

    println!("--- row_to_col_dense_convert ---");
    println!("in  (row-major) = [{}]", fmt_f64(&ra.val));
    let to_col = row_to_col_dense_convert(&ra);
    println!("out (col-major) = [{}]", fmt_f64(&to_col.val));
    println!("round trip      = [{}]\n", fmt_f64(&col_to_row_dense_convert(&to_col).val));

    println!("--- col_to_row_dense_convert ---");
    println!("in  (col-major) = [{}]", fmt_f64(&ca.val));
    let to_row = col_to_row_dense_convert(&ca);
    println!("out (row-major) = [{}]", fmt_f64(&to_row.val));
    println!("round trip      = [{}]\n", fmt_f64(&row_to_col_dense_convert(&to_row).val));
}

fn run_sparse_convert() {
    println!("=== Problem 14: Sparse Format Conversion ===\n");

    // The 3 x 4 from problem 10, 6 nonzeros:
    //
    //    1   0   2   0
    //    0   3   0   0
    //    4   0   5   6
    //
    let m = 3;
    let n = 4;
    let ra = CsrMatrix {
        m_size: m,
        n_size: n,
        row_ptr: vec![0, 2, 3, 6],
        col_idx: vec![0, 2, 1, 0, 2, 3],
        val: vec![1.0, 2.0, 3.0, 4.0, 5.0, 6.0],
    };
    let ca = CscMatrix {
        m_size: m,
        n_size: n,
        col_ptr: vec![0, 2, 3, 5, 6],
        row_idx: vec![0, 2, 1, 0, 2, 2],
        val: vec![1.0, 4.0, 3.0, 2.0, 5.0, 6.0],
    };

    let dense = vec![1.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0.0, 5.0, 6.0];

    println!("A is {} x {} with 6 nonzeros:", m, n);
    print_grid(&dense, m, n);
    println!();

    println!("--- csr_to_csc_sparse_convert ---");
    println!("in  rowPtr = [{}]", fmt_usize(&ra.row_ptr));
    println!("in  colIdx = [{}]", fmt_i32(&ra.col_idx));
    println!("in  val    = [{}]", fmt_f64(&ra.val));
    let to_csc = csr_to_csc_sparse_convert(&ra);
    println!("out colPtr = [{}]", fmt_usize(&to_csc.col_ptr));
    println!("out rowIdx = [{}]", fmt_i32(&to_csc.row_idx));
    println!("out val    = [{}]", fmt_f64(&to_csc.val));
    let back_csr = csc_to_csr_sparse_convert(&to_csc);
    println!("rt  rowPtr = [{}]", fmt_usize(&back_csr.row_ptr));
    println!("rt  colIdx = [{}]", fmt_i32(&back_csr.col_idx));
    println!("rt  val    = [{}]\n", fmt_f64(&back_csr.val));

    println!("--- csc_to_csr_sparse_convert ---");
    println!("in  colPtr = [{}]", fmt_usize(&ca.col_ptr));
    println!("in  rowIdx = [{}]", fmt_i32(&ca.row_idx));
    println!("in  val    = [{}]", fmt_f64(&ca.val));
    let to_csr = csc_to_csr_sparse_convert(&ca);
    println!("out rowPtr = [{}]", fmt_usize(&to_csr.row_ptr));
    println!("out colIdx = [{}]", fmt_i32(&to_csr.col_idx));
    println!("out val    = [{}]", fmt_f64(&to_csr.val));
    let back_csc = csr_to_csc_sparse_convert(&to_csr);
    println!("rt  colPtr = [{}]", fmt_usize(&back_csc.col_ptr));
    println!("rt  rowIdx = [{}]", fmt_i32(&back_csc.row_idx));
    println!("rt  val    = [{}]\n", fmt_f64(&back_csc.val));
}

fn main() {
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
}
