from libPySI import PySIEffect, PySICapability


class TextFile(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(TextFile, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = "stdSITextFile"
        self.region_type = PySIEffect.EffectType.SI_TEXT_FILE
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/filesystem/TextFile.qml"
        self.width = 130
        self.height = 125
        self.icon_width = 65
        self.icon_height = 75
        self.text_height = 50
        self.color = PySIEffect.Color(255, 10, 0, 255)
        self.text_color = "#FFFFFFFF"
        self.path = kwargs["cwd"] if len(kwargs.keys()) else ""
        self.is_child = bool(kwargs["is_child"]) if len(kwargs.keys()) else False
        self.filename = ""
        self.is_open_entry_capability_blocked = False

        if self.path is not "":
            self.filename = self.path[self.path.rfind("/") + 1:]

        self.is_container_visible = True
        self.add_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_data("text_height", self.text_height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/file_icon.png", PySIEffect.DataType.STRING)
        self.add_data("color", self.text_color, PySIEffect.DataType.STRING)
        self.add_data("name", self.filename, PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "OPEN_ENTRY": {"on_enter": self.on_open_entry_enter_recv, "on_continuous": None, "on_leave": self.on_open_entry_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap()

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {"__position__": self.set_position_from_position}
        })

    def set_position_from_position(self, rel_x, rel_y):
        self.x += rel_x
        self.y += rel_y

        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])

        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]

        return 0

    def on_open_entry_enter_recv(self):
        return 0

    def on_open_entry_leave_recv(self):
        return 0