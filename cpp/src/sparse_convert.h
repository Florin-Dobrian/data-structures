#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

#include "sparse_matvec.h"  // CsrMatrix, CscMatrix

/**
 * Convert a sparse matrix between CSR and CSC.
 *
 * Same matrix, same nonzeros, regrouped. CSR holds them row by row, CSC
 * column by column, so every entry moves and the value array comes out in a
 * different order. This is the operation problems 10 through 12 assume when
 * they hand-write both formats in their demos.
 *
 * Both directions are the same algorithm, a counting sort keyed on the target
 * group:
 *
 *   1. Count how many entries fall into each target group.
 *   2. Prefix-sum the counts into the output pointer array.
 *   3. Walk the source in order, placing each entry at its group's cursor and
 *      advancing that cursor.
 *
 * Two passes over the entries and one over the pointer array, so O(nnz + n),
 * linear, with no comparison performed anywhere. Nothing is sorted per group.
 *
 * The output indices come out in increasing order as a side effect. Step 3
 * visits the source groups in increasing order, so entries landing in a given
 * target group arrive with their other coordinate already ascending. That is
 * the counting sort's stability doing the work, and it is why running the
 * conversion twice canonicalizes a matrix whose indices arrived unsorted.
 *
 * cursor is scratch, initialized from the output pointer array and consumed
 * as it advances. Some implementations advance the pointer array itself and
 * rebuild it afterwards by shifting, saving n + 1 words at the cost of
 * clarity. Here the pointer array is left intact.
 */

/**
 * Implementation A: CSR to CSC.
 *
 * Counts per column, so cursor has nSize entries. The source walk is over
 * rows, which is what leaves each output column's rowIdx ascending.
 */
inline CscMatrix csr_to_csc_sparse_convert(const CsrMatrix& a) {
    const std::size_t nnz = a.val.size();

    std::vector<std::size_t> colPtr(a.nSize + 1, 0);          // 1. count
    for (std::size_t p = 0; p < nnz; ++p) {
        colPtr[a.colIdx[p] + 1]++;
    }
    for (std::size_t j = 0; j < a.nSize; ++j) {               // 2. prefix sum
        colPtr[j + 1] += colPtr[j];
    }

    std::vector<std::size_t> cursor(colPtr.begin(), colPtr.end() - 1);
    std::vector<std::int32_t> rowIdx(nnz);
    std::vector<double> val(nnz);

    for (std::size_t i = 0; i < a.mSize; ++i) {               // 3. scatter
        for (std::size_t rp = a.rowPtr[i]; rp < a.rowPtr[i + 1]; ++rp) {
            const std::size_t cp = cursor[a.colIdx[rp]]++;
            rowIdx[cp] = static_cast<std::int32_t>(i);
            val[cp] = a.val[rp];
        }
    }
    return CscMatrix(a.mSize, a.nSize, std::move(colPtr),
                     std::move(rowIdx), std::move(val));
}

/**
 * Implementation B: CSC to CSR.
 *
 * The mirror. Counts per row, so cursor has mSize entries, and the source
 * walk is over columns, which leaves each output row's colIdx ascending.
 */
inline CsrMatrix csc_to_csr_sparse_convert(const CscMatrix& a) {
    const std::size_t nnz = a.val.size();

    std::vector<std::size_t> rowPtr(a.mSize + 1, 0);          // 1. count
    for (std::size_t p = 0; p < nnz; ++p) {
        rowPtr[a.rowIdx[p] + 1]++;
    }
    for (std::size_t i = 0; i < a.mSize; ++i) {               // 2. prefix sum
        rowPtr[i + 1] += rowPtr[i];
    }

    std::vector<std::size_t> cursor(rowPtr.begin(), rowPtr.end() - 1);
    std::vector<std::int32_t> colIdx(nnz);
    std::vector<double> val(nnz);

    for (std::size_t j = 0; j < a.nSize; ++j) {               // 3. scatter
        for (std::size_t cp = a.colPtr[j]; cp < a.colPtr[j + 1]; ++cp) {
            const std::size_t rp = cursor[a.rowIdx[cp]]++;
            colIdx[rp] = static_cast<std::int32_t>(j);
            val[rp] = a.val[cp];
        }
    }
    return CsrMatrix(a.mSize, a.nSize, std::move(rowPtr),
                     std::move(colIdx), std::move(val));
}
