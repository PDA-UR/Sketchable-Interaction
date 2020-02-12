from libPySI import PySIEffect, PySICapability


class Dummy2(PySIEffect.PySIEffect):
    def __init__(self):
        super(Dummy2, self).__init__()

        self.name = "Dummy2"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "testSI"
        self.texture_path = ""
        self.x = 0
        self.y = 0

        self.cap_emit = PySIEffect.CollisionEventMap()
        self.cap_recv = PySIEffect.CollisionEventMap()

        self.cap_emit[PySICapability.__TEST2__] = PySIEffect.String2FunctionMap()
        self.cap_emit[PySICapability.__TEST2__]["on_enter"] = self.test_on_enter_emit
        self.cap_emit[PySICapability.__TEST2__]["on_continuous"] = self.test_on_continuous_emit
        self.cap_emit[PySICapability.__TEST2__]["on_leave"] = self.test_on_leave_emit

        self.cap_recv[PySICapability.__TEST1__] = PySIEffect.String2FunctionMap()
        self.cap_recv[PySICapability.__TEST1__]["on_enter"] = self.test_on_enter_recv
        self.cap_recv[PySICapability.__TEST1__]["on_continuous"] = self.test_on_continuous_recv
        self.cap_recv[PySICapability.__TEST1__]["on_leave"] = self.test_on_leave_recv

        self.cap_link_emit = {
            "__position__": self.position,
            "__scale__": self.scale,
            "__rotation__": self.rotation
        }

        self.cap_link_recv = {
            "__position__": {"__position__": self.set_position_from_position},
            "__color__": {"__position__": self.set_position_from_color},
            "__rotation__": {"__rotation__": self.set_rotation_from_rotation}
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
        return 2, 2

    def scale(self):
        return 1

    def rotation(self):
        return 0, 0, 0

    def set_position_from_position(self, x, y):
        print("Dummy2", x, y)
        return 0

    def set_position_from_color(self, r, g, b, a):
        print("Dummy2", r, g, b, a)
        return 0

    def set_rotation_from_rotation(self, x, y, z):
        print("Dummy2", x, y, z)
        return 0

    def set_rotation_from_position(self, x, y):
        print("Dummy2", x, y)
        return 0