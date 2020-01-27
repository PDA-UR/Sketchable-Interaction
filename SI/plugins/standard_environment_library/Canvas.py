from libPySI import PySIEffect, PySICapability


class Canvas(PySIEffect.PySIEffect):
    def __init__(self):
        super(Canvas, self).__init__()

        self.partial_region = []

        self.name = "stdCanvas"
        self.region_type = PySIEffect.EffectType.SI_CANVAS
        self.source = "libstdSI"
        self.texture_path = ""

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

    def on_sketch_enter_recv(self, x, y):
        self.partial_region = []

        return 0

    def on_sketch_continuous_recv(self, x, y):
        self.partial_region.append([x, y])
        self.partial_regions = [self.partial_region]

        return 0

    def on_sketch_leave_recv(self, other):
        return 0
