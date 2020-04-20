from libPySI import PySIEffect


class Dummy2(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Dummy2, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = "Dummy2"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "testSI"
        self.texture_path = ""
        self.x = 0
        self.y = 0

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({
            "TEST2": {PySIEffect.ON_ENTER: self.test_on_enter_emit, PySIEffect.ON_CONTINUOUS: self.test_on_continuous_emit, PySIEffect.ON_LEAVE: self.test_on_leave_emit}
        })

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "TEST1": {PySIEffect.ON_ENTER: self.test_on_enter_recv, PySIEffect.ON_CONTINUOUS: self.test_on_continuous_recv, PySIEffect.ON_LEAVE: self.test_on_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap({
            PySIEffect.POSITION: self.position,
            PySIEffect.SCALE: self.scale,
            PySIEffect.ROTATION: self.rotation
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            PySIEffect.POSITION: {PySIEffect.POSITION: self.set_position_from_position},
            PySIEffect.COLOR: {PySIEffect.POSITION: self.set_position_from_color},
            PySIEffect.ROTATION: {PySIEffect.ROTATION: self.set_rotation_from_rotation}
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