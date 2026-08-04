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
 * as it advances; the pointer array itself is left intact.
 */

/**
 * Implementation A: CSR to CSC.
 *
 * Counts per column, so cursor has nSize entries. The source walk is over
 * rows, which is what leaves each output column's rowIdx ascending.
 */
def csrToCscSparseConvert(a: CsrMatrix): CscMatrix = {
  val nnz = a.values.length

  val colPtr = new Array[Int](a.nSize + 1)          // 1. count
  var p = 0
  while (p < nnz) {
    colPtr(a.colIdx(p) + 1) += 1
    p += 1
  }
  var j = 0
  while (j < a.nSize) {                             // 2. prefix sum
    colPtr(j + 1) += colPtr(j)
    j += 1
  }

  val cursor = colPtr.take(a.nSize)
  val rowIdx = new Array[Int](nnz)
  val values = new Array[Double](nnz)

  var i = 0
  while (i < a.mSize) {                             // 3. scatter
    var rp = a.rowPtr(i)
    while (rp < a.rowPtr(i + 1)) {
      val c = a.colIdx(rp)
      val cp = cursor(c)
      cursor(c) += 1
      rowIdx(cp) = i
      values(cp) = a.values(rp)
      rp += 1
    }
    i += 1
  }

  CscMatrix(a.mSize, a.nSize, colPtr, rowIdx, values)
}

/**
 * Implementation B: CSC to CSR.
 *
 * The mirror. Counts per row, so cursor has mSize entries, and the source
 * walk is over columns, which leaves each output row's colIdx ascending.
 */
def cscToCsrSparseConvert(a: CscMatrix): CsrMatrix = {
  val nnz = a.values.length

  val rowPtr = new Array[Int](a.mSize + 1)          // 1. count
  var p = 0
  while (p < nnz) {
    rowPtr(a.rowIdx(p) + 1) += 1
    p += 1
  }
  var i = 0
  while (i < a.mSize) {                             // 2. prefix sum
    rowPtr(i + 1) += rowPtr(i)
    i += 1
  }

  val cursor = rowPtr.take(a.mSize)
  val colIdx = new Array[Int](nnz)
  val values = new Array[Double](nnz)

  var j = 0
  while (j < a.nSize) {                             // 3. scatter
    var cp = a.colPtr(j)
    while (cp < a.colPtr(j + 1)) {
      val r = a.rowIdx(cp)
      val rp = cursor(r)
      cursor(r) += 1
      colIdx(rp) = j
      values(rp) = a.values(cp)
      cp += 1
    }
    j += 1
  }

  CsrMatrix(a.mSize, a.nSize, rowPtr, colIdx, values)
}
