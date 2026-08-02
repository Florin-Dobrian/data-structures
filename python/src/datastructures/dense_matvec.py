class Vector:
    """A dense vector: one flat list of values.

    Shared by both implementations below, since a vector has no
    orientation to choose.
    """

    def __init__(self, size: int, val: list[float] | None = None):
        self.size = size
        self.val = val if val is not None else [0.0] * size


class RowDenseMatrix:
    """An m x n dense matrix stored row by row.

    Row i occupies the contiguous run val[i * n_size : i * n_size + n_size],
    so walking a row is a stride-1 scan and walking a column is a
    stride-n_size jump.
    """

    def __init__(self, m_size: int, n_size: int, val: list[float]):
        self.m_size = m_size
        self.n_size = n_size
        self.val = val


class ColDenseMatrix:
    """An m x n dense matrix stored column by column.

    Column j occupies the contiguous run val[j * m_size : j * m_size + m_size].
    The same matrix as above, transposed in memory only: the entry at
    (i, j) is val[j * m_size + i] here and val[i * n_size + j] there.
    """

    def __init__(self, m_size: int, n_size: int, val: list[float]):
        self.m_size = m_size
        self.n_size = n_size
        self.val = val


def row_dense_matvec(a: RowDenseMatrix, x: Vector) -> Vector:
    """Implementation A: row-oriented, a gather.

    Each y[i] is the dot product of row i with x, accumulated in a local
    and written once. The row is contiguous, so the inner loop walks
    consecutive entries of a.val alongside consecutive entries of x.val.

    rp is the position where row i begins. In this dense layout it is
    plain arithmetic, i * n_size; in the sparse CSR version it becomes a
    lookup in row_ptr, and the inner loop keeps its shape.

    Nothing here is a stored index: the only buffer is val, holding
    floats, and i, j and rp are all loop-local. Problem 10 is where the
    index/position split appears, since CSR and CSC store col_idx and
    row_idx as arrays, which C++ and Rust type as int32.
    """
    y = Vector(a.m_size)
    for i in range(a.m_size):
        rp = i * a.n_size
        total = 0.0
        for j in range(a.n_size):
            total += a.val[rp + j] * x.val[j]
        y.val[i] = total
    return y


def col_dense_matvec(a: ColDenseMatrix, x: Vector) -> Vector:
    """Implementation B: column-oriented, a scatter.

    Column j, scaled by x[j], is added into the whole of y. y starts at
    zero and accumulates across columns, so no entry of y is final until
    the last column has been processed.

    cp is the position where column j begins, j * m_size, and becomes a
    lookup in col_ptr in the sparse CSC version.

    Same arithmetic as A, transposed schedule: A reads all of x for each
    output and writes each y[i] once; B reads each x[j] once and touches
    all of y.
    """
    y = Vector(a.m_size)
    for j in range(a.n_size):
        cp = j * a.m_size
        xj = x.val[j]
        for i in range(a.m_size):
            y.val[i] += a.val[cp + i] * xj
    return y
