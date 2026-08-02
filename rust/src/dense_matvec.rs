/// A dense vector: one flat buffer of values.
///
/// Shared by both implementations below, since a vector has no
/// orientation to choose.
pub struct Vector {
    pub size: usize,
    pub val: Vec<f64>,
}

impl Vector {
    /// Zero-filled, the shape both matvec functions need for y.
    pub fn new(size: usize) -> Self {
        Vector { size, val: vec![0.0; size] }
    }

    pub fn from_val(size: usize, val: Vec<f64>) -> Self {
        Vector { size, val }
    }
}

/// An m x n dense matrix stored row by row.
///
/// Row i occupies the contiguous run val[i * n_size .. i * n_size + n_size],
/// so walking a row is a stride-1 scan and walking a column is a
/// stride-n_size jump.
pub struct RowDenseMatrix {
    pub m_size: usize,
    pub n_size: usize,
    pub val: Vec<f64>,
}

/// An m x n dense matrix stored column by column.
///
/// Column j occupies the contiguous run val[j * m_size .. j * m_size + m_size].
/// The same matrix as above, transposed in memory only: the entry at (i, j)
/// is val[j * m_size + i] here and val[i * n_size + j] there.
pub struct ColDenseMatrix {
    pub m_size: usize,
    pub n_size: usize,
    pub val: Vec<f64>,
}

/// Implementation A: row-oriented, a gather.
///
/// Each y[i] is the dot product of row i with x, accumulated in a local
/// and written once. The row is contiguous, so the inner loop is a
/// stride-1 scan of both a.val and x.val.
///
/// rp is the position where row i begins. In this dense layout it is
/// plain arithmetic, i * n_size; in the sparse CSR version it becomes a
/// lookup in row_ptr, and the inner loop keeps its shape.
///
/// Nothing here is a stored index: the only buffer is val, holding f64,
/// and i, j and rp are all loop-local. So everything is usize and nothing
/// is cast. Problem 10 is where the index/position split appears, since
/// CSR and CSC store col_idx and row_idx as i32 arrays.
pub fn row_dense_matvec(a: &RowDenseMatrix, x: &Vector) -> Vector {
    let mut y = Vector::new(a.m_size);
    for i in 0..a.m_size {
        let rp = i * a.n_size;
        let mut sum = 0.0;
        for j in 0..a.n_size {
            sum += a.val[rp + j] * x.val[j];
        }
        y.val[i] = sum;
    }
    y
}

/// Implementation B: column-oriented, a scatter.
///
/// Column j, scaled by x[j], is added into the whole of y. y starts at
/// zero and accumulates across columns, so no entry of y is final until
/// the last column has been processed. The column is contiguous, so the
/// inner loop is a stride-1 scan of a.val and of y.val.
///
/// cp is the position where column j begins, j * m_size, and becomes a
/// lookup in col_ptr in the sparse CSC version.
///
/// Same arithmetic as A, transposed schedule: A reads all of x for each
/// output and writes each y[i] once; B reads each x[j] once and touches
/// all of y.
pub fn col_dense_matvec(a: &ColDenseMatrix, x: &Vector) -> Vector {
    let mut y = Vector::new(a.m_size);
    for j in 0..a.n_size {
        let cp = j * a.m_size;
        let xj = x.val[j];
        for i in 0..a.m_size {
            y.val[i] += a.val[cp + i] * xj;
        }
    }
    y
}
