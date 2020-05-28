from libPySI import PySI

from plugins.standard_environment_library import SIEffect


class Dummy2(SIEffect.SIEffect):
    regiontype = PySI.EffectType.SI_CUSTOM
    regionname = "Dummy2"

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Dummy2, self).__init__(shape, uuid, "", Dummy2.regiontype, Dummy2.regionname, kwargs)
        self.shape = shape
        self.aabb
        self._uuid = uuid
        self.source = "testSI"
        self.x = 0
        self.y = 0

        self.cap_emit = PySI.String2_String2FunctionMap_Map({
            "TEST2": {PySI.CollisionEvent.ON_ENTER: self.test_on_enter_emit, PySI.CollisionEvent.ON_CONTINUOUS: self.test_on_continuous_emit, PySI.CollisionEvent.ON_LEAVE: self.test_on_leave_emit}
        })

        self.cap_recv = PySI.String2_String2FunctionMap_Map({
            "TEST1": {PySI.CollisionEvent.ON_ENTER: self.test_on_enter_recv, PySI.CollisionEvent.ON_CONTINUOUS: self.test_on_continuous_recv, PySI.CollisionEvent.ON_LEAVE: self.test_on_leave_recv}
        })

        self.cap_link_emit = PySI.String2FunctionMap({
            PySI.LinkingCapability.POSITION: self.position,
            PySI.LinkingCapability.SCALE: self.scale,
            PySI.LinkingCapability.ROTATION: self.rotation
        })

        self.cap_link_recv = PySI.String2_String2FunctionMap_Map({
            PySI.LinkingCapability.POSITION: {PySI.LinkingCapability.POSITION: self.set_position_from_position},
            PySI.LinkingCapability.COLOR: {PySI.LinkingCapability.POSITION: self.set_position_from_color},
            PySI.LinkingCapability.ROTATION: {PySI.LinkingCapability.ROTATION: self.set_rotation_from_rotation}
        })

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