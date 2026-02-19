from ds.trackers import DequeTracker, CircularBufferTracker

def run_research():
    size = 3
    data = [10, 20, 30, 40, 50]
    
    implementations = [
        DequeTracker(size),
        CircularBufferTracker(size)
    ]

    for tracker in implementations:
        name = tracker.__class__.__name__
        print(f"--- Testing {name} ---")
        for val in data:
            tracker.add(val)
            print(f"Added {val:2}: Average = {tracker.get_average():.2f}")
        print()

if __name__ == "__main__":
    run_research()
