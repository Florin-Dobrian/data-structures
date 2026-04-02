class HashMapTrie:
    """Implementation A: Hash map trie (flexible alphabet).

    Each node stores children in a plain dict[str, Node]. Python dicts
    are hash maps, so this supports any character set. Lookup and
    insertion are O(L) where L is the word length, with O(1) amortized
    per character.

    This is the most natural Python implementation — nesting dicts is
    idiomatic and concise. No explicit memory management needed.
    """

    def __init__(self):
        self._children: dict[str, "HashMapTrie"] = {}
        self._is_end = False

    def insert(self, word: str):
        current = self
        for c in word:
            if c not in current._children:
                current._children[c] = HashMapTrie()
            current = current._children[c]
        current._is_end = True

    def search(self, word: str) -> bool:
        node = self._traverse(word)
        return node is not None and node._is_end

    def starts_with(self, prefix: str) -> bool:
        return self._traverse(prefix) is not None

    def _traverse(self, s: str) -> "HashMapTrie | None":
        current = self
        for c in s:
            if c not in current._children:
                return None
            current = current._children[c]
        return current


class ArrayTrie:
    """Implementation B: Fixed-array trie (lowercase ASCII only).

    Each node stores children in a fixed list of 26 slots, one per
    letter a-z. Lookup is a direct index: children[ord(c) - ord('a')].
    This trades memory (26 slots per node regardless of usage) for
    speed (no hashing, direct index access).

    For sparse tries with few children per node, the hash map version
    uses less memory. For dense tries (e.g. dictionaries), the array
    version is faster due to direct indexing.
    """

    def __init__(self):
        self._children: list["ArrayTrie | None"] = [None] * 26
        self._is_end = False

    def insert(self, word: str):
        current = self
        for c in word:
            idx = ord(c) - ord('a')
            if current._children[idx] is None:
                current._children[idx] = ArrayTrie()
            current = current._children[idx]
        current._is_end = True

    def search(self, word: str) -> bool:
        node = self._traverse(word)
        return node is not None and node._is_end

    def starts_with(self, prefix: str) -> bool:
        return self._traverse(prefix) is not None

    def _traverse(self, s: str) -> "ArrayTrie | None":
        current = self
        for c in s:
            idx = ord(c) - ord('a')
            if current._children[idx] is None:
                return None
            current = current._children[idx]
        return current
