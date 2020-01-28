from libPySI import PySIEffect, PySICapability


class MouseCursor(PySIEffect.PySIEffect):
    def __init__(self):
        super(MouseCursor, self).__init__()

        print(self._uuid)

        self.name = "MouseCursor"
        self.region_type = PySIEffect.EffectType.SI_MOUSE_CURSOR
        self.source = "libstdSI"
        self.texture_path = ""

        self.cap_emit = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}
        }

        self.cap_recv = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}
        }

        self.cap_link_emit = {
            # attr: self.get_function,
        }

        self.cap_link_recv = {
            # "source_attr": {"recv_attr": self.set_function},
            "__position__": {"__position__": self.set_position}
        }

    def set_position(self, x, y):
        self.x = x
        self.y = y

        self.__handle_left_mouse_click()

        return 0

    def self_on_sketch_enter_emit(self, other):
        return 0, 0, self._uuid

    def on_sketch_continuous_emit(self, other):
        return self.x, self.y, self._uuid

    def on_sketch_leave_emit(self, other):
        return 0, 0, self._uuid

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
