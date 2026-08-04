from datastructures.dense_matvec import RowDenseMatrix, ColDenseMatrix


def row_to_col_dense_convert(a: RowDenseMatrix) -> ColDenseMatrix:
    """Implementation A: row-major to column-major.

    Same matrix, same m and n, same entries. Only the order they sit in the
    buffer changes: row-major puts (i, j) at i * n_size + j, column-major
    puts it at j * m_size + i. Every element moves, so this is a real copy
    rather than a relabelling.

    Problems 9 through 12 all hand-write both layouts in their demos. These
    two functions are what would produce the second one from the first.

    Reads consecutively, writes with a jump. The source position rp + j walks
    a row with stride 1; the destination position j * m_size + i lands in a
    different column each step. There is no loop order that makes both sides
    contiguous, which is the point.
    """
    out = [0.0] * len(a.val)
    for i in range(a.m_size):
        rp = i * a.n_size                           # row i begins here
        for j in range(a.n_size):
            out[j * a.m_size + i] = a.val[rp + j]
    return ColDenseMatrix(a.m_size, a.n_size, out)


def col_to_row_dense_convert(a: ColDenseMatrix) -> RowDenseMatrix:
    """Implementation B: column-major to row-major.

    The mirror of A. Reads consecutively down a column, writes across rows
    with a jump. Swapping which side is contiguous is the only difference.
    """
    out = [0.0] * len(a.val)
    for j in range(a.n_size):
        cp = j * a.m_size                           # column j begins here
        for i in range(a.m_size):
            out[i * a.n_size + j] = a.val[cp + i]
    return RowDenseMatrix(a.m_size, a.n_size, out)
