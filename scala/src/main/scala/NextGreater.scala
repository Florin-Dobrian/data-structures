import scala.collection.mutable

/**
 * Implementation A: Using mutable.Stack
 *
 * Scala's mutable.Stack provides push/pop/top. Iterate right to left,
 * maintaining a monotonically decreasing stack.
 */
object StackNextGreater {
  def solve(nums: Array[Int]): Array[Int] = {
    val result = Array.fill(nums.length)(-1)
    val stack = mutable.Stack[Int]()

    for (i <- nums.indices.reverse) {
      while (stack.nonEmpty && stack.top <= nums(i)) {
        stack.pop()
      }
      if (stack.nonEmpty) {
        result(i) = stack.top
      }
      stack.push(nums(i))
    }

    result
  }
}

/**
 * A minimal stack backed by a pre-allocated Array.
 */
class ArrayStack(capacity: Int) {
  private val data = new Array[Int](capacity)
  private var topIdx = -1

  def push(value: Int): Unit = { topIdx += 1; data(topIdx) = value }
  def pop(): Int = { val v = data(topIdx); topIdx -= 1; v }
  def top: Int = data(topIdx)
  def isEmpty: Boolean = topIdx == -1
}

/**
 * Implementation B: Using a manually implemented array-backed stack.
 *
 * Same algorithm, but uses a fixed-capacity stack built on a pre-allocated Array.
 */
object ManualStackNextGreater {
  def solve(nums: Array[Int]): Array[Int] = {
    val result = Array.fill(nums.length)(-1)
    val stack = new ArrayStack(nums.length)

    for (i <- nums.indices.reverse) {
      while (!stack.isEmpty && stack.top <= nums(i)) {
        stack.pop()
      }
      if (!stack.isEmpty) {
        result(i) = stack.top
      }
      stack.push(nums(i))
    }

    result
  }
}
