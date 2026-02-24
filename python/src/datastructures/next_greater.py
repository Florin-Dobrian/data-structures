def next_greater_right_to_left(nums: list[int]) -> list[int]:
    """Implementation A1: Right-to-left scan, standard stack (list).

    "What is my next greater?" — iterate from the end, maintaining a stack
    of *values* that are candidates. For each element, pop anything smaller
    or equal, then the stack top is the answer.

    The stack holds values because we can fill in each result[i] immediately
    as we visit it — we already know what's to the right.
    """
    result = [-1] * len(nums)
    stack: list[int] = []  # values

    for i in range(len(nums) - 1, -1, -1):
        while stack and stack[-1] <= nums[i]:
            stack.pop()
        if stack:
            result[i] = stack[-1]
        stack.append(nums[i])

    return result


def next_greater_left_to_right(nums: list[int]) -> list[int]:
    """Implementation A2: Left-to-right scan, standard stack (list).

    "Whose answer am I?" — iterate from the start, maintaining a stack
    of *indices* whose next greater element has not yet been found. When
    the current element is greater than what's on top, it is the answer
    for that earlier index — pop and record.

    The stack holds indices because we don't know an element's answer at
    the time we first see it — we only find out later, so we need to go
    back and fill in result at the correct position.
    """
    result = [-1] * len(nums)
    stack: list[int] = []  # indices

    for i in range(len(nums)):
        while stack and nums[stack[-1]] < nums[i]:
            result[stack.pop()] = nums[i]
        stack.append(i)

    return result


class _ArrayStack:
    """A minimal stack backed by a pre-allocated list."""

    def __init__(self, capacity: int):
        self.data = [0] * capacity
        self.top = -1

    def push(self, value: int):
        self.top += 1
        self.data[self.top] = value

    def pop(self) -> int:
        value = self.data[self.top]
        self.top -= 1
        return value

    def peek(self) -> int:
        return self.data[self.top]

    def is_empty(self) -> bool:
        return self.top == -1


def next_greater_right_to_left_manual(nums: list[int]) -> list[int]:
    """Implementation B1: Right-to-left scan, manual array-backed stack.

    Same algorithm as A1, but uses a fixed-capacity stack built on a
    pre-allocated list. No dynamic resizing — the capacity is known
    upfront (at most len(nums) elements on the stack at once).
    """
    result = [-1] * len(nums)
    stack = _ArrayStack(len(nums))  # values

    for i in range(len(nums) - 1, -1, -1):
        while not stack.is_empty() and stack.peek() <= nums[i]:
            stack.pop()
        if not stack.is_empty():
            result[i] = stack.peek()
        stack.push(nums[i])

    return result


def next_greater_left_to_right_manual(nums: list[int]) -> list[int]:
    """Implementation B2: Left-to-right scan, manual array-backed stack.

    Same algorithm as A2, but uses a fixed-capacity stack built on a
    pre-allocated list. No dynamic resizing.
    """
    result = [-1] * len(nums)
    stack = _ArrayStack(len(nums))  # indices

    for i in range(len(nums)):
        while not stack.is_empty() and nums[stack.peek()] < nums[i]:
            result[stack.pop()] = nums[i]
        stack.push(i)

    return result
