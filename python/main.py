from datastructures.avg_tracker import DequeTracker, CircularBufferTracker
from datastructures.lru_cache import OrderedDictLRUCache, ManualLRUCache
from datastructures.next_greater import (
    next_greater_right_to_left, next_greater_left_to_right,
    next_greater_right_to_left_manual, next_greater_left_to_right_manual,
)
from datastructures.merge_k_sorted import merge_k_heapq, merge_k_manual_heap
from datastructures.time_kv_store import BisectTimeKV, ManualBinarySearchTimeKV


def run_trackers():
    print("=== Problem 1: Moving Average Tracker ===\n")
    size = 3
    data = [10, 20, 30, 40, 50]

    for tracker in [DequeTracker(size), CircularBufferTracker(size)]:
        name = tracker.__class__.__name__
        print(f"--- {name} ---")
        for val in data:
            tracker.add(val)
            print(f"Added {val:2}: Average = {tracker.get_average():.2f}")
        print()


def run_lru_cache():
    print("=== Problem 2: LRU Cache ===\n")

    for cache in [OrderedDictLRUCache(2), ManualLRUCache(2)]:
        name = cache.__class__.__name__
        print(f"--- {name} ---")
        cache.put(1, 1)
        cache.put(2, 2)
        print(f"get(1) = {cache.get(1)}")   # 1
        cache.put(3, 3)                      # evicts key 2
        print(f"get(2) = {cache.get(2)}")   # -1
        cache.put(4, 4)                      # evicts key 1
        print(f"get(1) = {cache.get(1)}")   # -1
        print(f"get(3) = {cache.get(3)}")   # 3
        print(f"get(4) = {cache.get(4)}")   # 4
        print()


def run_next_greater():
    print("=== Problem 3: Next Greater Element ===\n")
    data = [4, 5, 2, 25, 7, 18]

    for fn in [next_greater_right_to_left, next_greater_left_to_right,
               next_greater_right_to_left_manual, next_greater_left_to_right_manual]:
        name = fn.__name__
        result = fn(data)
        print(f"--- {name} ---")
        print(f"Input:  {data}")
        print(f"Output: {result}")
        print()


def run_merge_k_sorted():
    print("=== Problem 4: Merge K Sorted Lists ===\n")
    lists = [[1, 4, 7], [2, 5, 8], [3, 6, 9]]

    for fn in [merge_k_heapq, merge_k_manual_heap]:
        name = fn.__name__
        result = fn(lists)
        print(f"--- {name} ---")
        print(f"Input:  {lists}")
        print(f"Output: {result}")
        print()


def run_time_kv_store():
    print("=== Problem 5: Time-Based Key-Value Store ===\n")

    for kv in [BisectTimeKV(), ManualBinarySearchTimeKV()]:
        name = kv.__class__.__name__
        print(f"--- {name} ---")
        kv.set("alice", "alice_v1", 1)
        kv.set("alice", "alice_v2", 4)
        kv.set("alice", "alice_v3", 7)
        kv.set("bob",   "bob_v1",   2)
        kv.set("bob",   "bob_v2",   5)

        print(f'get(alice, 0) = "{kv.get("alice", 0)}"')
        print(f'get(alice, 1) = "{kv.get("alice", 1)}"')
        print(f'get(alice, 3) = "{kv.get("alice", 3)}"')
        print(f'get(alice, 4) = "{kv.get("alice", 4)}"')
        print(f'get(alice, 6) = "{kv.get("alice", 6)}"')
        print(f'get(alice, 9) = "{kv.get("alice", 9)}"')
        print(f'get(bob,   3) = "{kv.get("bob",   3)}"')
        print(f'get(bob,   5) = "{kv.get("bob",   5)}"')
        print(f'get(carol, 1) = "{kv.get("carol", 1)}"')
        print()


if __name__ == "__main__":
    run_trackers()
    run_lru_cache()
    run_next_greater()
    run_merge_k_sorted()
    run_time_kv_store()
