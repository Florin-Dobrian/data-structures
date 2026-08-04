use crate::sparse_matvec::{CscMatrix, CsrMatrix};

/// Implementation A: CSR to CSC.
///
/// Same matrix, same nonzeros, regrouped. CSR holds them row by row, CSC
/// column by column, so every entry moves and the value array comes out in a
/// different order. This is the operation problems 10 through 12 assume when
/// they hand-write both formats in their demos.
///
/// A counting sort keyed on the target group:
///
///   1. Count how many entries fall into each target group.
///   2. Prefix-sum the counts into the output pointer array.
///   3. Walk the source in order, placing each entry at its group's cursor
///      and advancing that cursor.
///
/// Two passes over the entries and one over the pointer array, so O(nnz + n),
/// linear, with no comparison performed anywhere. Nothing is sorted per group.
///
/// The output indices come out in increasing order as a side effect. Step 3
/// visits the source rows in increasing order, so entries landing in a given
/// column arrive with their row already ascending. That is the counting
/// sort's stability doing the work, and it is why running the conversion
/// twice canonicalizes a matrix whose indices arrived unsorted.
///
/// cursor is scratch, initialized from the output pointer array and consumed
/// as it advances; col_ptr is left intact.
pub fn csr_to_csc_sparse_convert(a: &CsrMatrix) -> CscMatrix {
    let nnz = a.val.len();

    let mut col_ptr = vec![0usize; a.n_size + 1]; // 1. count
    for p in 0..nnz {
        col_ptr[a.col_idx[p] as usize + 1] += 1;
    }
    for j in 0..a.n_size {
        // 2. prefix sum
        col_ptr[j + 1] += col_ptr[j];
    }

    let mut cursor = col_ptr[..a.n_size].to_vec();
    let mut row_idx = vec![0i32; nnz];
    let mut val = vec![0.0; nnz];

    for i in 0..a.m_size {
        // 3. scatter
        for rp in a.row_ptr[i]..a.row_ptr[i + 1] {
            let c = a.col_idx[rp] as usize;
            let cp = cursor[c];
            cursor[c] += 1;
            row_idx[cp] = i as i32;
            val[cp] = a.val[rp];
        }
    }

    CscMatrix {
        m_size: a.m_size,
        n_size: a.n_size,
        col_ptr,
        row_idx,
        val,
    }
}

/// Implementation B: CSC to CSR.
///
/// The mirror of A. Counts per row, so cursor has m_size entries, and the
/// source walk is over columns, which leaves each output row's col_idx
/// ascending.
pub fn csc_to_csr_sparse_convert(a: &CscMatrix) -> CsrMatrix {
    let nnz = a.val.len();

    let mut row_ptr = vec![0usize; a.m_size + 1]; // 1. count
    for p in 0..nnz {
        row_ptr[a.row_idx[p] as usize + 1] += 1;
    }
    for i in 0..a.m_size {
        // 2. prefix sum
        row_ptr[i + 1] += row_ptr[i];
    }

    let mut cursor = row_ptr[..a.m_size].to_vec();
    let mut col_idx = vec![0i32; nnz];
    let mut val = vec![0.0; nnz];

    for j in 0..a.n_size {
        // 3. scatter
        for cp in a.col_ptr[j]..a.col_ptr[j + 1] {
            let r = a.row_idx[cp] as usize;
            let rp = cursor[r];
            cursor[r] += 1;
            col_idx[rp] = j as i32;
            val[rp] = a.val[cp];
        }
    }

    CsrMatrix {
        m_size: a.m_size,
        n_size: a.n_size,
        row_ptr,
        col_idx,
        val,
    }
}
