from libPySI import PySIEffect, PySICapability


class Canvas(PySIEffect.PySIEffect):
    def __init__(self):
        super(Canvas, self).__init__()

        self.name = "stdCanvas"
        self.region_type = PySIEffect.EffectType.SI_CANVAS
        self.source = "libstdSI"
        self.qml_path = ""

        self.color = [0, 0, 255, 255]

        self.cap_emit = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}
        }

        self.cap_recv = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}
            "SKETCH": {"on_enter": self.on_sketch_enter_recv, "on_continuous": self.on_sketch_continuous_recv, "on_leave": self.on_sketch_leave_recv}
        }

        self.cap_link_emit = {
            # attr: self.get_function,
        }

        self.cap_link_recv = {
            # "source_attr": {"recv_attr": self.set_function},
        }

    def on_sketch_enter_recv(self, x, y, sender_id):
        return 0

    def on_sketch_continuous_recv(self, x, y, cursor_id):
        if cursor_id not in self.__partial_regions__.keys():
            self.__partial_regions__[cursor_id] = PySIEffect.PointVector()

        self.__partial_regions__[cursor_id].append(PySIEffect.Point3(x, y, 1))

        return 0

    def on_sketch_leave_recv(self, x, y, sender_id):
        self.register_region(sender_id)
        return 0
