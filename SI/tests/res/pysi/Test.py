from libPySI import PySI


class test(PySI.Effect):
    def __init__(self, shape=PySI.PointVector(), aabb=PySI.PointVector(), uuid="", kwargs={}):
        super(test, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = ""
        self.region_type = PySI.EffectType.SI_CUSTOM
        self.source = ""
        self.qml_path = ""

        self.cap_emit = PySI.String2_String2FunctionMap_Map()

        self.cap_recv = PySI.String2_String2FunctionMap_Map({
            "MOVE": {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv,
                     "on_leave": self.on_move_leave_recv}
        })

        self.cap_link_emit = PySI.String2FunctionMap()

        self.cap_link_recv = PySI.String2_String2FunctionMap_Map({
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
        lr = PySI.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]