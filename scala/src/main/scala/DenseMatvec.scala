/**
 * A dense vector: one flat array of values.
 *
 * Shared by both implementations below, since a vector has no orientation
 * to choose.
 *
 * The field is `values` rather than `val`, which is a reserved word in
 * Scala. The other three languages all spell it `val`; this is the one
 * place the vocabulary has to bend to the host language.
 *
 * The class name shadows scala.Vector within this file, which is harmless
 * here since the collection is not used.
 */
case class Vector(size: Int, values: Array[Double])

object Vector {
  /** Zero-filled, the shape both matvec functions need for y. */
  def apply(size: Int): Vector = new Vector(size, Array.fill(size)(0.0))
}

/**
 * An m x n dense matrix stored row by row.
 *
 * Row i occupies the contiguous run values(i * nSize) to
 * values(i * nSize + nSize - 1), so walking a row is a stride-1 scan and
 * walking a column is a stride-nSize jump.
 */
case class RowDenseMatrix(mSize: Int, nSize: Int, values: Array[Double])

/**
 * An m x n dense matrix stored column by column.
 *
 * Column j occupies the contiguous run values(j * mSize) to
 * values(j * mSize + mSize - 1). The same matrix as above, transposed in
 * memory only: the entry at (i, j) is values(j * mSize + i) here and
 * values(i * nSize + j) there.
 */
case class ColDenseMatrix(mSize: Int, nSize: Int, values: Array[Double])

/**
 * Implementation A: row-oriented, a gather.
 *
 * Each y(i) is the dot product of row i with x, accumulated in a local and
 * written once. The row is contiguous, so the inner loop walks consecutive
 * entries of a.values alongside consecutive entries of x.values.
 *
 * rp is the position where row i begins. In this dense layout it is plain
 * arithmetic, i * nSize; in the sparse CSR version it becomes a lookup in
 * rowPtr, and the inner loop keeps its shape.
 *
 * Nothing here is a stored index: the only buffer is values, holding
 * Doubles, and i, j and rp are all loop-local. Problem 10 is where the
 * index/position split appears, since CSR and CSC store colIdx and rowIdx
 * as arrays, which C++ and Rust type as int32.
 *
 * While loops rather than for comprehensions, matching the explicit index
 * arithmetic of the other three languages.
 */
def rowDenseMatvec(a: RowDenseMatrix, x: Vector): Vector = {
  val y = Vector(a.mSize)
  var i = 0
  while (i < a.mSize) {
    val rp = i * a.nSize
    var sum = 0.0
    var j = 0
    while (j < a.nSize) {
      sum += a.values(rp + j) * x.values(j)
      j += 1
    }
    y.values(i) = sum
    i += 1
  }
  y
}

/**
 * Implementation B: column-oriented, a scatter.
 *
 * Column j, scaled by x(j), is added into the whole of y. y starts at zero
 * and accumulates across columns, so no entry of y is final until the last
 * column has been processed.
 *
 * cp is the position where column j begins, j * mSize, and becomes a lookup
 * in colPtr in the sparse CSC version.
 *
 * Same arithmetic as A, transposed schedule: A reads all of x for each
 * output and writes each y(i) once; B reads each x(j) once and touches all
 * of y.
 */
def colDenseMatvec(a: ColDenseMatrix, x: Vector): Vector = {
  val y = Vector(a.mSize)
  var j = 0
  while (j < a.nSize) {
    val cp = j * a.mSize
    val xj = x.values(j)
    var i = 0
    while (i < a.mSize) {
      y.values(i) += a.values(cp + i) * xj
      i += 1
    }
    j += 1
  }
  y
}
