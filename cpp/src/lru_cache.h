#pragma once

#include <unordered_map>
#include <list>
#include <vector>

/**
 * Implementation A: Using std::unordered_map + std::list
 *
 * The list stores key-value pairs in access order (most recent at back).
 * The map stores iterators into the list for O(1) lookup and splice.
 */
class ListLRUCache {
private:
    int capacity;
    std::list<std::pair<int, int>> order;
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> map;

public:
    ListLRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        order.splice(order.end(), order, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = map.find(key);
        if (it != map.end()) {
            it->second->second = value;
            order.splice(order.end(), order, it->second);
        } else {
            if ((int)map.size() == capacity) {
                auto& lru = order.front();
                map.erase(lru.first);
                order.pop_front();
            }
            order.emplace_back(key, value);
            map[key] = std::prev(order.end());
        }
    }
};

/**
 * Implementation B: Manual doubly linked list using a vector as a node pool.
 *
 * Nodes are allocated from a pre-sized vector. Sentinel head (index 0) and
 * tail (index 1) simplify insertion and removal. Free nodes are reused via
 * a simple free list.
 */
class ManualLRUCache {
private:
    struct Node {
        int key = 0;
        int value = 0;
        int prev = -1;
        int next = -1;
    };

    int capacity;
    std::vector<Node> pool;
    std::unordered_map<int, int> map;  // key -> pool index
    int free_head;

    void remove(int idx) {
        pool[pool[idx].prev].next = pool[idx].next;
        pool[pool[idx].next].prev = pool[idx].prev;
    }

    void add_before_tail(int idx) {
        int prev = pool[1].prev;  // 1 = tail sentinel
        pool[prev].next = idx;
        pool[idx].prev = prev;
        pool[idx].next = 1;
        pool[1].prev = idx;
    }

    int alloc_node() {
        int idx = free_head;
        free_head = pool[free_head].next;
        return idx;
    }

    void free_node(int idx) {
        pool[idx].next = free_head;
        free_head = idx;
    }

public:
    ManualLRUCache(int capacity) : capacity(capacity) {
        // Reserve: 0 = head sentinel, 1 = tail sentinel, 2..capacity+1 = data nodes
        pool.resize(capacity + 2);
        pool[0].next = 1;  // head -> tail
        pool[1].prev = 0;  // tail -> head

        // Build free list from indices 2..capacity+1
        free_head = 2;
        for (int i = 2; i < capacity + 2; i++) {
            pool[i].next = (i + 1 < capacity + 2) ? i + 1 : -1;
        }
    }

    int get(int key) {
        auto it = map.find(key);
        if (it == map.end()) return -1;
        int idx = it->second;
        remove(idx);
        add_before_tail(idx);
        return pool[idx].value;
    }

    void put(int key, int value) {
        auto it = map.find(key);
        if (it != map.end()) {
            int idx = it->second;
            pool[idx].value = value;
            remove(idx);
            add_before_tail(idx);
        } else {
            if ((int)map.size() == capacity) {
                int lru = pool[0].next;
                remove(lru);
                map.erase(pool[lru].key);
                free_node(lru);
            }
            int idx = alloc_node();
            pool[idx].key = key;
            pool[idx].value = value;
            map[key] = idx;
            add_before_tail(idx);
        }
    }
};
