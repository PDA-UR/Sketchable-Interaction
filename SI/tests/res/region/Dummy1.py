from libPySI import PySIEffect, PySICapability


class Dummy1(PySIEffect):
    def __init__(self):
        super(Dummy1, self).__init__()

        self.name = "Dummy1"
        self.region_type = "Custom"


        self.cap_emit = {
            PySICapability.__TEST1__: {"on_enter": self.test_on_enter_emit, "on_continuous": self.test_on_continuous_emit, "on_leave": self.test_on_leave_emit}
        }

        self.cap_recv = {
            PySICapability.__TEST2__: {"on_enter": self.test_on_enter_recv, "on_continuous": self.test_on_continuous_recv, "on_leave": self.test_on_leave_recv}
        }

        self.cap_link_emit = {
            "__position__": self.position,
            "__rotation__": self.rotation
        }

        self.cap_link_recv = {
            "__position__": {"__position__": self.set_position_from_position},
            "__color__": {"__position__": self.set_position_from_color},
            "__scale__": {"__position__": self.set_position_from_scale},
            "__rotation__": {"__rotation__": self.set_rotation_from_rotation}
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

    def position(self):
        return [1, 1]

    def rotation(self):
        return [0, 0, 0]

    def set_position_from_position(self, args):
        print("Dummy1", args)
        return 0

    def set_position_from_color(self, args):
        print("Dummy1", args)
        return 0

    def set_position_from_scale(self, args):
        print("Dummy1", args)
        return 0

    def set_rotation_from_rotation(self, args):
        print("Dummy1", args)
        return 0

    def set_position_from_rotation(self, args):
        print("Dummy1", args)
        return 0