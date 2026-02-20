from datastructures.avg_tracker import DequeTracker, CircularBufferTracker

for TrackerClass in [DequeTracker, CircularBufferTracker]:
    tracker = TrackerClass(size=3)
    for val in [10, 20, 30, 40]:
        tracker.add(val)
        print(f"{TrackerClass.__name__} after {val}: {tracker.get_average()}")
