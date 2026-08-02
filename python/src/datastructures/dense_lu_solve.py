from datastructures.dense_matvec import Vector, RowDenseMatrix, ColDenseMatrix


def row_dense_lu_solve(lu: RowDenseMatrix, b: Vector) -> Vector:
    """Implementation A: row-oriented, a gather.

    Triangular solves against a combined LU factor, A x = b. With A = L U,
    solving is two passes: forward L y = b, then back U x = y. The factor is
    the one problem 11 produces, U in the upper triangle and L's multipliers
    in the strict lower triangle, with L's unit diagonal implied. The forward
    pass therefore never divides, and the back pass divides by the diagonal,
    which belongs to U.

    Each unknown is finished the moment it is reached, by subtracting the dot
    product of the unknowns already solved. Nothing is written more than once.
    Row i begins at rp = i * n_size, so both inner loops scan a row with
    stride 1.

    The back pass runs i downward, which range() expresses directly as
    range(n - 1, -1, -1). C++ and Rust need an idiom here, because an unsigned
    counter cannot test against zero from above.

    Problem 11 produces only the column-major factor; the row-major one is
    taken as given here, since the subject is the solve rather than the
    factorization.
    """
    n = lu.m_size

    y = Vector(n)                                   # forward: L y = b
    for i in range(n):
        rp = i * n
        total = b.val[i]
        for j in range(i):
            total -= lu.val[rp + j] * y.val[j]      # L[i][j], unit diagonal
        y.val[i] = total

    x = Vector(n)                                   # back: U x = y
    for i in range(n - 1, -1, -1):
        rp = i * n
        total = y.val[i]
        for j in range(i + 1, n):
            total -= lu.val[rp + j] * x.val[j]      # U[i][j]
        x.val[i] = total / lu.val[rp + i]           # divide by the pivot
    return x


def col_dense_lu_solve(lu: ColDenseMatrix, b: Vector) -> Vector:
    """Implementation B: column-oriented, a scatter.

    Each unknown, once solved, is pushed into every equation that still needs
    it. The target starts as a copy of the right-hand side and is written
    many times, so no entry is final until its own column is reached. Column
    j begins at cp = j * m_size, so both inner loops scan a column with
    stride 1.

    Note where the division sits. The gather divides x[i] after its dot
    product is complete; the scatter divides x[j] before scattering it,
    because the value has to be final before it can be pushed. Same
    arithmetic, and the one place the mirror image is not quite symmetric.
    """
    n = lu.m_size

    y = Vector(n, list(b.val))                      # forward: L y = b
    for j in range(n):
        cp = j * n
        yj = y.val[j]                               # final, L is unit
        for i in range(j + 1, n):
            y.val[i] -= lu.val[cp + i] * yj         # L[i][j]

    x = Vector(n, list(y.val))                      # back: U x = y
    for j in range(n - 1, -1, -1):
        cp = j * n
        x.val[j] /= lu.val[cp + j]                  # final only after this
        xj = x.val[j]
        for i in range(j):
            x.val[i] -= lu.val[cp + i] * xj         # U[i][j]
    return x
