#include <iostream>
#include <vector>
#include "avg_tracker.h"
#include "lru_cache.h"

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

int main() {
    run_trackers();
    run_lru_cache();
    return 0;
}
