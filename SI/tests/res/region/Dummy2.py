from libPySI import PySIEffect, PySICapability


class Dummy2(PySIEffect):
    def __init__(self):
        super(Dummy2, self).__init__()

        self.name = "Dummy2"
        self.region_type = "Custom"

        self.cap_emit = {
            PySICapability.__TEST2__: {"on_enter": self.test_on_enter_emit, "on_continuous": self.test_on_continuous_emit, "on_leave": self.test_on_leave_emit}
        }

        self.cap_recv = {
            PySICapability.__TEST1__: {"on_enter": self.test_on_enter_recv, "on_continuous": self.test_on_continuous_recv, "on_leave": self.test_on_leave_recv}
        }

        self.cap_link_emit = {
            "__position__": self.position,
            "__scale__": self.scale
        }

        self.cap_link_recv = {
            "__position__": {"__position__": self.set_position_from_position},
            "__color__": {"__position__": self.set_position_from_color}
        }

    def __repr__(self):
        return ""

    def test_on_enter_emit(self, other):
        i = 5
        k = 3

        return 2 + 5, "WTF"

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

    def position(self):
        return [1, 1]

    def scale(self):
        return [1]

    def set_position_from_position(self, args):
        print("From Position 1", args)
        return 0

    def set_position_from_color(self, args):
        print("From Color 1", args)
        return 0
