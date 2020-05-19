from libPySI import PySI

from plugins.standard_environment_library import SIEffect

region_type = PySI.EffectType.SI_CANVAS
region_name = PySI.EffectName.SI_STD_NAME_CANVAS


class Canvas(SIEffect.SIEffect):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Canvas, self).__init__(shape, uuid, "", kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_CANVAS
        self.region_type = PySI.EffectType.SI_CANVAS
        self.source = "libstdSI"
        self.color = PySI.Color(0, 0, 255, 0)

        self.disable_effect(PySI.CollisionCapability.DELETION, self.RECEPTION)
        self.enable_effect(PySI.CollisionCapability.SKETCH, self.RECEPTION, self.on_sketch_enter_recv, self.on_sketch_continuous_recv, self.on_sketch_leave_recv)
        self.disable_link_reception(PySI.LinkingCapability.POSITION)

    def on_sketch_enter_recv(self, x, y, sender_id):
        return 0

    def on_sketch_continuous_recv(self, x, y, cursor_id):
        self.add_point_to_region_drawing(x, y, cursor_id)

    def on_sketch_leave_recv(self, x, y, cursor_id):
        self.register_region_from_drawing(cursor_id)