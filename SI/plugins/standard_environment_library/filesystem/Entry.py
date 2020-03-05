from libPySI import PySIEffect, PySICapability


class Entry(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Entry, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = ""
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "libstdSI"
        self.qml_path = ""
        self.width = 130
        self.height = 125
        self.icon_width = 65
        self.icon_height = 75
        self.text_height = 50
        self.color = PySIEffect.Color(255, 10, 0, 0)
        self.text_color = "#FFFFFFFF"
        self.path = kwargs["cwd"] if len(kwargs.keys()) else ""
        self.is_child = bool(kwargs["is_child"]) if len(kwargs.keys()) else False
        self.filename = ""
        self.is_visible = True
        self.is_under_user_control = False
        self.is_open_entry_capability_blocked = False
        self.last_x = 0
        self.last_y = 0

        if self.path is not "":
            self.filename = self.path[self.path.rfind("/") + 1:]
        self.is_container_visible = True

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "MOVE": {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv, "on_leave": self.on_move_leave_recv}
        })

        if self.is_child:
            self.cap_recv["PARENT"] = {"on_enter": self.on_parent_enter_recv, "on_continuous": None, "on_leave": self.on_parent_leave_recv}

        self.cap_link_emit = PySIEffect.String2FunctionMap()

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {"__position__": self.set_position_from_position}
        })

    def set_position_from_position(self, rel_x, rel_y):
        self.last_x = self.x
        self.last_y = self.y

        self.x += rel_x
        self.y += rel_y
        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        if cursor_id is not "" and link_attrib is not "":
            self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])
            self.is_under_user_control = True

        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        if cursor_id is not "" and link_attrib is not "":
            self.is_under_user_control = False

            lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

            if lr in self.link_relations:
                del self.link_relations[self.link_relations.index(lr)]

        return 0

    def on_parent_enter_recv(self, parent_id):
        self.link_relations.append([parent_id, "__position__", self._uuid, "__position__"])

        return 0

    def on_parent_leave_recv(self, parent_id):
        lr = PySIEffect.LinkRelation(parent_id, "__position__", self._uuid, "__position__")

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]

        return 0
