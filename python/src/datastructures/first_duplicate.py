from bisect import bisect_left, insort


def first_duplicate_hash_set(data: list[int]) -> int:
    """Implementation A: Using set (hash set).

    Iterate through the stream and add each value to the set. If the
    value is already present, we've found the first duplicate.
    O(n) expected time, O(n) space.

    Python's set uses a hash table under the hood, so 'in' and 'add'
    are both O(1) amortized.
    """
    seen: set[int] = set()
    for val in data:
        if val in seen:
            return val
        seen.add(val)
    return -1


def first_duplicate_sorted_set(data: list[int]) -> int:
    """Implementation B: Using a sorted list via bisect (sorted set equivalent).

    Python has no built-in sorted set, so we maintain a sorted list and
    use bisect_left for O(log n) lookup and insort for O(n) insertion
    (due to shifting). Overall time is O(n²) worst case.

    The tradeoff vs. the hash set: elements stay in sorted order at all
    times, and behavior is fully deterministic (no hash collisions, no
    randomized iteration order). In practice, the shift cost makes this
    slower for large inputs.

    bisect_left finds where val would go; if that position already holds
    val, it's a duplicate.
    """
    seen: list[int] = []
    for val in data:
        idx = bisect_left(seen, val)
        if idx < len(seen) and seen[idx] == val:
            return val
        insort(seen, val)
    return -1
