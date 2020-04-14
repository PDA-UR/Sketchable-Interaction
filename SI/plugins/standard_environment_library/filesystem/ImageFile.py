from libPySI import PySIEffect


class ImageFile(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(ImageFile, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = ""
        self.region_type = PySIEffect.EffectType.SI_IMAGE_FILE
        self.source = ""
        self.qml_path = ""

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            ImageFile.MOVE: {ImageFile.ON_ENTER: self.on_move_enter_recv, ImageFile.ON_CONTINUOUS: self.on_move_continuous_recv,
                     ImageFile.ON_LEAVE: self.on_move_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap()

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            ImageFile.POSITION: {ImageFile.POSITION: self.set_position_from_position}
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