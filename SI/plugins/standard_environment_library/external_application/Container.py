from libPySI import PySIEffect, PySICapability


class Container(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Container, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = "Container"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "stdContainer"
        self.qml_path = ""
        self.color = PySIEffect.Color(0, 0, 0, 255)
        self.children = []
        self.delta_x = 0
        self.delta_y = 0

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()
        self.cap_emit["PARENT"] = {"on_enter": self.on_child_enter_emit, "on_continuous": None, "on_leave": self.on_child_leave_emit}

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "MOVE": {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv, "on_leave": self.on_move_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap({
            "__position__": self.position
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {
                "__position__": self.set_position_from_position,
                "__scale__": self.set_scale_from_position
            }
        })

    def set_scale_from_position(self, rel_x, rel_y, sender_uuid=""):
        # determine in which quadrant the button is


        print("scale", rel_x, rel_y)

    def on_child_enter_emit(self, child):
        if child not in self.children:
            self.children.append(child)

        self.link_relations.append([child._uuid, "__position__", self._uuid, "__scale__"])

        return self._uuid

    def on_child_leave_emit(self, child):
        # index = self.children.index(child)
        #
        # del self.children[index]
        #
        # lr = PySIEffect.LinkRelation(child._uuid, "__position__", self._uuid, "__scale__")
        #
        # if lr in self.link_relations:
        #     del self.link_relations[self.link_relations.index(lr)]

        return self._uuid,

    def position(self):
        return self.delta_x, self.delta_y

    def set_position_from_position(self, rel_x, rel_y, sender_uuid=""):
        self.x += rel_x
        self.y += rel_y

        self.delta_x = rel_x
        self.delta_y = rel_y

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