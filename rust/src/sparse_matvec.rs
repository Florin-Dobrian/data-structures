use crate::dense_matvec::Vector;

/// An m x n sparse matrix in CSR (Compressed Sparse Row).
///
/// Three vectors. row_ptr has m_size + 1 entries: row i occupies positions
/// row_ptr[i] up to row_ptr[i + 1] in the other two. col_idx holds the
/// column of each stored entry, val holds its value. Entries within a row
/// are in increasing column order here, though nothing in the algorithm
/// requires it.
///
/// row_ptr holds positions, so it is usize. col_idx holds indices, so it is
/// i32: an index array is as long as the matrix has nonzeros, and halving
/// its width is the whole reason for the cap.
pub struct CsrMatrix {
    pub m_size: usize,
    pub n_size: usize,
    pub row_ptr: Vec<usize>,
    pub col_idx: Vec<i32>,
    pub val: Vec<f64>,
}

/// An m x n sparse matrix in CSC (Compressed Sparse Column).
///
/// The mirror of CSR. col_ptr has n_size + 1 entries: column j occupies
/// positions col_ptr[j] up to col_ptr[j + 1], row_idx holds the row of each
/// stored entry.
///
/// The same matrix in CSR and CSC has the same nonzeros in a different
/// order, so val differs between the two: CSR stores it row by row, CSC
/// column by column.
pub struct CscMatrix {
    pub m_size: usize,
    pub n_size: usize,
    pub col_ptr: Vec<usize>,
    pub row_idx: Vec<i32>,
    pub val: Vec<f64>,
}

/// Implementation A: CSR, a gather.
///
/// Each y[i] is the dot product of row i with x, accumulated in a local and
/// written once. Structurally identical to the dense row version, with one
/// change: the row no longer begins at i * n_size but at row_ptr[i], and the
/// entries it holds are scattered across x rather than consecutive, so the
/// column of each has to be read from col_idx.
///
/// That indirection, x.val[a.col_idx[rp] as usize], is the whole difference
/// between a dense and a sparse kernel. The dense version reads x
/// consecutively; this one jumps.
///
/// rp is a position, so the range loop yields usize and indexes val and
/// col_idx directly. col_idx[rp] is an i32 index, and Rust has no implicit
/// numeric conversion, so turning it into a subscript is written out with
/// `as usize`. C++ writes the same line with no cast at all, since its usual
/// arithmetic conversions handle it silently.
pub fn csr_sparse_matvec(a: &CsrMatrix, x: &Vector) -> Vector {
    let mut y = Vector::new(a.m_size);
    for i in 0..a.m_size {
        let mut sum = 0.0;
        for rp in a.row_ptr[i]..a.row_ptr[i + 1] {
            sum += a.val[rp] * x.val[a.col_idx[rp] as usize];
        }
        y.val[i] = sum;
    }
    y
}

/// Implementation B: CSC, a scatter.
///
/// Column j, scaled by x[j], is added into y. y starts at zero and
/// accumulates across columns, so no entry is final until the last column is
/// processed.
///
/// Here the indirection is on the write rather than the read: the column's
/// entries land in scattered rows, so y.val[a.row_idx[cp] as usize] is a
/// jump while x.val[j] is read once per column. Exactly the mirror of A, and
/// the reason a scatter is harder to parallelize: two columns can write the
/// same y entry.
pub fn csc_sparse_matvec(a: &CscMatrix, x: &Vector) -> Vector {
    let mut y = Vector::new(a.m_size);
    for j in 0..a.n_size {
        let xj = x.val[j];
        for cp in a.col_ptr[j]..a.col_ptr[j + 1] {
            y.val[a.row_idx[cp] as usize] += a.val[cp] * xj;
        }
    }
    y
}
