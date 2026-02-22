/// Implementation A: Using Vec as a stack.
///
/// Rust has no dedicated stack type — Vec with push/pop is idiomatic.
/// Iterate right to left, maintaining a monotonically decreasing stack.
pub fn next_greater_vec_stack(nums: &[i32]) -> Vec<i32> {
    let mut result = vec![-1; nums.len()];
    let mut stack: Vec<i32> = Vec::new();

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

/// Implementation B: Using a manually implemented array-backed stack.
///
/// Same algorithm as Implementation A, but uses a fixed-capacity stack
/// built on a pre-allocated Vec. No dynamic resizing.
pub fn next_greater_manual_stack(nums: &[i32]) -> Vec<i32> {
    let mut result = vec![-1; nums.len()];
    let mut stack = ArrayStack::new(nums.len());

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
