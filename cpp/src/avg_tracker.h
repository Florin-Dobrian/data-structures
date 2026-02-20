#pragma once

#include <deque>
#include <vector>

/**
 * Implementation A: Using std::deque
 * std::deque is a double-ended queue that allows O(1) insertion/deletion at both ends.
 */
class DequeTracker {
private:
    std::deque<double> buffer;
    size_t max_size;
    double current_sum = 0.0;

public:
    DequeTracker(size_t size) : max_size(size) {}

    void add(double value) {
        if (buffer.size() == max_size) {
            current_sum -= buffer.front();
            buffer.pop_front();
        }
        buffer.push_back(value);
        current_sum += value;
    }

    double get_average() const {
        if (buffer.empty()) return 0.0;
        return current_sum / buffer.size();
    }
};

/**
 * Implementation B: Manual Circular Buffer
 * Uses a fixed-size std::vector and an index to track the "head".
 */
class CircularBufferTracker {
private:
    std::vector<double> buffer;
    size_t max_size;
    size_t head = 0;
    size_t count = 0;
    double current_sum = 0.0;

public:
    CircularBufferTracker(size_t size) : max_size(size), buffer(size, 0.0) {}

    void add(double value) {
        if (count == max_size) {
            current_sum -= buffer[head];
        } else {
            count++;
        }
        buffer[head] = value;
        current_sum += value;
        head = (head + 1) % max_size;
    }

    double get_average() const {
        if (count == 0) return 0.0;
        return current_sum / count;
    }
};
