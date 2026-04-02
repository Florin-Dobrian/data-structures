import scala.collection.mutable

/**
 * Implementation A: Using mutable.PriorityQueue.
 *
 * Scala's PriorityQueue is a max-heap by default, so we pass a reversed
 * Ordering to get min-heap behavior. Each entry is (value, listIndex, elemIndex).
 */
object PriorityQueueMergeK {
  def solve(lists: Array[Array[Int]]): Array[Int] = {
    val result = mutable.ArrayBuffer[Int]()
    implicit val ord: Ordering[(Int, Int, Int)] = Ordering.by[(Int, Int, Int), Int](_._1).reverse
    val heap = mutable.PriorityQueue[(Int, Int, Int)]()

    for (i <- lists.indices) {
      if (lists(i).nonEmpty) {
        heap.enqueue((lists(i)(0), i, 0))
      }
    }

    while (heap.nonEmpty) {
      val (value, listIdx, elemIdx) = heap.dequeue()
      result += value
      if (elemIdx + 1 < lists(listIdx).length) {
        heap.enqueue((lists(listIdx)(elemIdx + 1), listIdx, elemIdx + 1))
      }
    }

    result.toArray
  }
}

/**
 * A minimal binary min-heap backed by a pre-allocated Array.
 * Each entry is (value, listIndex, elemIndex).
 * Comparisons use the value field only.
 */
class MinHeap(capacity: Int) {
  private val data = new Array[(Int, Int, Int)](capacity)
  private var sz = 0

  def push(entry: (Int, Int, Int)): Unit = {
    data(sz) = entry
    siftUp(sz)
    sz += 1
  }

  def pop(): (Int, Int, Int) = {
    val top = data(0)
    sz -= 1
    data(0) = data(sz)
    siftDown(0)
    top
  }

  def isEmpty: Boolean = sz == 0

  private def siftUp(idx: Int): Unit = {
    var i = idx
    while (i > 0) {
      val parent = (i - 1) / 2
      if (data(i)._1 < data(parent)._1) {
        val tmp = data(i); data(i) = data(parent); data(parent) = tmp
        i = parent
      } else {
        return
      }
    }
  }

  private def siftDown(idx: Int): Unit = {
    var i = idx
    while (true) {
      var smallest = i
      val left = 2 * i + 1
      val right = 2 * i + 2
      if (left < sz && data(left)._1 < data(smallest)._1) smallest = left
      if (right < sz && data(right)._1 < data(smallest)._1) smallest = right
      if (smallest != i) {
        val tmp = data(i); data(i) = data(smallest); data(smallest) = tmp
        i = smallest
      } else {
        return
      }
    }
  }
}

/**
 * Implementation B: Using a manually implemented binary min-heap.
 *
 * Same algorithm as Implementation A, but uses a fixed-capacity heap
 * built on a pre-allocated Array. The heap size is at most k.
 */
object ManualHeapMergeK {
  def solve(lists: Array[Array[Int]]): Array[Int] = {
    val result = mutable.ArrayBuffer[Int]()
    val heap = new MinHeap(lists.length)

    for (i <- lists.indices) {
      if (lists(i).nonEmpty) {
        heap.push((lists(i)(0), i, 0))
      }
    }

    while (!heap.isEmpty) {
      val (value, listIdx, elemIdx) = heap.pop()
      result += value
      if (elemIdx + 1 < lists(listIdx).length) {
        heap.push((lists(listIdx)(elemIdx + 1), listIdx, elemIdx + 1))
      }
    }

    result.toArray
  }
}
