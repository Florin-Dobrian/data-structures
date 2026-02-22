from collections import deque

class DequeTracker:
    """Implementation A: Using a dynamic Double-Ended Queue."""
    def __init__(self, size: int):
        if size < 1:
            raise ValueError(f"size must be at least 1, got {size}")
        self.size = size
        self.buffer = deque(maxlen=size)
        self.current_sum = 0.0

    def add(self, value: float):
        if len(self.buffer) == self.size:
            self.current_sum -= self.buffer[0]
        self.buffer.append(value)
        self.current_sum += value

    def get_average(self) -> float:
        if not self.buffer:
            return 0.0
        return self.current_sum / len(self.buffer)


class CircularBufferTracker:
    """Implementation B: Using a fixed-size list with manual index wrapping."""
    def __init__(self, size: int):
        if size < 1:
            raise ValueError(f"size must be at least 1, got {size}")
        self.size = size
        self.buffer = [0.0] * size
        self.head = 0
        self.count = 0
        self.current_sum = 0.0

    def add(self, value: float):
        if self.count == self.size:
            # Buffer is full, subtract the value we are about to overwrite
            self.current_sum -= self.buffer[self.head]
        else:
            self.count += 1
        
        self.buffer[self.head] = value
        self.current_sum += value
        self.head = (self.head + 1) % self.size

    def get_average(self) -> float:
        if self.count == 0:
            return 0.0
        return self.current_sum / self.count
