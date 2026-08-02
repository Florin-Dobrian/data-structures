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
 * Row i begins at rp = i * nSize, so both inner loops scan a row with
 * stride 1.
 *
 * The back pass runs i downward, which a while loop expresses by starting at
 * n - 1 and decrementing. Python says range(n - 1, -1, -1) and Rust says
 * .rev(); C++ needs the post-decrement idiom because its counter is unsigned.
 * Four spellings of one loop.
 */
def rowDenseLuSolve(lu: RowDenseMatrix, b: Vector): Vector = {
  val n = lu.mSize

  val y = Vector(n)                                 // forward: L y = b
  var i = 0
  while (i < n) {
    val rp = i * n
    var sum = b.values(i)
    var j = 0
    while (j < i) {
      sum -= lu.values(rp + j) * y.values(j)        // L(i)(j), unit diagonal
      j += 1
    }
    y.values(i) = sum
    i += 1
  }

  val x = Vector(n)                                 // back: U x = y
  var r = n - 1
  while (r >= 0) {
    val rp = r * n
    var sum = y.values(r)
    var j = r + 1
    while (j < n) {
      sum -= lu.values(rp + j) * x.values(j)        // U(r)(j)
      j += 1
    }
    x.values(r) = sum / lu.values(rp + r)           // divide by the pivot
    r -= 1
  }
  x
}

/**
 * Implementation B: column-oriented, a scatter.
 *
 * Each unknown, once solved, is pushed into every equation that still needs
 * it. The target starts as a copy of the right-hand side and is written many
 * times, so no entry is final until its own column is reached. Column j
 * begins at cp = j * mSize, so both inner loops scan a column with stride 1.
 *
 * Note where the division sits. The gather divides x(i) after its dot product
 * is complete; the scatter divides x(j) before scattering it, because the
 * value has to be final before it can be pushed. Same arithmetic, and the one
 * place the mirror image is not quite symmetric.
 */
def colDenseLuSolve(lu: ColDenseMatrix, b: Vector): Vector = {
  val n = lu.mSize

  val y = Vector(n, b.values.clone())               // forward: L y = b
  var j = 0
  while (j < n) {
    val cp = j * n
    val yj = y.values(j)                            // final, L is unit
    var i = j + 1
    while (i < n) {
      y.values(i) -= lu.values(cp + i) * yj         // L(i)(j)
      i += 1
    }
    j += 1
  }

  val x = Vector(n, y.values.clone())               // back: U x = y
  var c = n - 1
  while (c >= 0) {
    val cp = c * n
    x.values(c) /= lu.values(cp + c)                // final only after this
    val xc = x.values(c)
    var i = 0
    while (i < c) {
      x.values(i) -= lu.values(cp + i) * xc         // U(i)(c)
      i += 1
    }
    c -= 1
  }
  x
}
