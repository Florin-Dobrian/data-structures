import scala.collection.mutable

/**
 * Implementation A: Using a Mutable Queue
 */
class DequeTracker(size: Int) {
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

object Main extends App {
  val size = 3
  val data = List(10.0, 20.0, 30.0, 40.0, 50.0)

  println("--- Testing DequeTracker ---")
  val dt = new DequeTracker(size)
  data.foreach { v =>
    dt.add(v)
    println(f"Added $v%2.0f: Average = ${dt.getAverage}%.2f")
  }

  println("\n--- Testing CircularBufferTracker ---")
  val cbt = new CircularBufferTracker(size)
  data.foreach { v =>
    cbt.add(v)
    println(f"Added $v%2.0f: Average = ${cbt.getAverage}%.2f")
  }
}
