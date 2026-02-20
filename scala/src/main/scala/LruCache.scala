import scala.collection.mutable

/**
 * Implementation A: Using mutable.LinkedHashMap
 *
 * LinkedHashMap maintains insertion order. On access we remove and re-insert
 * the key to move it to the end. The first key is the least recently used.
 */
class LinkedHashMapLRUCache(capacity: Int) {
  private val cache = mutable.LinkedHashMap[Int, Int]()

  def get(key: Int): Int = {
    cache.remove(key) match {
      case Some(value) =>
        cache(key) = value
        value
      case None => -1
    }
  }

  def put(key: Int, value: Int): Unit = {
    cache.remove(key)
    if (cache.size == capacity) {
      cache.remove(cache.head._1)
    }
    cache(key) = value
  }
}

/**
 * Implementation B: HashMap + manual doubly linked list using an Array as a node pool.
 *
 * Nodes are pre-allocated in an array. Sentinel head (index 0) and tail (index 1)
 * simplify insertion and removal. Free nodes are managed via a free list.
 */
class ManualLRUCache(capacity: Int) {
  private case class Node(
    var key: Int = 0,
    var value: Int = 0,
    var prev: Int = -1,
    var next: Int = -1
  )

  private val pool = Array.fill(capacity + 2)(Node())
  private val map = mutable.HashMap[Int, Int]()  // key -> pool index
  private var freeHead: Int = 2

  // Initialize sentinels
  pool(0).next = 1  // head -> tail
  pool(1).prev = 0  // tail -> head

  // Build free list
  for (i <- 2 until capacity + 2) {
    pool(i).next = if (i + 1 < capacity + 2) i + 1 else -1
  }

  private def remove(idx: Int): Unit = {
    pool(pool(idx).prev).next = pool(idx).next
    pool(pool(idx).next).prev = pool(idx).prev
  }

  private def addBeforeTail(idx: Int): Unit = {
    val prev = pool(1).prev
    pool(prev).next = idx
    pool(idx).prev = prev
    pool(idx).next = 1
    pool(1).prev = idx
  }

  private def allocNode(): Int = {
    val idx = freeHead
    freeHead = pool(freeHead).next
    idx
  }

  private def freeNode(idx: Int): Unit = {
    pool(idx).next = freeHead
    freeHead = idx
  }

  def get(key: Int): Int = {
    map.get(key) match {
      case Some(idx) =>
        remove(idx)
        addBeforeTail(idx)
        pool(idx).value
      case None => -1
    }
  }

  def put(key: Int, value: Int): Unit = {
    map.get(key) match {
      case Some(idx) =>
        pool(idx).value = value
        remove(idx)
        addBeforeTail(idx)
      case None =>
        if (map.size == capacity) {
          val lru = pool(0).next
          remove(lru)
          map.remove(pool(lru).key)
          freeNode(lru)
        }
        val idx = allocNode()
        pool(idx).key = key
        pool(idx).value = value
        map(key) = idx
        addBeforeTail(idx)
    }
  }
}
