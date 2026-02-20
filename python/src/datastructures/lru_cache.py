from collections import OrderedDict


class OrderedDictLRUCache:
    """Implementation A: Using OrderedDict.

    OrderedDict.move_to_end() handles reordering on access,
    and popitem(last=False) evicts the least recently used entry.
    """

    def __init__(self, capacity: int):
        self.capacity = capacity
        self.cache: OrderedDict[int, int] = OrderedDict()

    def get(self, key: int) -> int:
        if key not in self.cache:
            return -1
        self.cache.move_to_end(key)
        return self.cache[key]

    def put(self, key: int, value: int):
        if key in self.cache:
            self.cache.move_to_end(key)
        self.cache[key] = value
        if len(self.cache) > self.capacity:
            self.cache.popitem(last=False)


class _Node:
    """Doubly linked list node for the manual LRU cache."""

    __slots__ = ("key", "value", "prev", "next")

    def __init__(self, key: int = 0, value: int = 0):
        self.key = key
        self.value = value
        self.prev: _Node | None = None
        self.next: _Node | None = None


class ManualLRUCache:
    """Implementation B: HashMap + doubly linked list.

    A dict maps keys to nodes in a doubly linked list.
    The list maintains access order: most recent at the tail,
    least recent at the head. Sentinel head/tail nodes simplify
    edge cases.
    """

    def __init__(self, capacity: int):
        self.capacity = capacity
        self.map: dict[int, _Node] = {}
        # Sentinel nodes
        self.head = _Node()
        self.tail = _Node()
        self.head.next = self.tail
        self.tail.prev = self.head

    def _remove(self, node: _Node):
        node.prev.next = node.next
        node.next.prev = node.prev

    def _add_to_tail(self, node: _Node):
        prev = self.tail.prev
        prev.next = node
        node.prev = prev
        node.next = self.tail
        self.tail.prev = node

    def get(self, key: int) -> int:
        if key not in self.map:
            return -1
        node = self.map[key]
        self._remove(node)
        self._add_to_tail(node)
        return node.value

    def put(self, key: int, value: int):
        if key in self.map:
            node = self.map[key]
            node.value = value
            self._remove(node)
            self._add_to_tail(node)
        else:
            node = _Node(key, value)
            self.map[key] = node
            self._add_to_tail(node)
            if len(self.map) > self.capacity:
                lru = self.head.next
                self._remove(lru)
                del self.map[lru.key]
