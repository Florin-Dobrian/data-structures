#pragma once

#include <cstddef>
#include <vector>

/**
 * A dense vector: one flat buffer of values.
 *
 * Shared by both implementations below, since a vector has no orientation
 * to choose.
 */
struct Vector {
    std::size_t size;
    std::vector<double> val;

    explicit Vector(std::size_t size) : size(size), val(size, 0.0) {}

    Vector(std::size_t size, std::vector<double> val)
        : size(size), val(std::move(val)) {}
};

/**
 * An m x n dense matrix stored row by row.
 *
 * Row i occupies the contiguous run val[i * nSize .. i * nSize + nSize - 1],
 * so walking a row is a stride-1 scan and walking a column is a stride-nSize
 * jump.
 */
struct RowDenseMatrix {
    std::size_t mSize;
    std::size_t nSize;
    std::vector<double> val;

    RowDenseMatrix(std::size_t mSize, std::size_t nSize, std::vector<double> val)
        : mSize(mSize), nSize(nSize), val(std::move(val)) {}
};

/**
 * An m x n dense matrix stored column by column.
 *
 * Column j occupies the contiguous run val[j * mSize .. j * mSize + mSize - 1].
 * The same matrix as above, transposed in memory only: the entry at (i, j) is
 * val[j * mSize + i] here and val[i * nSize + j] there.
 */
struct ColDenseMatrix {
    std::size_t mSize;
    std::size_t nSize;
    std::vector<double> val;

    ColDenseMatrix(std::size_t mSize, std::size_t nSize, std::vector<double> val)
        : mSize(mSize), nSize(nSize), val(std::move(val)) {}
};

/**
 * Implementation A: row-oriented, a gather.
 *
 * Each y[i] is the dot product of row i with x, accumulated in a local and
 * written once. The row is contiguous, so the inner loop is a stride-1 scan
 * of both a.val and x.val.
 *
 * rp is the position where row i begins. In this dense layout it is plain
 * arithmetic, i * nSize; in the sparse CSR version it becomes a lookup in
 * rowPtr, and the inner loop keeps its shape.
 *
 * Nothing here is a stored index: the only buffer is val, holding doubles,
 * and i, j and rp are all loop-local. So everything is std::size_t and
 * nothing is cast. Problem 10 is where the index/position split appears,
 * since CSR and CSC store colIdx and rowIdx as std::int32_t arrays.
 */
inline Vector row_dense_matvec(const RowDenseMatrix& a, const Vector& x) {
    Vector y(a.mSize);
    for (std::size_t i = 0; i < a.mSize; ++i) {
        const std::size_t rp = i * a.nSize;
        double sum = 0.0;
        for (std::size_t j = 0; j < a.nSize; ++j) {
            sum += a.val[rp + j] * x.val[j];
        }
        y.val[i] = sum;
    }
    return y;
}

/**
 * Implementation B: column-oriented, a scatter.
 *
 * Column j, scaled by x[j], is added into the whole of y. y starts at zero
 * and accumulates across columns, so no entry of y is final until the last
 * column has been processed. The column is contiguous, so the inner loop is
 * a stride-1 scan of a.val and of y.val.
 *
 * cp is the position where column j begins, j * mSize, and becomes a lookup
 * in colPtr in the sparse CSC version.
 *
 * Same arithmetic as A, transposed schedule: A reads all of x for each output
 * and writes each y[i] once; B reads each x[j] once and touches all of y.
 */
inline Vector col_dense_matvec(const ColDenseMatrix& a, const Vector& x) {
    Vector y(a.mSize);
    for (std::size_t j = 0; j < a.nSize; ++j) {
        const std::size_t cp = j * a.mSize;
        const double xj = x.val[j];
        for (std::size_t i = 0; i < a.mSize; ++i) {
            y.val[i] += a.val[cp + i] * xj;
        }
    }
    return y;
}
