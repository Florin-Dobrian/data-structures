/**
 * Implementation A: Naive union-find (simple parent tracking).
 *
 * Each node points to its parent; the root points to itself. find()
 * walks up the parent chain to the root. unite() links one root to
 * the other with no balancing.
 *
 * Worst case: the tree degenerates into a linked list, making find()
 * O(n). This is the baseline to show how much path compression and
 * union by rank improve performance.
 */
class NaiveUnionFind(n: Int) {
  private val parent = Array.tabulate(n)(identity)  // parent(i) = i
  private var components_ = n

  def find(x: Int): Int = {
    var current = x
    while (parent(current) != current) {
      current = parent(current)
    }
    current
  }

  def unite(a: Int, b: Int): Unit = {
    val ra = find(a)
    val rb = find(b)
    if (ra != rb) {
      parent(ra) = rb
      components_ -= 1
    }
  }

  def count: Int = components_
}

/**
 * Implementation B: Optimized union-find (path compression + union by rank).
 *
 * Path compression: during find(), every node along the path is
 * repointed directly to the root, flattening the tree for future
 * lookups. This is done iteratively with a two-pass approach.
 *
 * Union by rank: when merging two trees, the shorter tree is attached
 * under the taller one. Rank is an upper bound on tree height. This
 * prevents the degenerate linked-list case.
 *
 * Together, these give nearly O(1) amortized per operation — formally
 * O(α(n)) where α is the inverse Ackermann function, which is ≤ 4
 * for any practical input size.
 */
class RankedUnionFind(n: Int) {
  private val parent = Array.tabulate(n)(identity)
  private val rank = new Array[Int](n)  // initialized to 0
  private var components_ = n

  def find(x: Int): Int = {
    // Find root
    var root = x
    while (parent(root) != root) {
      root = parent(root)
    }
    // Path compression: point every node on the path to root
    var current = x
    while (parent(current) != root) {
      val next = parent(current)
      parent(current) = root
      current = next
    }
    root
  }

  def unite(a: Int, b: Int): Unit = {
    val ra = find(a)
    val rb = find(b)
    if (ra != rb) {
      // Union by rank: attach shorter tree under taller
      if (rank(ra) < rank(rb)) {
        parent(ra) = rb
      } else if (rank(ra) > rank(rb)) {
        parent(rb) = ra
      } else {
        parent(rb) = ra
        rank(ra) += 1
      }
      components_ -= 1
    }
  }

  def count: Int = components_
}
