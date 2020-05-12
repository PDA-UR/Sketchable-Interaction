from libPySI import PySIEffect

from plugins.standard_environment_library import SIEffect

region_type = PySIEffect.EffectType.SI_CANVAS
region_name = PySIEffect.SI_STD_NAME_CANVAS


class Canvas(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Canvas, self).__init__(shape, uuid, "", kwargs)
        # print(self.aabb)
        self.name = PySIEffect.SI_STD_NAME_CANVAS
        self.region_type = PySIEffect.EffectType.SI_CANVAS
        self.source = "libstdSI"
        self.color = PySIEffect.Color(0, 0, 255, 0)

        self.disable_effect(PySIEffect.DELETION, self.RECEPTION)
        self.enable_effect(PySIEffect.SKETCH, self.RECEPTION, self.on_sketch_enter_recv, self.on_sketch_continuous_recv, self.on_sketch_leave_recv)
        self.disable_link_reception(PySIEffect.POSITION)

    def on_sketch_enter_recv(self, x, y, sender_id):
        return 0

    def on_sketch_continuous_recv(self, x, y, cursor_id):
        self.add_point_to_region_drawing(x, y, cursor_id)

    def on_sketch_leave_recv(self, x, y, cursor_id):
        self.register_region_from_drawing(cursor_id)