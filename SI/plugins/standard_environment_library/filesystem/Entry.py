from libPySI import PySIEffect


class Entry(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Entry, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = PySIEffect.SI_STD_NAME_ENTRY
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
            PySIEffect.MOVE: {PySIEffect.ON_ENTER: self.on_move_enter_recv, PySIEffect.ON_CONTINUOUS: self.on_move_continuous_recv, PySIEffect.ON_LEAVE: self.on_move_leave_recv}
        })

        if self.is_child:
            self.cap_recv[PySIEffect.PARENT] = {PySIEffect.ON_ENTER: self.on_parent_enter_recv, PySIEffect.ON_CONTINUOUS: None, PySIEffect.ON_LEAVE: self.on_parent_leave_recv}

        self.cap_link_emit = PySIEffect.String2FunctionMap()

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            PySIEffect.POSITION: {PySIEffect.POSITION: self.set_position_from_position}
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
        self.link_relations.append([parent_id, PySIEffect.POSITION, self._uuid, PySIEffect.POSITION])

        return 0

    def on_parent_leave_recv(self, parent_id):
        self.is_child = False

        if self.region_type == int(PySIEffect.EffectType.SI_DIRECTORY):
            self.cap_emit[PySIEffect.PARENT] = {PySIEffect.ON_ENTER: self.on_child_enter_emit, PySIEffect.ON_CONTINUOUS: None, PySIEffect.ON_LEAVE: self.on_child_leave_emit}

        lr = PySIEffect.LinkRelation(parent_id, PySIEffect.POSITION, self._uuid, PySIEffect.POSITION)

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]

        return 0
