from datastructures.dense_matvec import ColDenseMatrix


def _copy_of(a: ColDenseMatrix) -> ColDenseMatrix:
    """A fresh matrix with a copy of a's buffer, so A survives factoring."""
    return ColDenseMatrix(a.m_size, a.n_size, list(a.val))


def left_dense_lu_factor(a: ColDenseMatrix) -> ColDenseMatrix:
    """Implementation A: left-looking, a gather.

    Dense LU without pivoting, A = L U. L is unit lower triangular
    (L[i][i] = 1, never stored) and U is upper triangular. Both live in one
    buffer: the factor is a copy of A overwritten in place, leaving U in the
    upper triangle and L's multipliers in the strict lower triangle. That is
    why the return type is ColDenseMatrix rather than a type of its own.

    Column-oriented storage, so column j begins at j * m_size and the entry
    at (i, j) is val[j * m_size + i].

    Column k waits until it is reached, then pulls in every earlier column's
    contribution in one pass, and only then normalizes its L part by the
    pivot. Nothing to the right of column k has been touched when the pass
    begins, and column k is finished when it ends.

    A must be square. No pivoting is done, which requires every leading
    principal minor to be nonzero; the demo matrix is built from integer L
    and U so its pivots are exactly the diagonal of U. A production
    factorization would pivot, and would factor in place rather than copying.
    """
    lu = _copy_of(a)
    n = lu.m_size

    for k in range(n):
        kp = k * n                                  # column k begins here
        for j in range(k):                          # earlier column j
            jp = j * n
            ujk = lu.val[kp + j]                    # U[j][k], already final
            for i in range(j + 1, n):
                lu.val[kp + i] -= lu.val[jp + i] * ujk
        pivot = lu.val[kp + k]                      # U[k][k]
        for i in range(k + 1, n):                   # normalize the L part
            lu.val[kp + i] /= pivot
    return lu


def right_dense_lu_factor(a: ColDenseMatrix) -> ColDenseMatrix:
    """Implementation B: right-looking, a scatter.

    The moment column k's multipliers are known, they push a rank-1 update
    into the entire trailing submatrix. Column k is finished early and
    everything to its right is partially updated, the mirror of the
    left-looking picture.

    That update is the outer product of column k's multipliers with row k of
    U, which is where a real implementation calls a dense GEMM.

    Same L and U as A, computed on a different schedule.
    """
    lu = _copy_of(a)
    n = lu.m_size

    for k in range(n):
        kp = k * n
        pivot = lu.val[kp + k]
        for i in range(k + 1, n):                   # multipliers first
            lu.val[kp + i] /= pivot
        for j in range(k + 1, n):                   # rank-1 update
            jp = j * n
            ukj = lu.val[jp + k]                    # U[k][j]
            for i in range(k + 1, n):
                lu.val[jp + i] -= lu.val[kp + i] * ukj
    return lu
