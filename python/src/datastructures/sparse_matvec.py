from datastructures.dense_matvec import Vector


class CsrMatrix:
    """An m x n sparse matrix in CSR (Compressed Sparse Row).

    Three lists. row_ptr has m_size + 1 entries: row i occupies positions
    row_ptr[i] up to row_ptr[i + 1] in the other two. col_idx holds the
    column of each stored entry, val holds its value. Entries within a row
    are in increasing column order here, though nothing in the algorithm
    requires it.

    row_ptr holds positions and col_idx holds indices. Python has no
    integer types to distinguish the two, so the split that C++ and Rust
    express as size_t against int32 survives here only in the naming.
    """

    def __init__(self, m_size: int, n_size: int,
                 row_ptr: list[int], col_idx: list[int], val: list[float]):
        self.m_size = m_size
        self.n_size = n_size
        self.row_ptr = row_ptr
        self.col_idx = col_idx
        self.val = val


class CscMatrix:
    """An m x n sparse matrix in CSC (Compressed Sparse Column).

    The mirror of CSR. col_ptr has n_size + 1 entries: column j occupies
    positions col_ptr[j] up to col_ptr[j + 1], row_idx holds the row of
    each stored entry.

    The same matrix in CSR and CSC has the same nonzeros in a different
    order, so val differs between the two: CSR stores it row by row, CSC
    column by column.
    """

    def __init__(self, m_size: int, n_size: int,
                 col_ptr: list[int], row_idx: list[int], val: list[float]):
        self.m_size = m_size
        self.n_size = n_size
        self.col_ptr = col_ptr
        self.row_idx = row_idx
        self.val = val


def csr_sparse_matvec(a: CsrMatrix, x: Vector) -> Vector:
    """Implementation A: CSR, a gather.

    Each y[i] is the dot product of row i with x, accumulated in a local
    and written once. Structurally identical to the dense row version,
    with one change: the row no longer begins at i * n_size but at
    row_ptr[i], and the entries it holds are scattered across x rather
    than consecutive, so the column of each has to be read from col_idx.

    That indirection, x.val[a.col_idx[rp]], is the whole difference
    between a dense and a sparse kernel. The dense version reads x
    consecutively; this one jumps.
    """
    y = Vector(a.m_size)
    for i in range(a.m_size):
        total = 0.0
        for rp in range(a.row_ptr[i], a.row_ptr[i + 1]):
            total += a.val[rp] * x.val[a.col_idx[rp]]
        y.val[i] = total
    return y


def csc_sparse_matvec(a: CscMatrix, x: Vector) -> Vector:
    """Implementation B: CSC, a scatter.

    Column j, scaled by x[j], is added into y. y starts at zero and
    accumulates across columns, so no entry is final until the last
    column is processed.

    Here the indirection is on the write rather than the read: the
    column's entries land in scattered rows, so y.val[a.row_idx[cp]] is a
    jump while x.val[j] is read once per column. Exactly the mirror of A,
    and the reason a scatter is harder to parallelize: two columns can
    write the same y entry.
    """
    y = Vector(a.m_size)
    for j in range(a.n_size):
        xj = x.val[j]
        for cp in range(a.col_ptr[j], a.col_ptr[j + 1]):
            y.val[a.row_idx[cp]] += a.val[cp] * xj
    return y
