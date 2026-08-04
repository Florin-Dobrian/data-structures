/**
 * Convert a dense matrix between the two layouts.
 *
 * Same matrix, same m and n, same entries. Only the order they sit in the
 * buffer changes: row-major puts (i, j) at i * nSize + j, column-major puts
 * it at j * mSize + i. Every element moves, so this is a real copy rather
 * than a relabelling.
 *
 * Problems 9 through 12 all hand-write both layouts in their demos. These two
 * functions are what would produce the second one from the first.
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
def rowToColDenseConvert(a: RowDenseMatrix): ColDenseMatrix = {
  val out = new Array[Double](a.values.length)
  var i = 0
  while (i < a.mSize) {
    val rp = i * a.nSize                            // row i begins here
    var j = 0
    while (j < a.nSize) {
      out(j * a.mSize + i) = a.values(rp + j)
      j += 1
    }
    i += 1
  }
  ColDenseMatrix(a.mSize, a.nSize, out)
}

/**
 * Implementation B: column-major to row-major.
 *
 * The mirror. Reads consecutively down a column, writes across rows with a
 * jump. Swapping which side is contiguous is the only difference from A.
 */
def colToRowDenseConvert(a: ColDenseMatrix): RowDenseMatrix = {
  val out = new Array[Double](a.values.length)
  var j = 0
  while (j < a.nSize) {
    val cp = j * a.mSize                            // column j begins here
    var i = 0
    while (i < a.mSize) {
      out(i * a.nSize + j) = a.values(cp + i)
      i += 1
    }
    j += 1
  }
  RowDenseMatrix(a.mSize, a.nSize, out)
}
