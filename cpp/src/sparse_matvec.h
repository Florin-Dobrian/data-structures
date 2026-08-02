#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "dense_matvec.h"  // Vector

/**
 * An m x n sparse matrix in CSR (Compressed Sparse Row).
 *
 * Three arrays. rowPtr has mSize + 1 entries: row i occupies positions
 * rowPtr[i] up to rowPtr[i + 1] in the other two. colIdx holds the column
 * of each stored entry, val holds its value. Entries within a row are in
 * increasing column order here, though nothing in the algorithm requires it.
 *
 * rowPtr holds positions, so it is std::size_t. colIdx holds indices, so it
 * is std::int32_t: an index array is as long as the matrix has nonzeros, and
 * halving its width is the whole reason for the cap.
 */
struct CsrMatrix {
    std::size_t mSize;
    std::size_t nSize;
    std::vector<std::size_t> rowPtr;
    std::vector<std::int32_t> colIdx;
    std::vector<double> val;

    CsrMatrix(std::size_t mSize, std::size_t nSize,
              std::vector<std::size_t> rowPtr,
              std::vector<std::int32_t> colIdx,
              std::vector<double> val)
        : mSize(mSize), nSize(nSize), rowPtr(std::move(rowPtr)),
          colIdx(std::move(colIdx)), val(std::move(val)) {}
};

/**
 * An m x n sparse matrix in CSC (Compressed Sparse Column).
 *
 * The mirror of CSR. colPtr has nSize + 1 entries: column j occupies
 * positions colPtr[j] up to colPtr[j + 1], rowIdx holds the row of each
 * stored entry.
 *
 * The same matrix in CSR and CSC has the same nonzeros in a different order,
 * so val differs between the two: CSR stores it row by row, CSC column by
 * column.
 */
struct CscMatrix {
    std::size_t mSize;
    std::size_t nSize;
    std::vector<std::size_t> colPtr;
    std::vector<std::int32_t> rowIdx;
    std::vector<double> val;

    CscMatrix(std::size_t mSize, std::size_t nSize,
              std::vector<std::size_t> colPtr,
              std::vector<std::int32_t> rowIdx,
              std::vector<double> val)
        : mSize(mSize), nSize(nSize), colPtr(std::move(colPtr)),
          rowIdx(std::move(rowIdx)), val(std::move(val)) {}
};

/**
 * Implementation A: CSR, a gather.
 *
 * Each y[i] is the dot product of row i with x, accumulated in a local and
 * written once. Structurally identical to the dense row version, with one
 * change: the row no longer begins at i * nSize but at rowPtr[i], and the
 * entries it holds are scattered across x rather than consecutive, so the
 * column of each has to be read from colIdx.
 *
 * That indirection, x.val[a.colIdx[rp]], is the whole difference between a
 * dense and a sparse kernel. The dense version reads x consecutively; this
 * one jumps.
 *
 * rp is a position into rowPtr's companion arrays, so it is std::size_t.
 * colIdx[rp] is an index, std::int32_t, and subscripting with it needs no
 * cast: the usual arithmetic conversions handle it, and it is silent under
 * -Wall -Wextra.
 */
inline Vector csr_sparse_matvec(const CsrMatrix& a, const Vector& x) {
    Vector y(a.mSize);
    for (std::size_t i = 0; i < a.mSize; ++i) {
        double sum = 0.0;
        for (std::size_t rp = a.rowPtr[i]; rp < a.rowPtr[i + 1]; ++rp) {
            sum += a.val[rp] * x.val[a.colIdx[rp]];
        }
        y.val[i] = sum;
    }
    return y;
}

/**
 * Implementation B: CSC, a scatter.
 *
 * Column j, scaled by x[j], is added into y. y starts at zero and accumulates
 * across columns, so no entry is final until the last column is processed.
 *
 * Here the indirection is on the write rather than the read: the column's
 * entries land in scattered rows, so y.val[a.rowIdx[cp]] is a jump while
 * x.val[j] is read once per column. Exactly the mirror of A, and the reason
 * a scatter is harder to parallelize: two columns can write the same y entry.
 */
inline Vector csc_sparse_matvec(const CscMatrix& a, const Vector& x) {
    Vector y(a.mSize);
    for (std::size_t j = 0; j < a.nSize; ++j) {
        const double xj = x.val[j];
        for (std::size_t cp = a.colPtr[j]; cp < a.colPtr[j + 1]; ++cp) {
            y.val[a.rowIdx[cp]] += a.val[cp] * xj;
        }
    }
    return y;
}
