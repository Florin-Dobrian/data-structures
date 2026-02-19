#include <iostream>
#include <vector>
#include <deque>
#include <numeric>
#include <string>

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

int main() {
    int size = 3;
    std::vector<double> data = {10.0, 20.0, 30.0, 40.0, 50.0};

    // Test Deque
    DequeTracker dt(size);
    std::cout << "--- Testing DequeTracker ---" << std::endl;
    for (double val : data) {
        dt.add(val);
        std::cout << "Added " << val << ": Average = " << dt.get_average() << std::endl;
    }

    // Test Circular Buffer
    CircularBufferTracker cbt(size);
    std::cout << "\n--- Testing CircularBufferTracker ---" << std::endl;
    for (double val : data) {
        cbt.add(val);
        std::cout << "Added " << val << ": Average = " << cbt.get_average() << std::endl;
    }

    return 0;
}
