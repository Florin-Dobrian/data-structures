#pragma once

#include <vector>
#include <queue>
#include <functional>

/**
 * Implementation A: Using std::priority_queue.
 *
 * std::priority_queue is a max-heap by default, so we use std::greater
 * to turn it into a min-heap. Each entry is {value, list_index, element_index}.
 * The smallest value is always at the top.
 */
inline std::vector<int> merge_k_priority_queue(const std::vector<std::vector<int>>& lists) {
    struct Entry {
        int value;
        int list_idx;
        int elem_idx;
        bool operator>(const Entry& other) const { return value > other.value; }
    };

    std::vector<int> result;
    std::priority_queue<Entry, std::vector<Entry>, std::greater<Entry>> heap;

    for (int i = 0; i < (int)lists.size(); i++) {
        if (!lists[i].empty()) {
            heap.push({lists[i][0], i, 0});
        }
    }

    while (!heap.empty()) {
        auto [val, list_idx, elem_idx] = heap.top();
        heap.pop();
        result.push_back(val);
        if (elem_idx + 1 < (int)lists[list_idx].size()) {
            heap.push({lists[list_idx][elem_idx + 1], list_idx, elem_idx + 1});
        }
    }

    return result;
}

/**
 * A minimal binary min-heap backed by a pre-allocated std::vector.
 * Each entry is {value, list_index, element_index}.
 * Comparisons use the value field only.
 */
class MinHeap {
private:
    struct Entry {
        int value;
        int list_idx;
        int elem_idx;
    };

    std::vector<Entry> data;
    int sz = 0;

    void sift_up(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (data[idx].value < data[parent].value) {
                std::swap(data[idx], data[parent]);
                idx = parent;
            } else {
                break;
            }
        }
    }

    void sift_down(int idx) {
        while (true) {
            int smallest = idx;
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            if (left < sz && data[left].value < data[smallest].value) smallest = left;
            if (right < sz && data[right].value < data[smallest].value) smallest = right;
            if (smallest != idx) {
                std::swap(data[idx], data[smallest]);
                idx = smallest;
            } else {
                break;
            }
        }
    }

public:
    MinHeap(size_t capacity) : data(capacity) {}

    void push(int value, int list_idx, int elem_idx) {
        data[sz] = {value, list_idx, elem_idx};
        sift_up(sz);
        sz++;
    }

    Entry pop() {
        Entry top = data[0];
        sz--;
        data[0] = data[sz];
        sift_down(0);
        return top;
    }

    bool empty() const { return sz == 0; }
};

/**
 * Implementation B: Using a manually implemented binary min-heap.
 *
 * Same algorithm as Implementation A, but uses a fixed-capacity heap
 * built on a pre-allocated vector. The heap size is at most k.
 */
inline std::vector<int> merge_k_manual_heap(const std::vector<std::vector<int>>& lists) {
    std::vector<int> result;
    MinHeap heap(lists.size());

    for (int i = 0; i < (int)lists.size(); i++) {
        if (!lists[i].empty()) {
            heap.push(lists[i][0], i, 0);
        }
    }

    while (!heap.empty()) {
        auto [val, list_idx, elem_idx] = heap.pop();
        result.push_back(val);
        if (elem_idx + 1 < (int)lists[list_idx].size()) {
            heap.push(lists[list_idx][elem_idx + 1], list_idx, elem_idx + 1);
        }
    }

    return result;
}
