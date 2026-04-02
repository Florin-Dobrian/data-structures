use std::cmp::Reverse;
use std::collections::BinaryHeap;

/// Implementation A: Using BinaryHeap (standard library).
///
/// BinaryHeap is a max-heap by default, so we wrap entries in Reverse
/// to get min-heap behavior. Each entry is (value, list_index, element_index).
pub fn merge_k_binary_heap(lists: &[Vec<i32>]) -> Vec<i32> {
    let mut result = Vec::new();
    let mut heap: BinaryHeap<Reverse<(i32, usize, usize)>> = BinaryHeap::new();

    for (i, list) in lists.iter().enumerate() {
        if !list.is_empty() {
            heap.push(Reverse((list[0], i, 0)));
        }
    }

    while let Some(Reverse((val, list_idx, elem_idx))) = heap.pop() {
        result.push(val);
        if elem_idx + 1 < lists[list_idx].len() {
            heap.push(Reverse((lists[list_idx][elem_idx + 1], list_idx, elem_idx + 1)));
        }
    }

    result
}

/// A minimal binary min-heap backed by a pre-allocated Vec.
/// Each entry is (value, list_index, element_index).
/// Comparisons use the value field only.
struct MinHeap {
    data: Vec<(i32, usize, usize)>,
    size: usize,
}

impl MinHeap {
    fn new(capacity: usize) -> Self {
        Self {
            data: vec![(0, 0, 0); capacity],
            size: 0,
        }
    }

    fn push(&mut self, entry: (i32, usize, usize)) {
        self.data[self.size] = entry;
        self.sift_up(self.size);
        self.size += 1;
    }

    fn pop(&mut self) -> (i32, usize, usize) {
        let top = self.data[0];
        self.size -= 1;
        self.data[0] = self.data[self.size];
        self.sift_down(0);
        top
    }

    fn is_empty(&self) -> bool {
        self.size == 0
    }

    fn sift_up(&mut self, mut idx: usize) {
        while idx > 0 {
            let parent = (idx - 1) / 2;
            if self.data[idx].0 < self.data[parent].0 {
                self.data.swap(idx, parent);
                idx = parent;
            } else {
                break;
            }
        }
    }

    fn sift_down(&mut self, mut idx: usize) {
        loop {
            let mut smallest = idx;
            let left = 2 * idx + 1;
            let right = 2 * idx + 2;
            if left < self.size && self.data[left].0 < self.data[smallest].0 {
                smallest = left;
            }
            if right < self.size && self.data[right].0 < self.data[smallest].0 {
                smallest = right;
            }
            if smallest != idx {
                self.data.swap(idx, smallest);
                idx = smallest;
            } else {
                break;
            }
        }
    }
}

/// Implementation B: Using a manually implemented binary min-heap.
///
/// Same algorithm as Implementation A, but uses a fixed-capacity heap
/// built on a pre-allocated Vec. The heap size is at most k.
pub fn merge_k_manual_heap(lists: &[Vec<i32>]) -> Vec<i32> {
    let mut result = Vec::new();
    let mut heap = MinHeap::new(lists.len());

    for (i, list) in lists.iter().enumerate() {
        if !list.is_empty() {
            heap.push((list[0], i, 0));
        }
    }

    while !heap.is_empty() {
        let (val, list_idx, elem_idx) = heap.pop();
        result.push(val);
        if elem_idx + 1 < lists[list_idx].len() {
            heap.push((lists[list_idx][elem_idx + 1], list_idx, elem_idx + 1));
        }
    }

    result
}
