/**
 * An m x n sparse matrix in CSR (Compressed Sparse Row).
 *
 * Three arrays. rowPtr has mSize + 1 entries: row i occupies positions
 * rowPtr(i) up to rowPtr(i + 1) in the other two. colIdx holds the column of
 * each stored entry, values holds its value. Entries within a row are in
 * increasing column order here, though nothing in the algorithm requires it.
 *
 * rowPtr holds positions and colIdx holds indices. Scala has one Int for
 * both roles, so the split that C++ and Rust express as size_t against int32
 * survives here only in the naming.
 *
 * The value field is `values` rather than `val`, which is a reserved word in
 * Scala, matching DenseMatvec.
 */
case class CsrMatrix(
  mSize: Int,
  nSize: Int,
  rowPtr: Array[Int],
  colIdx: Array[Int],
  values: Array[Double]
)

/**
 * An m x n sparse matrix in CSC (Compressed Sparse Column).
 *
 * The mirror of CSR. colPtr has nSize + 1 entries: column j occupies
 * positions colPtr(j) up to colPtr(j + 1), rowIdx holds the row of each
 * stored entry.
 *
 * The same matrix in CSR and CSC has the same nonzeros in a different order,
 * so values differs between the two: CSR stores it row by row, CSC column by
 * column.
 */
case class CscMatrix(
  mSize: Int,
  nSize: Int,
  colPtr: Array[Int],
  rowIdx: Array[Int],
  values: Array[Double]
)

/**
 * Implementation A: CSR, a gather.
 *
 * Each y(i) is the dot product of row i with x, accumulated in a local and
 * written once. Structurally identical to the dense row version, with one
 * change: the row no longer begins at i * nSize but at rowPtr(i), and the
 * entries it holds are scattered across x rather than consecutive, so the
 * column of each has to be read from colIdx.
 *
 * That indirection, x.values(a.colIdx(rp)), is the whole difference between
 * a dense and a sparse kernel. The dense version reads x consecutively; this
 * one jumps.
 */
def csrSparseMatvec(a: CsrMatrix, x: Vector): Vector = {
  val y = Vector(a.mSize)
  var i = 0
  while (i < a.mSize) {
    var sum = 0.0
    var rp = a.rowPtr(i)
    while (rp < a.rowPtr(i + 1)) {
      sum += a.values(rp) * x.values(a.colIdx(rp))
      rp += 1
    }
    y.values(i) = sum
    i += 1
  }
  y
}

/**
 * Implementation B: CSC, a scatter.
 *
 * Column j, scaled by x(j), is added into y. y starts at zero and accumulates
 * across columns, so no entry is final until the last column is processed.
 *
 * Here the indirection is on the write rather than the read: the column's
 * entries land in scattered rows, so y.values(a.rowIdx(cp)) is a jump while
 * x.values(j) is read once per column. Exactly the mirror of A, and the
 * reason a scatter is harder to parallelize: two columns can write the same
 * y entry.
 */
def cscSparseMatvec(a: CscMatrix, x: Vector): Vector = {
  val y = Vector(a.mSize)
  var j = 0
  while (j < a.nSize) {
    val xj = x.values(j)
    var cp = a.colPtr(j)
    while (cp < a.colPtr(j + 1)) {
      y.values(a.rowIdx(cp)) += a.values(cp) * xj
      cp += 1
    }
    j += 1
  }
  y
}
