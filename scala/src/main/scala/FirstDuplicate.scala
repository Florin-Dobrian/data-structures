import scala.collection.mutable

/**
 * Implementation A: Using mutable.HashSet.
 *
 * Iterate through the stream and add each value to the hash set.
 * If add returns false (the value was already present), we've found
 * the first duplicate. O(n) expected time, O(n) space.
 *
 * mutable.HashSet.add returns true if the element was added (new),
 * false if it already existed — no separate lookup needed.
 */
object HashSetFirstDuplicate {
  def solve(data: Array[Int]): Int = {
    val seen = mutable.HashSet[Int]()
    var i = 0
    while (i < data.length) {
      if (!seen.add(data(i))) return data(i)
      i += 1
    }
    -1
  }
}

/**
 * Implementation B: Using mutable.TreeSet (sorted set / red-black tree).
 *
 * Same algorithm as A, but the underlying container maintains sorted
 * order. Insert and lookup are O(log n) instead of O(1) amortized,
 * making overall time O(n log n). The tradeoff: iteration yields
 * elements in sorted order, and worst-case performance is more
 * predictable (no hash collisions).
 *
 * mutable.TreeSet.add has the same boolean-return interface as HashSet,
 * so the code is nearly identical.
 */
object TreeSetFirstDuplicate {
  def solve(data: Array[Int]): Int = {
    val seen = mutable.TreeSet[Int]()
    var i = 0
    while (i < data.length) {
      if (!seen.add(data(i))) return data(i)
      i += 1
    }
    -1
  }
}
