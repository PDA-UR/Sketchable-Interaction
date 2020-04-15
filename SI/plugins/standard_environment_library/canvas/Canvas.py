from libPySI import PySIEffect


class Canvas(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Canvas, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = PySIEffect.SI_STD_NAME_CANVAS
        self.region_type = PySIEffect.EffectType.SI_CANVAS
        self.source = "libstdSI"
        self.qml_path = ""
        self.color = PySIEffect.Color(0, 0, 255, 0)
        self.registered_regions = PySIEffect.StringVector()

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            PySIEffect.SKETCH: {PySIEffect.ON_ENTER: self.on_sketch_enter_recv, PySIEffect.ON_CONTINUOUS: self.on_sketch_continuous_recv,
                                PySIEffect.ON_LEAVE: self.on_sketch_leave_recv}
        })

    def on_sketch_enter_recv(self, x, y, sender_id):
        return 0

    def on_sketch_continuous_recv(self, x, y, cursor_id):
        if cursor_id not in self.__partial_regions__.keys():
            self.__partial_regions__[cursor_id] = PySIEffect.PointVector()

        self.__partial_regions__[cursor_id].append([x, y])

        return 0

    def on_sketch_leave_recv(self, x, y, sender_id):
        self.registered_regions.append(sender_id)

        return 0
