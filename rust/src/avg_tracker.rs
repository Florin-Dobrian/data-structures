use std::collections::VecDeque;

/// Implementation A: Using the standard library's double-ended queue.
pub struct DequeTracker {
    size: usize,
    buffer: VecDeque<f64>,
    current_sum: f64,
}

impl DequeTracker {
    pub fn new(size: usize) -> Self {
        assert!(size >= 1, "size must be at least 1, got {}", size);
        Self {
            size,
            buffer: VecDeque::with_capacity(size),
            current_sum: 0.0,
        }
    }

    pub fn add(&mut self, value: f64) {
        if self.buffer.len() == self.size {
            if let Some(old_val) = self.buffer.pop_front() {
                self.current_sum -= old_val;
            }
        }
        self.buffer.push_back(value);
        self.current_sum += value;
    }

    pub fn get_average(&self) -> f64 {
        if self.buffer.is_empty() {
            return 0.0;
        }
        self.current_sum / self.buffer.len() as f64
    }
}

/// Implementation B: Manual Circular Buffer using a Vector.
pub struct CircularBufferTracker {
    size: usize,
    buffer: Vec<f64>,
    head: usize,
    count: usize,
    current_sum: f64,
}

impl CircularBufferTracker {
    pub fn new(size: usize) -> Self {
        assert!(size >= 1, "size must be at least 1, got {}", size);
        Self {
            size,
            buffer: vec![0.0; size],
            head: 0,
            count: 0,
            current_sum: 0.0,
        }
    }

    pub fn add(&mut self, value: f64) {
        if self.count == self.size {
            self.current_sum -= self.buffer[self.head];
        } else {
            self.count += 1;
        }

        self.buffer[self.head] = value;
        self.current_sum += value;
        self.head = (self.head + 1) % self.size;
    }

    pub fn get_average(&self) -> f64 {
        if self.count == 0 {
            return 0.0;
        }
        self.current_sum / self.count as f64
    }
}
