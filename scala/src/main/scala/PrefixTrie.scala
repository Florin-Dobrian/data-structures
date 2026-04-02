import scala.collection.mutable

/**
 * Implementation A: Hash map trie (flexible alphabet).
 *
 * Each node stores children in a mutable.HashMap[Char, Node]. This
 * supports any character set without wasting memory on unused slots.
 * Lookup and insertion are O(L) where L is the word length, with O(1)
 * amortized per character via the hash map.
 */
class HashMapTrie {
  private val children = mutable.HashMap[Char, HashMapTrie]()
  private var isEnd = false

  def insert(word: String): Unit = {
    var current = this
    for (c <- word) {
      current = current.children.getOrElseUpdate(c, new HashMapTrie)
    }
    current.isEnd = true
  }

  def search(word: String): Boolean = {
    val node = traverse(word)
    node != null && node.isEnd
  }

  def startsWith(prefix: String): Boolean = {
    traverse(prefix) != null
  }

  private def traverse(s: String): HashMapTrie = {
    var current = this
    var i = 0
    while (i < s.length) {
      current.children.get(s(i)) match {
        case Some(child) => current = child
        case None => return null
      }
      i += 1
    }
    current
  }
}

/**
 * Implementation B: Fixed-array trie (lowercase ASCII only).
 *
 * Each node stores children in a fixed Array of 26 slots, one per
 * letter a-z. Lookup is a direct index: children(c - 'a'). This
 * trades memory (26 references per node regardless of usage) for
 * speed (no hashing, direct index access).
 *
 * For sparse tries with few children per node, the hash map version
 * uses less memory. For dense tries (e.g. dictionaries), the array
 * version is faster due to direct indexing.
 */
class ArrayTrie {
  private val children = new Array[ArrayTrie](26)
  private var isEnd = false

  def insert(word: String): Unit = {
    var current = this
    for (c <- word) {
      val idx = c - 'a'
      if (current.children(idx) == null) {
        current.children(idx) = new ArrayTrie
      }
      current = current.children(idx)
    }
    current.isEnd = true
  }

  def search(word: String): Boolean = {
    val node = traverse(word)
    node != null && node.isEnd
  }

  def startsWith(prefix: String): Boolean = {
    traverse(prefix) != null
  }

  private def traverse(s: String): ArrayTrie = {
    var current = this
    var i = 0
    while (i < s.length) {
      val idx = s(i) - 'a'
      if (current.children(idx) == null) return null
      current = current.children(idx)
      i += 1
    }
    current
  }
}
