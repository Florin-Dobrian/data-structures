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

  def runFirstDuplicate(): Unit = {
    println("=== Problem 6: First Duplicate in a Stream ===\n")

    val cases: List[(String, Array[Int])] = List(
      ("has duplicate",    Array(2, 1, 3, 5, 3, 2)),
      ("immediate repeat", Array(7, 7, 1, 2)),
      ("no duplicate",     Array(1, 2, 3, 4, 5)),
      ("empty",            Array[Int]())
    )

    val solvers: List[(String, Array[Int] => Int)] = List(
      ("HashSetFirstDuplicate", HashSetFirstDuplicate.solve),
      ("TreeSetFirstDuplicate", TreeSetFirstDuplicate.solve)
    )

    for ((name, solve) <- solvers) {
      println(s"--- $name ---")
      for ((label, data) <- cases) {
        println(s"$label: [${data.mkString(", ")}] -> ${solve(data)}")
      }
      println()
    }
  }

  def runPrefixTrie(): Unit = {
    println("=== Problem 7: Prefix Trie ===\n")

    val tries: List[(String, String => Unit, String => Boolean, String => Boolean)] = {
      val hm = new HashMapTrie
      val at = new ArrayTrie
      List(
        ("HashMapTrie", hm.insert, hm.search, hm.startsWith),
        ("ArrayTrie",   at.insert, at.search, at.startsWith)
      )
    }

    for ((name, insert, search, startsWith) <- tries) {
      println(s"--- $name ---")
      insert("apple")
      insert("app")
      insert("apricot")
      insert("banana")

      println(s"search(apple)   = ${search("apple")}")
      println(s"search(app)     = ${search("app")}")
      println(s"search(ap)      = ${search("ap")}")
      println(s"search(banana)  = ${search("banana")}")
      println(s"search(band)    = ${search("band")}")
      println(s"starts_with(ap) = ${startsWith("ap")}")
      println(s"starts_with(ba) = ${startsWith("ba")}")
      println(s"starts_with(ca) = ${startsWith("ca")}")
      println()
    }
  }

  def runUnionFind(): Unit = {
    println("=== Problem 8: Connected Components (Union-Find) ===\n")

    val n = 5
    val edges = List((0, 1), (1, 2), (3, 4))

    val solvers: List[(String, Int => { def find(x: Int): Int; def unite(a: Int, b: Int): Unit; def count: Int })] = List()

    // Test NaiveUnionFind
    {
      val uf = new NaiveUnionFind(n)
      println("--- NaiveUnionFind ---")
      println(s"Nodes: $n, Edges: [${edges.map { case (a, b) => s"($a,$b)" }.mkString(", ")}]")
      for ((a, b) <- edges) uf.unite(a, b)
      println(s"Components: ${uf.count}")
      println(s"find(0) = ${uf.find(0)}, find(2) = ${uf.find(2)} (same: ${uf.find(0) == uf.find(2)})")
      println(s"find(0) = ${uf.find(0)}, find(3) = ${uf.find(3)} (same: ${uf.find(0) == uf.find(3)})")
      println()
    }

    // Test RankedUnionFind
    {
      val uf = new RankedUnionFind(n)
      println("--- RankedUnionFind ---")
      println(s"Nodes: $n, Edges: [${edges.map { case (a, b) => s"($a,$b)" }.mkString(", ")}]")
      for ((a, b) <- edges) uf.unite(a, b)
      println(s"Components: ${uf.count}")
      println(s"find(0) = ${uf.find(0)}, find(2) = ${uf.find(2)} (same: ${uf.find(0) == uf.find(2)})")
      println(s"find(0) = ${uf.find(0)}, find(3) = ${uf.find(3)} (same: ${uf.find(0) == uf.find(3)})")
      println()
    }
  }

  /** Print an m x n matrix as a grid, given its entries in row-major order. */
  def printGrid(rowMajor: Array[Double], m: Int, n: Int): Unit = {
    for (i <- 0 until m) {
      println((0 until n).map(j => f"${rowMajor(i * n + j)}%5.0f").mkString)
    }
  }

  def runDenseMatvec(): Unit = {
    println("=== Problem 9: Dense Matrix-Vector Product ===\n")

    val m = 3
    val n = 4
    val rowMajor = Array(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0)
    val colMajor = Array(1.0, 5.0, 9.0, 2.0, 6.0, 10.0, 3.0, 7.0, 11.0, 4.0, 8.0, 12.0)

    val ra = RowDenseMatrix(m, n, rowMajor)
    val ca = ColDenseMatrix(m, n, colMajor)
    val x = Vector(n, Array(1.0, 2.0, 3.0, 4.0))

    println(s"A is $m x $n:")
    printGrid(rowMajor, m, n)
    println(s"x = [${x.values.mkString(", ")}]\n")

    println("--- rowDenseMatvec (gather) ---")
    println(s"val = [${ra.values.mkString(", ")}]")
    println(s"y = [${rowDenseMatvec(ra, x).values.mkString(", ")}]\n")

    println("--- colDenseMatvec (scatter) ---")
    println(s"val = [${ca.values.mkString(", ")}]")
    println(s"y = [${colDenseMatvec(ca, x).values.mkString(", ")}]\n")
  }

  def runSparseMatvec(): Unit = {
    println("=== Problem 10: Sparse Matrix-Vector Product ===\n")

    val m = 3
    val n = 4

    val ra = CsrMatrix(m, n,
      Array(0, 2, 3, 6),
      Array(0, 2, 1, 0, 2, 3),
      Array(1.0, 2.0, 3.0, 4.0, 5.0, 6.0))

    val ca = CscMatrix(m, n,
      Array(0, 2, 3, 5, 6),
      Array(0, 2, 1, 0, 2, 2),
      Array(1.0, 4.0, 3.0, 2.0, 5.0, 6.0))

    val x = Vector(n, Array(1.0, 2.0, 3.0, 4.0))

    // The dense picture of the same matrix, for the header only.
    val dense = Array(1.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0.0, 5.0, 6.0)

    println(s"A is $m x $n with 6 nonzeros:")
    printGrid(dense, m, n)
    println(s"x = [${x.values.mkString(", ")}]\n")

    println("--- csrSparseMatvec (gather) ---")
    println(s"rowPtr = [${ra.rowPtr.mkString(", ")}]")
    println(s"colIdx = [${ra.colIdx.mkString(", ")}]")
    println(s"val = [${ra.values.mkString(", ")}]")
    println(s"y = [${csrSparseMatvec(ra, x).values.mkString(", ")}]\n")

    println("--- cscSparseMatvec (scatter) ---")
    println(s"colPtr = [${ca.colPtr.mkString(", ")}]")
    println(s"rowIdx = [${ca.rowIdx.mkString(", ")}]")
    println(s"val = [${ca.values.mkString(", ")}]")
    println(s"y = [${cscSparseMatvec(ca, x).values.mkString(", ")}]\n")
  }

  /** Print a column-major m x n matrix as a grid. */
  def printColGrid(colMajor: Array[Double], m: Int, n: Int): Unit = {
    for (i <- 0 until m) {
      println((0 until n).map(j => f"${colMajor(j * m + i)}%5.0f").mkString)
    }
  }

  def runDenseLuFactor(): Unit = {
    println("=== Problem 11: Dense LU Factorization ===\n")

    // A 4 x 4 built as the product of integer L and U, so the factor comes
    // out in exact integers and the pivots are 2, 3, 4, 5:
    //
    //    2   4  -2   6
    //    4  11  -3   9
    //   -2   5   9 -13
    //    6   6  -4  31
    //
    val n = 4
    val colMajor = Array(2.0, 4.0, -2.0, 6.0, 4.0, 11.0, 5.0, 6.0,
                         -2.0, -3.0, 9.0, -4.0, 6.0, 9.0, -13.0, 31.0)

    val a = ColDenseMatrix(n, n, colMajor)

    println(s"A is $n x $n:")
    printColGrid(a.values, n, n)
    println(s"val = [${a.values.mkString(", ")}]\n")

    def show(name: String, lu: ColDenseMatrix): Unit = {
      println(s"--- $name ---")
      println("LU (U upper, L strict lower, unit diagonal implied):")
      printColGrid(lu.values, n, n)
      println(s"val = [${lu.values.mkString(", ")}]\n")
    }

    show("leftDenseLuFactor (gather)", leftDenseLuFactor(a))
    show("rightDenseLuFactor (scatter)", rightDenseLuFactor(a))
  }

  def runDenseLuSolve(): Unit = {
    println("=== Problem 12: Dense LU Solve ===\n")

    // The factor problem 11 computes, in both layouts. U in the upper
    // triangle, L's multipliers in the strict lower, unit diagonal implied:
    //
    //    2   4  -2   6
    //    2   3   1  -3
    //   -1   3   4   2
    //    3  -2   1   5
    //
    val n = 4
    val rowMajor = Array(2.0, 4.0, -2.0, 6.0, 2.0, 3.0, 1.0, -3.0,
                         -1.0, 3.0, 4.0, 2.0, 3.0, -2.0, 1.0, 5.0)
    val colMajor = Array(2.0, 2.0, -1.0, 3.0, 4.0, 3.0, 3.0, -2.0,
                         -2.0, 1.0, 4.0, 1.0, 6.0, -3.0, 2.0, 5.0)

    val rlu = RowDenseMatrix(n, n, rowMajor)
    val clu = ColDenseMatrix(n, n, colMajor)
    val b = Vector(n, Array(28.0, 53.0, -17.0, 130.0))

    println(s"LU is $n x $n:")
    printGrid(rowMajor, n, n)
    println(s"b = [${b.values.mkString(", ")}]\n")

    println("--- rowDenseLuSolve (gather) ---")
    println(s"val = [${rlu.values.mkString(", ")}]")
    println(s"x = [${rowDenseLuSolve(rlu, b).values.mkString(", ")}]\n")

    println("--- colDenseLuSolve (scatter) ---")
    println(s"val = [${clu.values.mkString(", ")}]")
    println(s"x = [${colDenseLuSolve(clu, b).values.mkString(", ")}]\n")
  }

  def runDenseConvert(): Unit = {
    println("=== Problem 13: Dense Layout Conversion ===\n")

    // The 3 x 4 from problem 9:
    //
    //    1   2   3   4
    //    5   6   7   8
    //    9  10  11  12
    //
    val m = 3
    val n = 4
    val rowMajor = Array(1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0)
    val colMajor = Array(1.0, 5.0, 9.0, 2.0, 6.0, 10.0, 3.0, 7.0, 11.0, 4.0, 8.0, 12.0)

    val ra = RowDenseMatrix(m, n, rowMajor)
    val ca = ColDenseMatrix(m, n, colMajor)

    println(s"A is $m x $n:")
    printGrid(rowMajor, m, n)
    println()

    println("--- rowToColDenseConvert ---")
    println(s"in  (row-major) = [${ra.values.mkString(", ")}]")
    val toCol = rowToColDenseConvert(ra)
    println(s"out (col-major) = [${toCol.values.mkString(", ")}]")
    println(s"round trip      = [${colToRowDenseConvert(toCol).values.mkString(", ")}]\n")

    println("--- colToRowDenseConvert ---")
    println(s"in  (col-major) = [${ca.values.mkString(", ")}]")
    val toRow = colToRowDenseConvert(ca)
    println(s"out (row-major) = [${toRow.values.mkString(", ")}]")
    println(s"round trip      = [${rowToColDenseConvert(toRow).values.mkString(", ")}]\n")
  }

  def runSparseConvert(): Unit = {
    println("=== Problem 14: Sparse Format Conversion ===\n")

    // The 3 x 4 from problem 10, 6 nonzeros:
    //
    //    1   0   2   0
    //    0   3   0   0
    //    4   0   5   6
    //
    val m = 3
    val n = 4
    val ra = CsrMatrix(m, n, Array(0, 2, 3, 6), Array(0, 2, 1, 0, 2, 3),
      Array(1.0, 2.0, 3.0, 4.0, 5.0, 6.0))
    val ca = CscMatrix(m, n, Array(0, 2, 3, 5, 6), Array(0, 2, 1, 0, 2, 2),
      Array(1.0, 4.0, 3.0, 2.0, 5.0, 6.0))

    val dense = Array(1.0, 0.0, 2.0, 0.0, 0.0, 3.0, 0.0, 0.0, 4.0, 0.0, 5.0, 6.0)

    println(s"A is $m x $n with 6 nonzeros:")
    printGrid(dense, m, n)
    println()

    println("--- csrToCscSparseConvert ---")
    println(s"in  rowPtr = [${ra.rowPtr.mkString(", ")}]")
    println(s"in  colIdx = [${ra.colIdx.mkString(", ")}]")
    println(s"in  val    = [${ra.values.mkString(", ")}]")
    val toCsc = csrToCscSparseConvert(ra)
    println(s"out colPtr = [${toCsc.colPtr.mkString(", ")}]")
    println(s"out rowIdx = [${toCsc.rowIdx.mkString(", ")}]")
    println(s"out val    = [${toCsc.values.mkString(", ")}]")
    val backCsr = cscToCsrSparseConvert(toCsc)
    println(s"rt  rowPtr = [${backCsr.rowPtr.mkString(", ")}]")
    println(s"rt  colIdx = [${backCsr.colIdx.mkString(", ")}]")
    println(s"rt  val    = [${backCsr.values.mkString(", ")}]\n")

    println("--- cscToCsrSparseConvert ---")
    println(s"in  colPtr = [${ca.colPtr.mkString(", ")}]")
    println(s"in  rowIdx = [${ca.rowIdx.mkString(", ")}]")
    println(s"in  val    = [${ca.values.mkString(", ")}]")
    val toCsr = cscToCsrSparseConvert(ca)
    println(s"out rowPtr = [${toCsr.rowPtr.mkString(", ")}]")
    println(s"out colIdx = [${toCsr.colIdx.mkString(", ")}]")
    println(s"out val    = [${toCsr.values.mkString(", ")}]")
    val backCsc = csrToCscSparseConvert(toCsr)
    println(s"rt  colPtr = [${backCsc.colPtr.mkString(", ")}]")
    println(s"rt  rowIdx = [${backCsc.rowIdx.mkString(", ")}]")
    println(s"rt  val    = [${backCsc.values.mkString(", ")}]\n")
  }

  runTrackers()
  runLruCache()
  runNextGreater()
  runMergeKSorted()
  runTimeKvStore()
  runFirstDuplicate()
  runPrefixTrie()
  runUnionFind()
  runDenseMatvec()
  runSparseMatvec()
  runDenseLuFactor()
  runDenseLuSolve()
  runDenseConvert()
  runSparseConvert()
}
