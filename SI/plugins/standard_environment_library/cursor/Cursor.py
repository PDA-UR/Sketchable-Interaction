from libPySI import PySIEffect, PySICapability


class MouseCursor(PySIEffect.PySIEffect):
    def __init__(self):
        super(MouseCursor, self).__init__()

        self.name = "MouseCursor"
        self.region_type = PySIEffect.EffectType.SI_MOUSE_CURSOR
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/cursor/Cursor.qml"
        self.color = [0, 255, 255, 255]

        self.width = 18
        self.height = 24

        self.set_data("width", self.width, PySIEffect.DataType.INT)
        self.set_data("height", self.height, PySIEffect.DataType.INT)
        self.set_data("img_path", "res/mouse_cursor.png", PySIEffect.DataType.STRING)

        self.parent_canvas = None
        self.move_target = None

        self.last_x = 0
        self.last_y = 0


        self.cap_emit = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}
        }

        self.cap_recv = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}
        }

        self.cap_link_emit = {
            # attr: self.get_function,
            "__position__": self.position
        }

        self.cap_link_recv = {
            # "source_attr": {"recv_attr": self.set_function},
            "__position__": {"__position__": self.set_position}
        }

    def position(self):
        rel_x = self.x - self.last_x
        rel_y = self.y - self.last_y

        return rel_x, rel_y

    def set_position(self, x, y):
        self.last_x = self.x
        self.last_y = self.y

        self.x = x
        self.y = y

        self.__handle_left_mouse_click()
        self.__handle_right_mouse_click()

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

    def __handle_left_mouse_click(self):
        if self.left_mouse_clicked:
            if "SKETCH" not in self.cap_emit.keys():
                temp = self.cap_emit
                temp["SKETCH"] = {"on_enter": self.self_on_sketch_enter_emit, "on_continuous": self.on_sketch_continuous_emit, "on_leave": self.on_sketch_leave_emit}
                self.cap_emit = temp
        elif "SKETCH" in self.cap_emit.keys():
            temp = self.cap_emit
            del temp["SKETCH"]
            self.cap_emit = temp
            if self.parent_canvas is not None:
                self.parent_canvas.on_sketch_leave_recv(*self.on_sketch_leave_emit(self.parent_canvas))

    def __handle_right_mouse_click(self):
        if self.right_mouse_clicked:
            if "MOVE" not in self.cap_emit.keys():
                temp = self.cap_emit
                temp["MOVE"] = {"on_enter": self.on_move_enter_emit, "on_continuous": self.on_move_continuous_emit, "on_leave": self.on_move_leave_emit}
                self.cap_emit = temp
        elif "MOVE" in self.cap_emit.keys():
            temp = self.cap_emit
            del temp["MOVE"]
            self.cap_emit = temp
            if self.move_target is not None:
                self.move_target.on_move_leave_recv(*self.on_move_leave_emit(self.move_target))

