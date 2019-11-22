from libPySI import PySIEffect


class Dummy2(PySIEffect):
    def __init__(self):
        super(Dummy2, self).__init__()

        self.name = "Dummy2"
        self.region_type = "Dummy2"

        self.cap_emit = {
            "TEST2": {"on_enter": self.test_on_enter_emit, "on_continuous": self.test_on_continuous_emit, "on_leave": self.test_on_leave_emit}
        }

        self.cap_recv = {
            "TEST1": {"on_enter": self.test_on_enter_recv, "on_continuous": self.test_on_continuous_recv, "on_leave": self.test_on_leave_recv}
        }

    def __repr__(self):
        return ""

    def test_on_enter_emit(self, other):
        i = 5
        k = 3

        return 2 + 5, "juhu"

    def test_on_continuous_emit(self, other):
        return 0

    def test_on_leave_emit(self, other):
        pass

    def test_on_enter_recv(self, message):
        return 0

    def test_on_continuous_recv(self, message):
        return 0

    def test_on_leave_recv(self, message):
        return 0