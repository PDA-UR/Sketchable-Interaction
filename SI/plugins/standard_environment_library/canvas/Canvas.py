from libPySI import PySIEffect, PySICapability


class Canvas(PySIEffect.PySIEffect):
    def __init__(self):
        super(Canvas, self).__init__()

        self.name = "stdCanvas"
        self.region_type = PySIEffect.EffectType.SI_CANVAS
        self.source = "libstdSI"
        self.qml_path = ""
        self.color = PySIEffect.Color(0, 0, 255, 255)
        self.registered_regions = PySIEffect.StringVector()

        self.cap_emit = PySIEffect.CollisionEventMap()

        self.cap_recv = PySIEffect.CollisionEventMap()
        self.cap_recv["SKETCH"] = {"on_enter": self.on_sketch_enter_recv, "on_continuous": self.on_sketch_continuous_recv, "on_leave": self.on_sketch_leave_recv}

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
