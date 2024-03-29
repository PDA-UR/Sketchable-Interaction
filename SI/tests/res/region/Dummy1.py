from libPySI import PySI

from plugins.standard_environment_library.SIEffect import SIEffect


class Dummy1(SIEffect):
    regiontype = PySI.EffectType.SI_CUSTOM
    regionname = "Dummy1"

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super().__init__(shape, uuid, "", Dummy1.regiontype, Dummy1.regionname, kwargs)
        # self.source = "testSI"
        # self.x = 0
        # self.y = 0


        self.cap_emit = PySI.String2String2FunctionMapMap({
            "TEST1": {PySI.CollisionEvent.ON_ENTER: self.test_on_enter_emit, PySI.CollisionEvent.ON_CONTINUOUS: self.test_on_continuous_emit, PySI.CollisionEvent.ON_LEAVE: self.test_on_leave_emit}
        })

        self.cap_recv = PySI.String2String2FunctionMapMap({
            "TEST2": {PySI.CollisionEvent.ON_ENTER: self.test_on_enter_recv, PySI.CollisionEvent.ON_CONTINUOUS: self.test_on_continuous_recv, PySI.CollisionEvent.ON_LEAVE: self.test_on_leave_recv}
        })

        self.cap_link_emit = PySI.String2FunctionMap({
            PySI.LinkingCapability.POSITION: self.position,
            PySI.LinkingCapability.ROTATION: self.rotation
        })

        self.cap_link_recv = PySI.String2String2FunctionMapMap({
            PySI.LinkingCapability.POSITION: {PySI.LinkingCapability.POSITION: self.set_position_from_position},
            PySI.LinkingCapability.COLOR: {PySI.LinkingCapability.POSITION: self.set_position_from_color},
            PySI.LinkingCapability.SCALE: {PySI.LinkingCapability.POSITION: self.set_position_from_scale},
            PySI.LinkingCapability.ROTATION: {PySI.LinkingCapability.ROTATION: self.set_rotation_from_rotation}
        })

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
        return 1, 1

    def rotation(self):
        return 0, 0, 0


    def set_position_from_position(self, x, y):
        print("Dummy1", x, y)
        return 0

    def set_position_from_color(self, r, g, b, a):
        print("Dummy1", r, g, b, a)
        return 0

    def set_position_from_scale(self, f):
        print("Dummy1", f)
        return 0

    def set_rotation_from_rotation(self, x, y, z):
        print("Dummy1", x, y, z)
        return 0

    def set_position_from_rotation(self, args):
        print("Dummy1", args)
        return 0