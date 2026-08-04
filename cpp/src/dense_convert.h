#pragma once

#include <cstddef>
#include <vector>

#include "dense_matvec.h"  // RowDenseMatrix, ColDenseMatrix

/**
 * Convert a dense matrix between the two layouts.
 *
 * Same matrix, same m and n, same entries. Only the order they sit in the
 * buffer changes: row-major puts (i, j) at i * nSize + j, column-major puts
 * it at j * mSize + i. Every element moves, so this is a real copy rather
 * than a relabelling.
 *
 * Problems 9 through 12 all hand-write both layouts in their demos. These
 * two functions are what would produce the second one from the first.
 *
 * The two directions are mirror images: the same double loop with the two
 * index expressions swapped. Whichever way it runs, one side is walked with
 * stride 1 and the other with a stride of a whole row or column, so exactly
 * one of the read and the write is contiguous and the other jumps. There is
 * no loop order that makes both contiguous, which is the point.
 */

/**
 * Implementation A: row-major to column-major.
 *
 * Reads consecutively, writes with a jump. The source position rp + j walks a
 * row with stride 1; the destination position j * mSize + i lands in a
 * different column each step.
 */
inline ColDenseMatrix row_to_col_dense_convert(const RowDenseMatrix& a) {
    std::vector<double> out(a.val.size());
    for (std::size_t i = 0; i < a.mSize; ++i) {
        const std::size_t rp = i * a.nSize;          // row i begins here
        for (std::size_t j = 0; j < a.nSize; ++j) {
            out[j * a.mSize + i] = a.val[rp + j];
        }
    }
    return ColDenseMatrix(a.mSize, a.nSize, std::move(out));
}

/**
 * Implementation B: column-major to row-major.
 *
 * The mirror. Reads consecutively down a column, writes across rows with a
 * jump. Swapping which side is contiguous is the only difference from A.
 */
inline RowDenseMatrix col_to_row_dense_convert(const ColDenseMatrix& a) {
    std::vector<double> out(a.val.size());
    for (std::size_t j = 0; j < a.nSize; ++j) {
        const std::size_t cp = j * a.mSize;          // column j begins here
        for (std::size_t i = 0; i < a.mSize; ++i) {
            out[i * a.nSize + j] = a.val[cp + i];
        }
    }
    return RowDenseMatrix(a.mSize, a.nSize, std::move(out));
}
