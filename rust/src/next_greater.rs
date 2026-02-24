/// Implementation A1: Right-to-left scan, standard stack (Vec).
///
/// "What is my next greater?" — iterate from the end, maintaining a stack
/// of *values* that are candidates. For each element, pop anything smaller
/// or equal, then the stack top is the answer.
///
/// The stack holds values because we can fill in each result[i] immediately
/// as we visit it — we already know what's to the right.
pub fn next_greater_right_to_left(nums: &[i32]) -> Vec<i32> {
    let mut result = vec![-1; nums.len()];
    let mut stack: Vec<i32> = Vec::new(); // values

    for i in (0..nums.len()).rev() {
        while let Some(&top) = stack.last() {
            if top <= nums[i] {
                stack.pop();
            } else {
                break;
            }
        }
        if let Some(&top) = stack.last() {
            result[i] = top;
        }
        stack.push(nums[i]);
    }

    result
}

/// Implementation A2: Left-to-right scan, standard stack (Vec).
///
/// "Whose answer am I?" — iterate from the start, maintaining a stack
/// of *indices* whose next greater element has not yet been found. When
/// the current element is greater than what's on top, it is the answer
/// for that earlier index — pop and record.
///
/// The stack holds indices because we don't know an element's answer at
/// the time we first see it — we only find out later, so we need to go
/// back and fill in result at the correct position.
pub fn next_greater_left_to_right(nums: &[i32]) -> Vec<i32> {
    let mut result = vec![-1; nums.len()];
    let mut stack: Vec<usize> = Vec::new(); // indices

    for i in 0..nums.len() {
        while let Some(&top_idx) = stack.last() {
            if nums[top_idx] < nums[i] {
                stack.pop();
                result[top_idx] = nums[i];
            } else {
                break;
            }
        }
        stack.push(i);
    }

    result
}

/// A minimal stack backed by a pre-allocated Vec.
struct ArrayStack {
    data: Vec<i32>,
    top: isize,
}

impl ArrayStack {
    fn new(capacity: usize) -> Self {
        Self {
            data: vec![0; capacity],
            top: -1,
        }
    }

    fn push(&mut self, value: i32) {
        self.top += 1;
        self.data[self.top as usize] = value;
    }

    fn pop(&mut self) -> i32 {
        let value = self.data[self.top as usize];
        self.top -= 1;
        value
    }

    fn peek(&self) -> i32 {
        self.data[self.top as usize]
    }

    fn is_empty(&self) -> bool {
        self.top == -1
    }
}

/// Implementation B1: Right-to-left scan, manual array-backed stack.
///
/// Same algorithm as A1, but uses a fixed-capacity stack built on a
/// pre-allocated Vec. No dynamic resizing.
pub fn next_greater_right_to_left_manual(nums: &[i32]) -> Vec<i32> {
    let mut result = vec![-1; nums.len()];
    let mut stack = ArrayStack::new(nums.len()); // values

    for i in (0..nums.len()).rev() {
        while !stack.is_empty() && stack.peek() <= nums[i] {
            stack.pop();
        }
        if !stack.is_empty() {
            result[i] = stack.peek();
        }
        stack.push(nums[i]);
    }

    result
}

/// Implementation B2: Left-to-right scan, manual array-backed stack.
///
/// Same algorithm as A2, but uses a fixed-capacity stack built on a
/// pre-allocated Vec. No dynamic resizing.
pub fn next_greater_left_to_right_manual(nums: &[i32]) -> Vec<i32> {
    let mut result = vec![-1; nums.len()];
    let mut stack = ArrayStack::new(nums.len()); // indices

    for i in 0..nums.len() {
        while !stack.is_empty() && nums[stack.peek() as usize] < nums[i] {
            let idx = stack.pop();
            result[idx as usize] = nums[i];
        }
        stack.push(i as i32);
    }

    result
}
