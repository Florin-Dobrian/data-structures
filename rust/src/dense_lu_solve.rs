use crate::dense_matvec::{ColDenseMatrix, RowDenseMatrix, Vector};

/// Implementation A: row-oriented, a gather.
///
/// Triangular solves against a combined LU factor, A x = b. With A = L U,
/// solving is two passes: forward L y = b, then back U x = y. The factor is
/// the one problem 11 produces, U in the upper triangle and L's multipliers
/// in the strict lower triangle, with L's unit diagonal implied. The forward
/// pass therefore never divides, and the back pass divides by the diagonal,
/// which belongs to U.
///
/// Each unknown is finished the moment it is reached, by subtracting the dot
/// product of the unknowns already solved. Nothing is written more than once.
/// Row i begins at rp = i * n_size, so both inner loops scan a row with
/// stride 1.
///
/// The back pass runs i downward, spelled `(0..n).rev()`. A range is an
/// iterator here, so reversing it is a method call rather than a different
/// loop: Python says range(n - 1, -1, -1), Scala decrements a var, and C++
/// needs the post-decrement idiom because its counter is unsigned.
///
/// Problem 11 produces only the column-major factor; the row-major one is
/// taken as given here, since the subject is the solve rather than the
/// factorization.
pub fn row_dense_lu_solve(lu: &RowDenseMatrix, b: &Vector) -> Vector {
    let n = lu.m_size;

    let mut y = Vector::new(n); // forward: L y = b
    for i in 0..n {
        let rp = i * n;
        let mut sum = b.val[i];
        for j in 0..i {
            sum -= lu.val[rp + j] * y.val[j]; // L[i][j], unit diagonal
        }
        y.val[i] = sum;
    }

    let mut x = Vector::new(n); // back: U x = y
    for i in (0..n).rev() {
        let rp = i * n;
        let mut sum = y.val[i];
        for j in (i + 1)..n {
            sum -= lu.val[rp + j] * x.val[j]; // U[i][j]
        }
        x.val[i] = sum / lu.val[rp + i]; // divide by the pivot
    }
    x
}

/// Implementation B: column-oriented, a scatter.
///
/// Each unknown, once solved, is pushed into every equation that still needs
/// it. The target starts as a copy of the right-hand side and is written many
/// times, so no entry is final until its own column is reached. Column j
/// begins at cp = j * m_size, so both inner loops scan a column with stride 1.
///
/// Note where the division sits. The gather divides x[i] after its dot
/// product is complete; the scatter divides x[j] before scattering it,
/// because the value has to be final before it can be pushed. Same
/// arithmetic, and the one place the mirror image is not quite symmetric.
pub fn col_dense_lu_solve(lu: &ColDenseMatrix, b: &Vector) -> Vector {
    let n = lu.m_size;

    let mut y = Vector::from_val(n, b.val.clone()); // forward: L y = b
    for j in 0..n {
        let cp = j * n;
        let yj = y.val[j]; // final, L is unit
        for i in (j + 1)..n {
            y.val[i] -= lu.val[cp + i] * yj; // L[i][j]
        }
    }

    let mut x = Vector::from_val(n, y.val.clone()); // back: U x = y
    for j in (0..n).rev() {
        let cp = j * n;
        x.val[j] /= lu.val[cp + j]; // final only after this
        let xj = x.val[j];
        for i in 0..j {
            x.val[i] -= lu.val[cp + i] * xj; // U[i][j]
        }
    }
    x
}
