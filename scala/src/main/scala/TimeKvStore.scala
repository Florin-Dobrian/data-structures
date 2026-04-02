import scala.collection.mutable

/**
 * Implementation A: Using mutable.TreeMap (sorted map / red-black tree).
 *
 * Each key maps to a TreeMap[Int, String] of timestamp -> value.
 * get() uses to(timestamp).lastOption to find the largest timestamp
 * <= the query. The to() method returns a view of all entries up to
 * and including the given key. O(log n) per operation.
 */
class TreeMapTimeKV {
  private val store = mutable.HashMap[String, mutable.TreeMap[Int, String]]()

  def set(key: String, value: String, timestamp: Int): Unit = {
    store.getOrElseUpdate(key, mutable.TreeMap[Int, String]())(timestamp) = value
  }

  def get(key: String, timestamp: Int): String = {
    store.get(key) match {
      case None => ""
      case Some(tsMap) =>
        tsMap.to(timestamp).lastOption match {
          case Some((_, value)) => value
          case None => ""
        }
    }
  }
}

/**
 * Implementation B: Using manual binary search on a sorted ArrayBuffer.
 *
 * Each key maps to a flat ArrayBuffer of (timestamp, value) pairs.
 * Because timestamps are inserted in strictly increasing order, the
 * buffer stays sorted without any insertion cost — just append.
 *
 * get() uses a hand-written binary search to find the rightmost
 * timestamp <= the query. If mid's timestamp fits, we record it as a
 * candidate and search right for a later one. If it's too large, we
 * search left.
 */
class ManualBinarySearchTimeKV {
  private val store = mutable.HashMap[String, mutable.ArrayBuffer[(Int, String)]]()

  def set(key: String, value: String, timestamp: Int): Unit = {
    store.getOrElseUpdate(key, mutable.ArrayBuffer[(Int, String)]()) += ((timestamp, value))
  }

  def get(key: String, timestamp: Int): String = {
    store.get(key) match {
      case None => ""
      case Some(entries) =>
        if (entries.isEmpty) return ""

        // Binary search for rightmost timestamp <= query
        var lo = 0
        var hi = entries.size - 1
        var best = -1

        while (lo <= hi) {
          val mid = lo + (hi - lo) / 2
          if (entries(mid)._1 <= timestamp) {
            best = mid
            lo = mid + 1
          } else {
            hi = mid - 1
          }
        }

        if (best == -1) "" else entries(best)._2
    }
  }
}
