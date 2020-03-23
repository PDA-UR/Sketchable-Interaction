from libPySI import PySIEffect, PySICapability


class Deletion(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Deletion, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = "stdDeletion"
        self.region_type = PySIEffect.EffectType.SI_DELETION
        self.qml_path = "plugins/standard_environment_library/deletion/Deletion.qml"
        self.color = PySIEffect.Color(255, 255, 0, 255)
        self.is_under_user_control = False

        self.add_data("img_width", 75, PySIEffect.DataType.INT)
        self.add_data("img_height", 75, PySIEffect.DataType.INT)

        if len(self.aabb):
            self.add_data("widget_width", self.aabb[3].x - self.aabb[0].x, PySIEffect.DataType.FLOAT)
            self.add_data("widget_height", self.aabb[1].y - self.aabb[0].y, PySIEffect.DataType.FLOAT)
        self.add_data("img_path", "res/deletion.png", PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({
            "DELETION": {"on_enter": self.on_deletion_enter_emit, "on_continuous": self.on_deletion_continuous_emit, "on_leave": self.on_deletion_enter_emit}
        })

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "MOVE": {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv, "on_leave": self.on_move_leave_recv},
            "DELETION": {"on_enter": None, "on_continuous": None, "on_leave": None}
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {"__position__": self.set_position_from_position}
        })

        self.link_relations = PySIEffect.LinkRelationVector()

    def set_position_from_position(self, rel_x, rel_y):
        self.x += rel_x
        self.y += rel_y

        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        if not cursor_id is "" and not link_attrib is "":
            self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])
            self.is_under_user_control = True

        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        if not cursor_id is "" and not link_attrib is "":
            lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)
            if lr in self.link_relations:
                del self.link_relations[self.link_relations.index(lr)]

            self.is_under_user_control = False

        return 0

    def on_deletion_enter_emit(self, other):
        if other.region_type is int(PySIEffect.EffectType.SI_DELETION):
            if self.is_under_user_control:
                other.signal_deletion()

    def on_deletion_continuous_emit(self, other):
        if other.region_type is not int(PySIEffect.EffectType.SI_DELETION):
            if not other.is_under_user_control:
                other.signal_deletion()
