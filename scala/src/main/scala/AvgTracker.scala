import scala.collection.mutable

/**
 * Implementation A: Using a Mutable Queue
 */
class DequeTracker(size: Int) {
  require(size >= 1, s"size must be at least 1, got $size")

  private val buffer = mutable.Queue[Double]()
  private var currentSum: Double = 0.0

  def add(value: Double): Unit = {
    if (buffer.size == size) {
      currentSum -= buffer.dequeue()
    }
    buffer.enqueue(value)
    currentSum += value
  }

  def getAverage: Double = {
    if (buffer.isEmpty) 0.0 else currentSum / buffer.size
  }
}

/**
 * Implementation B: Manual Circular Buffer
 */
class CircularBufferTracker(size: Int) {
  require(size >= 1, s"size must be at least 1, got $size")

  private val buffer = new Array[Double](size)
  private var head = 0
  private var count = 0
  private var currentSum: Double = 0.0

  def add(value: Double): Unit = {
    if (count == size) {
      currentSum -= buffer(head)
    } else {
      count += 1
    }
    buffer(head) = value
    currentSum += value
    head = (head + 1) % size
  }

  def getAverage: Double = {
    if (count == 0) 0.0 else currentSum / count
  }
}
