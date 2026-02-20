object Main extends App {

  def runTrackers(): Unit = {
    println("=== Problem 1: Moving Average Tracker ===\n")
    val size = 3
    val data = List(10.0, 20.0, 30.0, 40.0, 50.0)

    println("--- DequeTracker ---")
    val dt = new DequeTracker(size)
    data.foreach { v =>
      dt.add(v)
      println(f"Added $v%2.0f: Average = ${dt.getAverage}%.2f")
    }

    println("\n--- CircularBufferTracker ---")
    val cbt = new CircularBufferTracker(size)
    data.foreach { v =>
      cbt.add(v)
      println(f"Added $v%2.0f: Average = ${cbt.getAverage}%.2f")
    }
    println()
  }

  def testLru(name: String, get: Int => Int, put: (Int, Int) => Unit): Unit = {
    println(s"--- $name ---")
    put(1, 1)
    put(2, 2)
    println(s"get(1) = ${get(1)}")
    put(3, 3)  // evicts key 2
    println(s"get(2) = ${get(2)}")
    put(4, 4)  // evicts key 1
    println(s"get(1) = ${get(1)}")
    println(s"get(3) = ${get(3)}")
    println(s"get(4) = ${get(4)}")
    println()
  }

  def runLruCache(): Unit = {
    println("=== Problem 2: LRU Cache ===\n")

    val lhm = new LinkedHashMapLRUCache(2)
    testLru("LinkedHashMapLRUCache", lhm.get, lhm.put)

    val mc = new ManualLRUCache(2)
    testLru("ManualLRUCache", mc.get, mc.put)
  }

  runTrackers()
  runLruCache()
}
