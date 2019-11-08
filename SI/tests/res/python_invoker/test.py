from libPySI import PySIEffect


class Test(PySIEffect):
    def __init__(self):
        self.x1 = 5
        self.x2 = 5.5
        self.x3 = "hello"
        self.x4 = "c"
        self.x5 = [1, 2, 3, 4, 5]
        self.x6 = [[1, 2, 3, 4, 5], [5, 4, 3], [1, 2], [2], [1, 3, 5, 7]]
        self.a = 1

    def on_enter(self, other):
        return 1
