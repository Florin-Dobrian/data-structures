#include <iostream>
#include <vector>
#include "avg_tracker.h"
#include "lru_cache.h"
#include "next_greater.h"
#include "merge_k_sorted.h"
#include "time_kv_store.h"
#include "first_duplicate.h"
#include "prefix_trie.h"
#include "union_find.h"

void run_trackers() {
    std::cout << "=== Problem 1: Moving Average Tracker ===\n" << std::endl;
    int size = 3;
    std::vector<double> data = {10.0, 20.0, 30.0, 40.0, 50.0};

    DequeTracker dt(size);
    std::cout << "--- DequeTracker ---" << std::endl;
    for (double val : data) {
        dt.add(val);
        std::cout << "Added " << val << ": Average = " << dt.get_average() << std::endl;
    }

    std::cout << std::endl;

    CircularBufferTracker cbt(size);
    std::cout << "--- CircularBufferTracker ---" << std::endl;
    for (double val : data) {
        cbt.add(val);
        std::cout << "Added " << val << ": Average = " << cbt.get_average() << std::endl;
    }

    std::cout << std::endl;
}

void run_lru_cache() {
    std::cout << "=== Problem 2: LRU Cache ===\n" << std::endl;

    auto test = [](auto& cache, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        cache.put(1, 1);
        cache.put(2, 2);
        std::cout << "get(1) = " << cache.get(1) << std::endl;
        cache.put(3, 3);  // evicts key 2
        std::cout << "get(2) = " << cache.get(2) << std::endl;
        cache.put(4, 4);  // evicts key 1
        std::cout << "get(1) = " << cache.get(1) << std::endl;
        std::cout << "get(3) = " << cache.get(3) << std::endl;
        std::cout << "get(4) = " << cache.get(4) << std::endl;
        std::cout << std::endl;
    };

    ListLRUCache lc(2);
    test(lc, "ListLRUCache");

    ManualLRUCache mc(2);
    test(mc, "ManualLRUCache");
}

void run_next_greater() {
    std::cout << "=== Problem 3: Next Greater Element ===\n" << std::endl;
    std::vector<int> data = {4, 5, 2, 25, 7, 18};

    auto print_result = [&](const std::string& name, const std::vector<int>& result) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "Input:  [";
        for (size_t i = 0; i < data.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << data[i];
        }
        std::cout << "]" << std::endl;
        std::cout << "Output: [";
        for (size_t i = 0; i < result.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << result[i];
        }
        std::cout << "]" << std::endl << std::endl;
    };

    print_result("next_greater_right_to_left", next_greater_right_to_left(data));
    print_result("next_greater_left_to_right", next_greater_left_to_right(data));
    print_result("next_greater_right_to_left_manual", next_greater_right_to_left_manual(data));
    print_result("next_greater_left_to_right_manual", next_greater_left_to_right_manual(data));
}

void run_merge_k_sorted() {
    std::cout << "=== Problem 4: Merge K Sorted Lists ===\n" << std::endl;
    std::vector<std::vector<int>> lists = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};

    auto print_result = [&](const std::string& name, const std::vector<int>& result) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "Input:  [";
        for (size_t i = 0; i < lists.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << "[";
            for (size_t j = 0; j < lists[i].size(); j++) {
                if (j > 0) std::cout << ", ";
                std::cout << lists[i][j];
            }
            std::cout << "]";
        }
        std::cout << "]" << std::endl;
        std::cout << "Output: [";
        for (size_t i = 0; i < result.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << result[i];
        }
        std::cout << "]" << std::endl << std::endl;
    };

    print_result("merge_k_priority_queue", merge_k_priority_queue(lists));
    print_result("merge_k_manual_heap", merge_k_manual_heap(lists));
}

void run_time_kv_store() {
    std::cout << "=== Problem 5: Time-Based Key-Value Store ===\n" << std::endl;

    auto test = [](auto& kv, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        kv.set("alice", "alice_v1", 1);
        kv.set("alice", "alice_v2", 4);
        kv.set("alice", "alice_v3", 7);
        kv.set("bob",   "bob_v1",   2);
        kv.set("bob",   "bob_v2",   5);

        std::cout << "get(alice, 0) = \"" << kv.get("alice", 0) << "\"" << std::endl;
        std::cout << "get(alice, 1) = \"" << kv.get("alice", 1) << "\"" << std::endl;
        std::cout << "get(alice, 3) = \"" << kv.get("alice", 3) << "\"" << std::endl;
        std::cout << "get(alice, 4) = \"" << kv.get("alice", 4) << "\"" << std::endl;
        std::cout << "get(alice, 6) = \"" << kv.get("alice", 6) << "\"" << std::endl;
        std::cout << "get(alice, 9) = \"" << kv.get("alice", 9) << "\"" << std::endl;
        std::cout << "get(bob,   3) = \"" << kv.get("bob",   3) << "\"" << std::endl;
        std::cout << "get(bob,   5) = \"" << kv.get("bob",   5) << "\"" << std::endl;
        std::cout << "get(carol, 1) = \"" << kv.get("carol", 1) << "\"" << std::endl;
        std::cout << std::endl;
    };

    StdMapTimeKV sm;
    test(sm, "StdMapTimeKV");

    ManualBinarySearchTimeKV mb;
    test(mb, "ManualBinarySearchTimeKV");
}

void run_first_duplicate() {
    std::cout << "=== Problem 6: First Duplicate in a Stream ===\n" << std::endl;

    struct TestCase { std::string label; std::vector<int> data; };
    std::vector<TestCase> cases = {
        {"has duplicate",    {2, 1, 3, 5, 3, 2}},
        {"immediate repeat", {7, 7, 1, 2}},
        {"no duplicate",     {1, 2, 3, 4, 5}},
        {"empty",            {}},
    };

    auto print_vec = [](const std::vector<int>& v) {
        std::cout << "[";
        for (size_t i = 0; i < v.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << v[i];
        }
        std::cout << "]";
    };

    auto run = [&](const std::string& name, int(*fn)(const std::vector<int>&)) {
        std::cout << "--- " << name << " ---" << std::endl;
        for (auto& tc : cases) {
            std::cout << tc.label << ": ";
            print_vec(tc.data);
            std::cout << " -> " << fn(tc.data) << std::endl;
        }
        std::cout << std::endl;
    };

    run("first_duplicate_hash_set", first_duplicate_hash_set);
    run("first_duplicate_sorted_set", first_duplicate_sorted_set);
}

void run_prefix_trie() {
    std::cout << "=== Problem 7: Prefix Trie ===\n" << std::endl;

    auto test = [](auto& trie, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        trie.insert("apple");
        trie.insert("app");
        trie.insert("apricot");
        trie.insert("banana");

        std::cout << "search(apple)   = " << std::boolalpha << trie.search("apple")   << std::endl;
        std::cout << "search(app)     = " << std::boolalpha << trie.search("app")     << std::endl;
        std::cout << "search(ap)      = " << std::boolalpha << trie.search("ap")      << std::endl;
        std::cout << "search(banana)  = " << std::boolalpha << trie.search("banana")  << std::endl;
        std::cout << "search(band)    = " << std::boolalpha << trie.search("band")    << std::endl;
        std::cout << "starts_with(ap) = " << std::boolalpha << trie.starts_with("ap") << std::endl;
        std::cout << "starts_with(ba) = " << std::boolalpha << trie.starts_with("ba") << std::endl;
        std::cout << "starts_with(ca) = " << std::boolalpha << trie.starts_with("ca") << std::endl;
        std::cout << std::endl;
    };

    HashMapTrie hm;
    test(hm, "HashMapTrie");

    ArrayTrie at;
    test(at, "ArrayTrie");
}

void run_union_find() {
    std::cout << "=== Problem 8: Connected Components (Union-Find) ===\n" << std::endl;

    // Graph: 5 nodes, edges form two components: {0,1,2} and {3,4}
    int n = 5;
    std::vector<std::pair<int,int>> edges = {{0,1}, {1,2}, {3,4}};

    auto test = [&](auto& uf, const std::string& name) {
        std::cout << "--- " << name << " ---" << std::endl;
        std::cout << "Nodes: " << n << ", Edges: [";
        for (size_t i = 0; i < edges.size(); i++) {
            if (i > 0) std::cout << ", ";
            std::cout << "(" << edges[i].first << "," << edges[i].second << ")";
        }
        std::cout << "]" << std::endl;

        for (auto& [a, b] : edges) {
            uf.unite(a, b);
        }

        std::cout << "Components: " << uf.count() << std::endl;
        std::cout << "find(0) = " << uf.find(0) << ", find(2) = " << uf.find(2)
                  << " (same: " << std::boolalpha << (uf.find(0) == uf.find(2)) << ")" << std::endl;
        std::cout << "find(0) = " << uf.find(0) << ", find(3) = " << uf.find(3)
                  << " (same: " << std::boolalpha << (uf.find(0) == uf.find(3)) << ")" << std::endl;
        std::cout << std::endl;
    };

    NaiveUnionFind nuf(n);
    test(nuf, "NaiveUnionFind");

    RankedUnionFind ruf(n);
    test(ruf, "RankedUnionFind");
}

int main() {
    run_trackers();
    run_lru_cache();
    run_next_greater();
    run_merge_k_sorted();
    run_time_kv_store();
    run_first_duplicate();
    run_prefix_trie();
    run_union_find();
    return 0;
}
