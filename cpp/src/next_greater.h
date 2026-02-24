#pragma once

#include <vector>
#include <stack>

/**
 * Implementation A1: Right-to-left scan, std::stack.
 *
 * "What is my next greater?" — iterate from the end, maintaining a stack
 * of *values* that are candidates. For each element, pop anything smaller
 * or equal, then the stack top is the answer.
 *
 * The stack holds values because we can fill in each result[i] immediately
 * as we visit it — we already know what's to the right.
 */
inline std::vector<int> next_greater_right_to_left(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    std::stack<int> stk;  // values

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
 * Implementation A2: Left-to-right scan, std::stack.
 *
 * "Whose answer am I?" — iterate from the start, maintaining a stack
 * of *indices* whose next greater element has not yet been found. When
 * the current element is greater than what's on top, it is the answer
 * for that earlier index — pop and record.
 *
 * The stack holds indices because we don't know an element's answer at
 * the time we first see it — we only find out later, so we need to go
 * back and fill in result at the correct position.
 */
inline std::vector<int> next_greater_left_to_right(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    std::stack<int> stk;  // indices

    for (int i = 0; i < (int)nums.size(); i++) {
        while (!stk.empty() && nums[stk.top()] < nums[i]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }

    return result;
}

/**
 * A fixed-capacity stack using a std::vector and a top index.
 * No dynamic resizing — capacity is known upfront.
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

/**
 * Implementation B1: Right-to-left scan, manual array-backed stack.
 *
 * Same algorithm as A1, but uses a fixed-capacity stack.
 */
inline std::vector<int> next_greater_right_to_left_manual(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    ArrayStack stk(nums.size());  // values

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
 * Implementation B2: Left-to-right scan, manual array-backed stack.
 *
 * Same algorithm as A2, but uses a fixed-capacity stack.
 */
inline std::vector<int> next_greater_left_to_right_manual(const std::vector<int>& nums) {
    std::vector<int> result(nums.size(), -1);
    ArrayStack stk(nums.size());  // indices

    for (int i = 0; i < (int)nums.size(); i++) {
        while (!stk.empty() && nums[stk.top()] < nums[i]) {
            result[stk.top()] = nums[i];
            stk.pop();
        }
        stk.push(i);
    }

    return result;
}
