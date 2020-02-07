from libPySI import PySIEffect, PySICapability


class Tag(PySIEffect.PySIEffect):
    def __init__(self):
        super(Tag, self).__init__()

        self.name = "Tag"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/tag/Tag.qml"
        self.color = [255, 0, 0, 255]


        self.cap_emit = {
            # <capability>: {"on_enter": self.<emit_enter_function>, "on_continuous": self.<emit_continuous_function>, "on_leave": self.<emit_leave_function>
            # ...
        }

        self.cap_recv = {
            # <capability>: {"on_enter": self.<recv_enter_function>, "on_continuous": self.<recv_continuous_function>, "on_leave": self.<recv_leave_function>
            # ...

            "MOVE": {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv, "on_leave": self.on_move_leave_recv}
        }

        self.cap_link_emit = {
            # <attribute>: self.<get_function>
            # ...
        }

        self.cap_link_recv = {
            "__position__": {"__position__": self.set_position_from_position}
            # <source_attribute>: {"recv_attribute": self.<set_function>},
            # ...
        }

    def set_position_from_position(self, rel_x, rel_y):
        self.x += rel_x
        self.y += rel_y

        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        if link_attrib is not "" and cursor_id is not "":
            self.register_link(cursor_id, link_attrib, self._uuid, link_attrib)
        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        if link_attrib is not "" and cursor_id is not "":
            self.remove_link(cursor_id, link_attrib, self._uuid, link_attrib)
        return 0