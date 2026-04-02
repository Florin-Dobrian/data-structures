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

  def runNextGreater(): Unit = {
    println("=== Problem 3: Next Greater Element ===\n")
    val data = Array(4, 5, 2, 25, 7, 18)

    val solvers: List[(String, Array[Int] => Array[Int])] = List(
      ("RightToLeftNextGreater", RightToLeftNextGreater.solve),
      ("LeftToRightNextGreater", LeftToRightNextGreater.solve),
      ("RightToLeftManualNextGreater", RightToLeftManualNextGreater.solve),
      ("LeftToRightManualNextGreater", LeftToRightManualNextGreater.solve)
    )

    for ((name, solve) <- solvers) {
      val result = solve(data)
      println(s"--- $name ---")
      println(s"Input:  [${data.mkString(", ")}]")
      println(s"Output: [${result.mkString(", ")}]")
      println()
    }
  }

  def runMergeKSorted(): Unit = {
    println("=== Problem 4: Merge K Sorted Lists ===\n")
    val lists = Array(Array(1, 4, 7), Array(2, 5, 8), Array(3, 6, 9))

    val solvers: List[(String, Array[Array[Int]] => Array[Int])] = List(
      ("PriorityQueueMergeK", PriorityQueueMergeK.solve),
      ("ManualHeapMergeK", ManualHeapMergeK.solve)
    )

    val inputStr = lists.map(a => s"[${a.mkString(", ")}]").mkString("[", ", ", "]")
    for ((name, solve) <- solvers) {
      val result = solve(lists)
      println(s"--- $name ---")
      println(s"Input:  $inputStr")
      println(s"Output: [${result.mkString(", ")}]")
      println()
    }
  }

  def runTimeKvStore(): Unit = {
    println("=== Problem 5: Time-Based Key-Value Store ===\n")

    val solvers: List[(String, (String, String, Int) => Unit, (String, Int) => String)] = {
      val tm = new TreeMapTimeKV
      val mb = new ManualBinarySearchTimeKV
      List(
        ("TreeMapTimeKV", tm.set, tm.get),
        ("ManualBinarySearchTimeKV", mb.set, mb.get)
      )
    }

    for ((name, set, get) <- solvers) {
      println(s"--- $name ---")
      set("alice", "alice_v1", 1)
      set("alice", "alice_v2", 4)
      set("alice", "alice_v3", 7)
      set("bob",   "bob_v1",   2)
      set("bob",   "bob_v2",   5)

      println(s"""get(alice, 0) = "${get("alice", 0)}"""")
      println(s"""get(alice, 1) = "${get("alice", 1)}"""")
      println(s"""get(alice, 3) = "${get("alice", 3)}"""")
      println(s"""get(alice, 4) = "${get("alice", 4)}"""")
      println(s"""get(alice, 6) = "${get("alice", 6)}"""")
      println(s"""get(alice, 9) = "${get("alice", 9)}"""")
      println(s"""get(bob,   3) = "${get("bob",   3)}"""")
      println(s"""get(bob,   5) = "${get("bob",   5)}"""")
      println(s"""get(carol, 1) = "${get("carol", 1)}"""")
      println()
    }
  }

  runTrackers()
  runLruCache()
  runNextGreater()
  runMergeKSorted()
  runTimeKvStore()
}
