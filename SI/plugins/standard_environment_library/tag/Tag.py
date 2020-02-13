from libPySI import PySIEffect, PySICapability


class Tag(PySIEffect.PySIEffect):
    def __init__(self):
        super(Tag, self).__init__()

        self.name = "Tag"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/tag/Tag.qml"
        self.color = PySIEffect.Color(255, 0, 0, 255)

        self.cap_emit = PySIEffect.CollisionEventMap()
        self.cap_recv = PySIEffect.CollisionEventMap()

        self.cap_recv["MOVE"] = PySIEffect.LinkEmissionEventMap()
        self.cap_recv["MOVE"]["on_enter"] = self.on_move_enter_recv
        self.cap_recv["MOVE"]["on_continuous"] = self.on_move_continuous_recv
        self.cap_recv["MOVE"]["on_leave"] = self.on_move_leave_recv

        self.cap_link_recv = PySIEffect.LinkReceptionEventMap()
        self.cap_link_recv["__position__"] = PySIEffect.String2FunctionMap()
        self.cap_link_recv["__position__"]["__position__"] = self.set_position_from_position

    def set_position_from_position(self, rel_x, rel_y):
        self.x += rel_x
        self.y += rel_y

        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        self.register_link(cursor_id, link_attrib, self._uuid, link_attrib)
        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        self.remove_link(cursor_id, link_attrib, self._uuid, link_attrib)
        return 0