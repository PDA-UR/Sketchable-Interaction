from libPySI import PySIEffect, PySICapability


class MouseCursor(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(MouseCursor, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = "MouseCursor"
        self.region_type = PySIEffect.EffectType.SI_MOUSE_CURSOR
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/cursor/Cursor.qml"
        self.color = PySIEffect.Color(0, 255, 255, 255)

        self.width = 18
        self.height = 24
        self.last_x = 0
        self.last_y = 0

        self.add_data("width", self.width, PySIEffect.DataType.INT)
        self.add_data("height", self.height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/mouse_cursor.png", PySIEffect.DataType.STRING)

        self.parent_canvas = None
        self.move_target = None

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()
        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_link_emit = PySIEffect.String2FunctionMap({
            "__position__": self.position
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {"__position__": self.set_position}
        })

    def position(self):
        rel_x = self.x - self.last_x
        rel_y = self.y - self.last_y

        return rel_x, rel_y

    def set_position(self, x, y):
        self.last_x = self.x
        self.last_y = self.y

        self.x = x
        self.y = y

        return 0

    def self_on_sketch_enter_emit(self, other):
        self.parent_canvas = other
        return 0, 0, self._uuid

    def on_sketch_continuous_emit(self, other):
        return self.x, self.y, self._uuid

    def on_sketch_leave_emit(self, other):
        self.parent_canvas = None
        return 0, 0, self._uuid

    def on_move_enter_emit(self, other):
        if self.move_target is None:
            self.move_target = other

        if self.move_target is other:
            return self._uuid, "__position__"

        return "", ""

    def on_move_continuous_emit(self, other):
        pass

    def on_move_leave_emit(self, other):
        if self.move_target is other:
            self.move_target = None
            return self._uuid, "__position__"

        return "", ""

    def on_left_mouse_click(self, is_active):
        if is_active:
            if "SKETCH" not in self.cap_emit.keys():
                self.cap_emit["SKETCH"] = {"on_enter": self.self_on_sketch_enter_emit, "on_continuous": self.on_sketch_continuous_emit, "on_leave": self.on_sketch_leave_emit}
        else:
            if "SKETCH" in self.cap_emit.keys():
                del self.cap_emit["SKETCH"]

                if self.parent_canvas is not None:
                    self.parent_canvas.on_sketch_leave_recv(*self.on_sketch_leave_emit(self.parent_canvas))

    def on_right_mouse_click(self, is_active):
        if self.right_mouse_clicked:
            if "MOVE" not in self.cap_emit.keys():
                self.cap_emit["MOVE"] = {"on_enter": self.on_move_enter_emit, "on_continuous": self.on_move_continuous_emit, "on_leave": self.on_move_leave_emit}
        elif "MOVE" in self.cap_emit.keys():
            del self.cap_emit["MOVE"]
            if self.move_target is not None:
                self.move_target.on_move_leave_recv(*self.on_move_leave_emit(self.move_target))

