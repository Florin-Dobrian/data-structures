#pragma once

#include <cstddef>
#include <vector>

#include "dense_matvec.h"  // Vector, RowDenseMatrix, ColDenseMatrix

/**
 * Triangular solves against a combined LU factor, A x = b.
 *
 * With A = L U, solving is two passes: forward L y = b, then back U x = y.
 * The factor is the one problem 11 produces, U in the upper triangle and L's
 * multipliers in the strict lower triangle, with L's unit diagonal implied.
 * The forward pass therefore never divides, and the back pass divides by the
 * diagonal, which belongs to U.
 *
 * The two implementations differ in loop order, not in what they compute, and
 * each takes the factor in the layout its order walks with stride 1. Problem
 * 11 produces only the column-major factor; the row-major one is taken as
 * given here, since the subject is the solve rather than the factorization.
 */

/**
 * Implementation A: row-oriented, a gather.
 *
 * Each unknown is finished the moment it is reached, by subtracting the dot
 * product of the unknowns already solved. Nothing is written more than once.
 * Row i begins at rp = i * nSize, so both inner loops scan a row with stride 1.
 *
 * The back pass runs i downward. Written with a std::size_t counter that
 * would never terminate against `i >= 0`, so it uses the post-decrement idiom
 * `for (std::size_t i = n; i-- > 0;)`, which enters with i = n-1 and exits
 * after i = 0.
 */
inline Vector row_dense_lu_solve(const RowDenseMatrix& lu, const Vector& b) {
    const std::size_t n = lu.mSize;

    Vector y(n);                                    // forward: L y = b
    for (std::size_t i = 0; i < n; ++i) {
        const std::size_t rp = i * n;
        double sum = b.val[i];
        for (std::size_t j = 0; j < i; ++j) {
            sum -= lu.val[rp + j] * y.val[j];       // L[i][j], unit diagonal
        }
        y.val[i] = sum;
    }

    Vector x(n);                                    // back: U x = y
    for (std::size_t i = n; i-- > 0;) {
        const std::size_t rp = i * n;
        double sum = y.val[i];
        for (std::size_t j = i + 1; j < n; ++j) {
            sum -= lu.val[rp + j] * x.val[j];       // U[i][j]
        }
        x.val[i] = sum / lu.val[rp + i];            // divide by the pivot
    }
    return x;
}

/**
 * Implementation B: column-oriented, a scatter.
 *
 * Each unknown, once solved, is pushed into every equation that still needs
 * it. The target starts as a copy of the right-hand side and is written many
 * times, so no entry is final until its own column is reached. Column j
 * begins at cp = j * mSize, so both inner loops scan a column with stride 1.
 *
 * Note where the division sits. The gather divides x[i] after its dot product
 * is complete; the scatter divides x[j] before scattering it, because the
 * value has to be final before it can be pushed. Same arithmetic, and the one
 * place the mirror image is not quite symmetric.
 */
inline Vector col_dense_lu_solve(const ColDenseMatrix& lu, const Vector& b) {
    const std::size_t n = lu.mSize;

    Vector y(n, b.val);                             // forward: L y = b
    for (std::size_t j = 0; j < n; ++j) {
        const std::size_t cp = j * n;
        const double yj = y.val[j];                 // final, L is unit
        for (std::size_t i = j + 1; i < n; ++i) {
            y.val[i] -= lu.val[cp + i] * yj;        // L[i][j]
        }
    }

    Vector x(n, y.val);                             // back: U x = y
    for (std::size_t j = n; j-- > 0;) {
        const std::size_t cp = j * n;
        x.val[j] /= lu.val[cp + j];                 // final only after this
        const double xj = x.val[j];
        for (std::size_t i = 0; i < j; ++i) {
            x.val[i] -= lu.val[cp + i] * xj;        // U[i][j]
        }
    }
    return x;
}
