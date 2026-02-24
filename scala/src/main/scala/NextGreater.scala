import scala.collection.mutable

/**
 * Implementation A1: Right-to-left scan, mutable.Stack.
 *
 * "What is my next greater?" — iterate from the end, maintaining a stack
 * of *values* that are candidates. For each element, pop anything smaller
 * or equal, then the stack top is the answer.
 *
 * The stack holds values because we can fill in each result(i) immediately
 * as we visit it — we already know what's to the right.
 */
object RightToLeftNextGreater {
  def solve(nums: Array[Int]): Array[Int] = {
    val result = Array.fill(nums.length)(-1)
    val stack = mutable.Stack[Int]() // values

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
 * Implementation A2: Left-to-right scan, mutable.Stack.
 *
 * "Whose answer am I?" — iterate from the start, maintaining a stack
 * of *indices* whose next greater element has not yet been found. When
 * the current element is greater than what's on top, it is the answer
 * for that earlier index — pop and record.
 *
 * The stack holds indices because we don't know an element's answer at
 * the time we first see it — we only find out later, so we need to go
 * back and fill in result at the correct position.
 */
object LeftToRightNextGreater {
  def solve(nums: Array[Int]): Array[Int] = {
    val result = Array.fill(nums.length)(-1)
    val stack = mutable.Stack[Int]() // indices

    for (i <- nums.indices) {
      while (stack.nonEmpty && nums(stack.top) < nums(i)) {
        result(stack.pop()) = nums(i)
      }
      stack.push(i)
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
 * Implementation B1: Right-to-left scan, manual array-backed stack.
 *
 * Same algorithm as A1, but uses a fixed-capacity stack built on a
 * pre-allocated Array. No dynamic resizing.
 */
object RightToLeftManualNextGreater {
  def solve(nums: Array[Int]): Array[Int] = {
    val result = Array.fill(nums.length)(-1)
    val stack = new ArrayStack(nums.length) // values

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

/**
 * Implementation B2: Left-to-right scan, manual array-backed stack.
 *
 * Same algorithm as A2, but uses a fixed-capacity stack built on a
 * pre-allocated Array. No dynamic resizing.
 */
object LeftToRightManualNextGreater {
  def solve(nums: Array[Int]): Array[Int] = {
    val result = Array.fill(nums.length)(-1)
    val stack = new ArrayStack(nums.length) // indices

    for (i <- nums.indices) {
      while (!stack.isEmpty && nums(stack.top) < nums(i)) {
        result(stack.pop()) = nums(i)
      }
      stack.push(i)
    }

    result
  }
}
