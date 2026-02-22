def next_greater_list_stack(nums: list[int]) -> list[int]:
    """Implementation A: Using a list as a stack.

    Python has no dedicated stack type — a list with append/pop is idiomatic.
    We iterate right to left, maintaining a stack of elements that are
    candidates for being the "next greater." For each element, pop anything
    smaller or equal (it can never be the answer for anything to the left),
    then the top of the stack is the answer.
    """
    result = [-1] * len(nums)
    stack: list[int] = []

    for i in range(len(nums) - 1, -1, -1):
        while stack and stack[-1] <= nums[i]:
            stack.pop()
        if stack:
            result[i] = stack[-1]
        stack.append(nums[i])

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


def next_greater_manual_stack(nums: list[int]) -> list[int]:
    """Implementation B: Using a manually implemented array-backed stack.

    Same algorithm as Implementation A, but uses a fixed-capacity stack
    built on a pre-allocated list. No dynamic resizing — the capacity
    is known upfront (at most len(nums) elements on the stack at once).
    """
    result = [-1] * len(nums)
    stack = _ArrayStack(len(nums))

    for i in range(len(nums) - 1, -1, -1):
        while not stack.is_empty() and stack.peek() <= nums[i]:
            stack.pop()
        if not stack.is_empty():
            result[i] = stack.peek()
        stack.push(nums[i])

    return result
