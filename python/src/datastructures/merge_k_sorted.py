import heapq


def merge_k_heapq(lists: list[list[int]]) -> list[int]:
    """Implementation A: Using heapq (standard library min-heap).

    Push (value, list_index, element_index) tuples onto the heap.
    Python's heapq compares tuples lexicographically, so the smallest
    value always comes out first. The list_index breaks ties
    deterministically (avoids comparing elements from different lists).
    """
    result: list[int] = []
    heap: list[tuple[int, int, int]] = []

    for i, lst in enumerate(lists):
        if lst:
            heapq.heappush(heap, (lst[0], i, 0))

    while heap:
        val, list_idx, elem_idx = heapq.heappop(heap)
        result.append(val)
        if elem_idx + 1 < len(lists[list_idx]):
            next_val = lists[list_idx][elem_idx + 1]
            heapq.heappush(heap, (next_val, list_idx, elem_idx + 1))

    return result


class _MinHeap:
    """A minimal binary min-heap backed by a pre-allocated list.

    Each entry is a (value, list_index, element_index) tuple.
    Comparisons use the value field only.
    """

    def __init__(self, capacity: int):
        self.data: list[tuple[int, int, int]] = [(0, 0, 0)] * capacity
        self.size = 0

    def push(self, entry: tuple[int, int, int]):
        self.data[self.size] = entry
        self._sift_up(self.size)
        self.size += 1

    def pop(self) -> tuple[int, int, int]:
        top = self.data[0]
        self.size -= 1
        self.data[0] = self.data[self.size]
        self._sift_down(0)
        return top

    def is_empty(self) -> bool:
        return self.size == 0

    def _sift_up(self, idx: int):
        while idx > 0:
            parent = (idx - 1) // 2
            if self.data[idx][0] < self.data[parent][0]:
                self.data[idx], self.data[parent] = self.data[parent], self.data[idx]
                idx = parent
            else:
                break

    def _sift_down(self, idx: int):
        while True:
            smallest = idx
            left = 2 * idx + 1
            right = 2 * idx + 2
            if left < self.size and self.data[left][0] < self.data[smallest][0]:
                smallest = left
            if right < self.size and self.data[right][0] < self.data[smallest][0]:
                smallest = right
            if smallest != idx:
                self.data[idx], self.data[smallest] = self.data[smallest], self.data[idx]
                idx = smallest
            else:
                break


def merge_k_manual_heap(lists: list[list[int]]) -> list[int]:
    """Implementation B: Using a manually implemented binary min-heap.

    Same algorithm as Implementation A, but uses a fixed-capacity heap
    built on a pre-allocated list. The heap size is at most k (the number
    of input lists).
    """
    result: list[int] = []
    heap = _MinHeap(len(lists))

    for i, lst in enumerate(lists):
        if lst:
            heap.push((lst[0], i, 0))

    while not heap.is_empty():
        val, list_idx, elem_idx = heap.pop()
        result.append(val)
        if elem_idx + 1 < len(lists[list_idx]):
            next_val = lists[list_idx][elem_idx + 1]
            heap.push((next_val, list_idx, elem_idx + 1))

    return result
