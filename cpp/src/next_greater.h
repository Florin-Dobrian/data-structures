#pragma once

#include <vector>
#include <stack>

/**
 * Implementation A: Using std::stack
 *
 * std::stack is an adapter over std::deque by default.
 * Iterate right to left, maintaining a monotonically decreasing stack.
 */
inline std::vector<int> next_greater_std_stack(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    std::stack<int> stk;

    for (int i = (int)nums.size() - 1; i >= 0; i--) {
        while (!stk.empty() && stk.top() <= nums[i]) {
            stk.pop();
        }
        if (!stk.empty()) {
            result[i] = stk.top();
        }
        stk.push(nums[i]);
    }

    return result;
}

/**
 * Implementation B: Manual array-backed stack
 *
 * A fixed-capacity stack using a std::vector and a top index.
 * Same algorithm, no dynamic resizing — capacity is known upfront.
 */
class ArrayStack {
private:
    std::vector<int> data;
    int top_idx = -1;

public:
    ArrayStack(size_t capacity) : data(capacity) {}

    void push(int value) { data[++top_idx] = value; }
    int pop() { return data[top_idx--]; }
    int top() const { return data[top_idx]; }
    bool empty() const { return top_idx == -1; }
};

inline std::vector<int> next_greater_manual_stack(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    ArrayStack stk(nums.size());

    for (int i = (int)nums.size() - 1; i >= 0; i--) {
        while (!stk.empty() && stk.top() <= nums[i]) {
            stk.pop();
        }
        if (!stk.empty()) {
            result[i] = stk.top();
        }
        stk.push(nums[i]);
    }

    return result;
}
