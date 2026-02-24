#include <iostream>
#include <vector>
#include "avg_tracker.h"
#include "lru_cache.h"
#include "next_greater.h"

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

int main() {
    run_trackers();
    run_lru_cache();
    run_next_greater();
    return 0;
}
