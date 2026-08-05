#pragma once

#include <cstddef>
#include <vector>

#include "dense_matvec.h"  // ColDenseMatrix

/**
 * Dense LU factorization without pivoting, A = L U.
 *
 * L is unit lower triangular (L[i][i] = 1, never stored) and U is upper
 * triangular. Both live in one buffer: the factor is a copy of A overwritten
 * in place, leaving U in the upper triangle and L's multipliers in the strict
 * lower triangle. That is why the return type is ColDenseMatrix rather than a
 * type of its own, and why the two triangles need no separate storage.
 *
 * Column-oriented storage throughout, so column j begins at cp = j * mSize
 * and the entry at (i, j) is val[j * mSize + i].
 *
 * A must be square. No pivoting is done, which requires every leading
 * principal minor to be nonzero; the demo matrix is built from integer L and
 * U so its pivots are exactly the diagonal of U and nothing is near zero.
 * A production factorization would pivot, and would also factor in place
 * rather than copying.
 *
 * The two implementations compute the same L and U and differ only in when
 * the cross-column contributions are applied. This is the same gather and
 * scatter pair as the matvec, on a harder operation.
 */

/**
 * Implementation A: left-looking, a gather.
 *
 * Column k waits until it is reached, then pulls in every earlier column's
 * contribution in one pass, and only then normalizes its L part by the pivot.
 * Nothing to the right of column k has been touched when the pass begins, and
 * column k is finished when it ends.
 */
inline ColDenseMatrix left_dense_lu_factor(const ColDenseMatrix& a) {
    ColDenseMatrix lu = a;  // factor a copy, leaving A intact
    const std::size_t n = lu.mSize;

    for (std::size_t k = 0; k < n; ++k) {
        const std::size_t kp = k * n;                  // column k begins here
        for (std::size_t j = 0; j < k; ++j) {          // earlier column j
            const std::size_t jp = j * n;
            const double ujk = lu.val[kp + j];         // U[j][k], already final
            for (std::size_t i = j + 1; i < n; ++i) {
                lu.val[kp + i] -= lu.val[jp + i] * ujk;
            }
        }
        const double pivot = lu.val[kp + k];           // U[k][k]
        for (std::size_t i = k + 1; i < n; ++i) {      // normalize the L part
            lu.val[kp + i] /= pivot;
        }
    }
    return lu;
}

/**
 * Implementation B: right-looking, a scatter.
 *
 * The moment column j's multipliers are known, they push a rank-1 update into
 * the entire trailing submatrix. Column j is finished early and everything to
 * its right is partially updated, the mirror of the left-looking picture.
 *
 * That update is the outer product of column j's multipliers with row j of U,
 * which is where a real implementation calls a dense GEMM.
 *
 * j is always the source column and k the target, with j < k, so this is the
 * same set of (j, k) pairs the left-looking version visits, in the opposite
 * nesting order. The innermost statement is identical in both.
 */
inline ColDenseMatrix right_dense_lu_factor(const ColDenseMatrix& a) {
    ColDenseMatrix lu = a;
    const std::size_t n = lu.mSize;

    for (std::size_t j = 0; j < n; ++j) {
        const std::size_t jp = j * n;
        const double pivot = lu.val[jp + j];           // U[j][j]
        for (std::size_t i = j + 1; i < n; ++i) {      // multipliers first
            lu.val[jp + i] /= pivot;
        }
        for (std::size_t k = j + 1; k < n; ++k) {      // later column k
            const std::size_t kp = k * n;
            const double ujk = lu.val[kp + j];         // U[j][k]
            for (std::size_t i = j + 1; i < n; ++i) {
                lu.val[kp + i] -= lu.val[jp + i] * ujk;
            }
        }
    }
    return lu;
}
