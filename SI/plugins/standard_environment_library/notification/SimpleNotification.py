from libPySI import PySIEffect


class SimpleNotification(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(SimpleNotification, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = PySIEffect.SI_STD_NAME_SIMPLE_NOTIFICATION
        self.region_type = PySIEffect.EffectType.SI_NOTIFICATION
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/notification/SimpleNotification.qml"
        self.color = PySIEffect.Color(255, 255, 255, 255)
        self.is_under_user_control = False
        self.message = "Hello World"

        self.width = 800
        self.height = 75

        self.add_data("rect_width", self.width, PySIEffect.DataType.INT)
        self.add_data("rect_height", self.height, PySIEffect.DataType.INT)
        self.add_data("message", self.message, PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap()

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
        })

    def update_message(self, message):
        self.message = message
        self.add_data("message", self.message, PySIEffect.DataType.STRING)

    def on_move_enter_recv(self, cursor_id, link_attrib):
        self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])
        self.is_under_user_control = True

        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        self.is_under_user_control = False

        lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]