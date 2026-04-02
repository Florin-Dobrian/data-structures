from bisect import bisect_right


class BisectTimeKV:
    """Implementation A: Using bisect (standard library binary search).

    Each key maps to a pair of parallel lists: timestamps and values.
    Timestamps are inserted in strictly increasing order, so the lists
    stay sorted without any extra work.

    get() uses bisect_right to find the insertion point for the query
    timestamp, then steps back one. bisect_right returns the index
    where the query would be inserted to keep the list sorted — the
    entry we want is the one just before that position.
    """

    def __init__(self):
        self.timestamps: dict[str, list[int]] = {}
        self.values: dict[str, list[str]] = {}

    def set(self, key: str, value: str, timestamp: int):
        if key not in self.timestamps:
            self.timestamps[key] = []
            self.values[key] = []
        self.timestamps[key].append(timestamp)
        self.values[key].append(value)

    def get(self, key: str, timestamp: int) -> str:
        if key not in self.timestamps:
            return ""

        ts = self.timestamps[key]
        idx = bisect_right(ts, timestamp)
        if idx == 0:
            return ""
        return self.values[key][idx - 1]


class ManualBinarySearchTimeKV:
    """Implementation B: Using manual binary search on a sorted list.

    Each key maps to a flat list of (timestamp, value) tuples. Because
    timestamps are inserted in strictly increasing order, the list stays
    sorted without any insertion cost — just append.

    get() uses a hand-written binary search to find the rightmost
    timestamp <= the query. If mid's timestamp fits, we record it as a
    candidate and search right for a later one. If it's too large, we
    search left.
    """

    def __init__(self):
        self.store: dict[str, list[tuple[int, str]]] = {}

    def set(self, key: str, value: str, timestamp: int):
        if key not in self.store:
            self.store[key] = []
        self.store[key].append((timestamp, value))

    def get(self, key: str, timestamp: int) -> str:
        if key not in self.store:
            return ""

        entries = self.store[key]
        if not entries:
            return ""

        # Binary search for rightmost timestamp <= query
        lo = 0
        hi = len(entries) - 1
        best = -1

        while lo <= hi:
            mid = lo + (hi - lo) // 2
            if entries[mid][0] <= timestamp:
                best = mid
                lo = mid + 1
            else:
                hi = mid - 1

        if best == -1:
            return ""
        return entries[best][1]
