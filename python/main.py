from datastructures.avg_tracker import DequeTracker, CircularBufferTracker
from datastructures.lru_cache import OrderedDictLRUCache, ManualLRUCache
from datastructures.next_greater import (
    next_greater_right_to_left, next_greater_left_to_right,
    next_greater_right_to_left_manual, next_greater_left_to_right_manual,
)
from datastructures.merge_k_sorted import merge_k_heapq, merge_k_manual_heap
from datastructures.time_kv_store import BisectTimeKV, ManualBinarySearchTimeKV
from datastructures.first_duplicate import first_duplicate_hash_set, first_duplicate_sorted_set
from datastructures.prefix_trie import HashMapTrie, ArrayTrie
from datastructures.union_find import NaiveUnionFind, RankedUnionFind


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


def run_first_duplicate():
    print("=== Problem 6: First Duplicate in a Stream ===\n")

    cases = [
        ("has duplicate",    [2, 1, 3, 5, 3, 2]),
        ("immediate repeat", [7, 7, 1, 2]),
        ("no duplicate",     [1, 2, 3, 4, 5]),
        ("empty",            []),
    ]

    for fn in [first_duplicate_hash_set, first_duplicate_sorted_set]:
        name = fn.__name__
        print(f"--- {name} ---")
        for label, data in cases:
            print(f"{label}: {data} -> {fn(data)}")
        print()


def run_prefix_trie():
    print("=== Problem 7: Prefix Trie ===\n")

    for trie in [HashMapTrie(), ArrayTrie()]:
        name = trie.__class__.__name__
        print(f"--- {name} ---")
        trie.insert("apple")
        trie.insert("app")
        trie.insert("apricot")
        trie.insert("banana")

        print(f"search(apple)   = {trie.search('apple')}")
        print(f"search(app)     = {trie.search('app')}")
        print(f"search(ap)      = {trie.search('ap')}")
        print(f"search(banana)  = {trie.search('banana')}")
        print(f"search(band)    = {trie.search('band')}")
        print(f"starts_with(ap) = {trie.starts_with('ap')}")
        print(f"starts_with(ba) = {trie.starts_with('ba')}")
        print(f"starts_with(ca) = {trie.starts_with('ca')}")
        print()


def run_union_find():
    print("=== Problem 8: Connected Components (Union-Find) ===\n")

    n = 5
    edges = [(0, 1), (1, 2), (3, 4)]

    for uf in [NaiveUnionFind(n), RankedUnionFind(n)]:
        name = uf.__class__.__name__
        print(f"--- {name} ---")
        print(f"Nodes: {n}, Edges: {edges}")

        for a, b in edges:
            uf.unite(a, b)

        print(f"Components: {uf.count()}")
        print(f"find(0) = {uf.find(0)}, find(2) = {uf.find(2)}"
              f" (same: {uf.find(0) == uf.find(2)})")
        print(f"find(0) = {uf.find(0)}, find(3) = {uf.find(3)}"
              f" (same: {uf.find(0) == uf.find(3)})")
        print()


if __name__ == "__main__":
    run_trackers()
    run_lru_cache()
    run_next_greater()
    run_merge_k_sorted()
    run_time_kv_store()
    run_first_duplicate()
    run_prefix_trie()
    run_union_find()
