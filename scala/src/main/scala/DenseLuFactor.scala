/**
 * Dense LU factorization without pivoting, A = L U.
 *
 * L is unit lower triangular (L(i)(i) = 1, never stored) and U is upper
 * triangular. Both live in one buffer: the factor is a copy of A overwritten
 * in place, leaving U in the upper triangle and L's multipliers in the strict
 * lower triangle. That is why the return type is ColDenseMatrix rather than a
 * type of its own, and why the two triangles need no separate storage.
 *
 * Column-oriented storage throughout, so column j begins at j * mSize and the
 * entry at (i, j) is values(j * mSize + i).
 *
 * A must be square. No pivoting is done, which requires every leading
 * principal minor to be nonzero; the demo matrix is built from integer L and
 * U so its pivots are exactly the diagonal of U. A production factorization
 * would pivot, and would factor in place rather than copying.
 */

/** A fresh matrix with a copy of a's buffer, so A survives factoring. */
private def copyOf(a: ColDenseMatrix): ColDenseMatrix =
  ColDenseMatrix(a.mSize, a.nSize, a.values.clone())

/**
 * Implementation A: left-looking, a gather.
 *
 * Column k waits until it is reached, then pulls in every earlier column's
 * contribution in one pass, and only then normalizes its L part by the pivot.
 * Nothing to the right of column k has been touched when the pass begins, and
 * column k is finished when it ends.
 */
def leftDenseLuFactor(a: ColDenseMatrix): ColDenseMatrix = {
  val lu = copyOf(a)
  val n = lu.mSize

  var k = 0
  while (k < n) {
    val kp = k * n                                  // column k begins here
    var j = 0
    while (j < k) {                                 // earlier column j
      val jp = j * n
      val ujk = lu.values(kp + j)                   // U(j)(k), already final
      var i = j + 1
      while (i < n) {
        lu.values(kp + i) -= lu.values(jp + i) * ujk
        i += 1
      }
      j += 1
    }
    val pivot = lu.values(kp + k)                   // U(k)(k)
    var i = k + 1
    while (i < n) {                                 // normalize the L part
      lu.values(kp + i) /= pivot
      i += 1
    }
    k += 1
  }
  lu
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
 *
 * Same L and U as A, computed on a different schedule.
 */
def rightDenseLuFactor(a: ColDenseMatrix): ColDenseMatrix = {
  val lu = copyOf(a)
  val n = lu.mSize

  var j = 0
  while (j < n) {
    val jp = j * n
    val pivot = lu.values(jp + j)                   // U(j)(j)
    var i = j + 1
    while (i < n) {                                 // multipliers first
      lu.values(jp + i) /= pivot
      i += 1
    }
    var k = j + 1
    while (k < n) {                                 // later column k
      val kp = k * n
      val ujk = lu.values(kp + j)                   // U(j)(k)
      var r = j + 1
      while (r < n) {
        lu.values(kp + r) -= lu.values(jp + r) * ujk
        r += 1
      }
      k += 1
    }
    j += 1
  }
  lu
}
