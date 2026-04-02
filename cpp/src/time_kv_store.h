#pragma once

#include <string>
#include <unordered_map>
#include <map>
#include <vector>

/**
 * Implementation A: Using std::map (sorted map / red-black tree).
 *
 * Each key maps to a std::map<int, string> of timestamp -> value.
 * get() uses upper_bound(timestamp) and steps back one to find the
 * largest timestamp <= the query. O(log n) per operation.
 *
 * upper_bound returns an iterator to the first element with key
 * strictly greater than the query — so the entry we want is the
 * one just before it.
 */
class StdMapTimeKV {
private:
    std::unordered_map<std::string, std::map<int, std::string>> store;

public:
    void set(const std::string& key, const std::string& value, int timestamp) {
        store[key][timestamp] = value;
    }

    std::string get(const std::string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& ts_map = it->second;
        auto upper = ts_map.upper_bound(timestamp);
        if (upper == ts_map.begin()) return "";
        --upper;
        return upper->second;
    }
};

/**
 * Implementation B: Using manual binary search on a sorted vector.
 *
 * Each key maps to a flat vector of (timestamp, value) pairs. Because
 * timestamps are inserted in strictly increasing order, the vector
 * stays sorted without any insertion cost — just append.
 *
 * get() uses a hand-written binary search to find the rightmost
 * timestamp <= the query. O(log n) per lookup, O(1) amortized insert.
 *
 * The binary search maintains lo/hi bounds and narrows to find the
 * last entry where timestamp <= target. If mid's timestamp fits, we
 * record it as a candidate and search right for a later one. If it's
 * too large, we search left.
 */
class ManualBinarySearchTimeKV {
private:
    std::unordered_map<std::string, std::vector<std::pair<int, std::string>>> store;

public:
    void set(const std::string& key, const std::string& value, int timestamp) {
        store[key].push_back({timestamp, value});
    }

    std::string get(const std::string& key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& entries = it->second;
        if (entries.empty()) return "";

        // Binary search for rightmost timestamp <= query
        int lo = 0;
        int hi = (int)entries.size() - 1;
        int best = -1;

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (entries[mid].first <= timestamp) {
                best = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }

        if (best == -1) return "";
        return entries[best].second;
    }
};
