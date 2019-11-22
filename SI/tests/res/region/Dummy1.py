from libPySI import PySIEffect


class Dummy1(PySIEffect):
    def __init__(self):
        super(Dummy1, self).__init__()

        self.name = "Canvas"
        self.region_type = "canvas"

        self.cap_emit = {
            "TEST1": {"on_enter": self.test_on_enter_emit, "on_continuous": self.test_on_continuous_emit, "on_leave": self.test_on_leave_emit}
        }

        self.cap_recv = {
            "TEST2": {"on_enter": self.test_on_enter_recv, "on_continuous": self.test_on_continuous_recv, "on_leave": self.test_on_leave_recv}
        }

    def __repr__(self):
        return ""

    def test_on_enter_emit(self, other):
        i = 5
        k = 2

        return 0

    def test_on_continuous_emit(self, other):
        return 0

    def test_on_leave_emit(self, other):
        return 0

    def test_on_enter_recv(self, m1, m2):
        return 0

    def test_on_continuous_recv(self, message):
        return 0

    def test_on_leave_recv(self):
        return 0