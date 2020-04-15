from libPySI import PySIEffect


class Container(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Container, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = PySIEffect.SI_STD_NAME_CONTAINER
        self.region_type = PySIEffect.EffectType.SI_EXTERNAL_APPLICATION_CONTAINER
        self.source = "libStdSI"
        self.qml_path = ""
        self.color = PySIEffect.Color(0, 0, 255, 255)

        if len(self.aabb):
            self.last_width = self.aabb[0].x + self.aabb[3].x
            self.last_height = self.aabb[0].y + self.aabb[1].y

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()
        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_link_emit = PySIEffect.String2FunctionMap()

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            PySIEffect.GEOMETRY: {PySIEffect.GEOMETRY: self.set_geometry_from_geometry}
        })

    def set_geometry_from_geometry(self, x, y, width, height):
        if self.last_width == width and self.last_height == height:
            self.x = x
            self.y = y

            self.last_width = width
            self.last_height = height
        else:
            self.shape = PySIEffect.PointVector([[x, y], [x, y + height], [x + width, y + height], [x + width, y]])
            self.notify_shape_changed(True)
            self.has_data_changed = True
            self.x = x
            self.y = y
            self.last_width = width
            self.last_height = height
