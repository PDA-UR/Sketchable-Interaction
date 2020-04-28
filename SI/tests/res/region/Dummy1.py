from libPySI import PySIEffect


class Dummy1(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Dummy1, self).__init__()
        self.shape = shape
        self.aabb
        self._uuid = uuid
        self.name = "Dummy1"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "testSI"
        self.texture_path = ""
        self.x = 0
        self.y = 0

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({
            "TEST1": {PySIEffect.ON_ENTER: self.test_on_enter_emit, PySIEffect.ON_CONTINUOUS: self.test_on_continuous_emit, PySIEffect.ON_LEAVE: self.test_on_leave_emit}
        })

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "TEST2": {PySIEffect.ON_ENTER: self.test_on_enter_recv, PySIEffect.ON_CONTINUOUS: self.test_on_continuous_recv, PySIEffect.ON_LEAVE: self.test_on_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap({
            PySIEffect.POSITION: self.position,
            PySIEffect.ROTATION: self.rotation
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            PySIEffect.POSITION: {PySIEffect.POSITION: self.set_position_from_position},
            PySIEffect.COLOR: {PySIEffect.POSITION: self.set_position_from_color},
            PySIEffect.SCALE: {PySIEffect.POSITION: self.set_position_from_scale},
            PySIEffect.ROTATION: {PySIEffect.ROTATION: self.set_rotation_from_rotation}
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