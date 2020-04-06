from libPySI import PySIEffect, PySICapability
import Button


class ScaleButton(Button.Button):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(ScaleButton, self).__init__(shape, aabb, uuid, kwargs)
        self.name = "stdScaleButton"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        # self.qml_path = ""

        self.cap_link_emit["__position__"] = self.position
        self.cap_recv["MOVE"] = {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv, "on_leave": self.on_move_leave_recv}

    def position(self):
        return self.x, self.y

    def on_parent_enter_recv(self, parent_id):
        self.link_relations.append([parent_id, "__position__", self._uuid, "__position__"])

        return 0

    def on_parent_leave_recv(self, parent_id):
        lr = PySIEffect.LinkRelation(parent_id, "__position__", self._uuid, "__position__")

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]

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