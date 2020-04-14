from libPySI import PySIEffect


class Button(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Button, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = "stdButton"
        self.region_type = PySIEffect.EffectType.SI_BUTTON
        self.source = "libStdSI"
        self.qml_path = "plugins/standard_environment_library/button/Button.qml"
        self.color = PySIEffect.Color(192, 192, 192, 0)
        self.delta_x = 0
        self.delta_y = 0

        self.value = kwargs["value"] if len(kwargs) else True

        self.is_triggered = False
        self.is_triggered_reported = False

        self.icon_width = 100
        self.icon_height = 100
        self.width = 100
        self.height = 100

        self.add_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/next.png", PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({
        })

        self.cap_emit[Button.BTN] = {Button.ON_ENTER: self.on_btn_enter_emit, Button.ON_CONTINUOUS: self.on_btn_continuous_emit, Button.ON_LEAVE: self.on_btn_leave_emit}

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            Button.CLICK: {Button.ON_ENTER: self.on_click_enter_recv, Button.ON_CONTINUOUS: self.on_click_continuous_recv, Button.ON_LEAVE: self.on_click_leave_recv}
        })

        self.cap_recv[Button.PARENT] = {Button.ON_ENTER: self.on_parent_enter_recv, Button.ON_CONTINUOUS: None, Button.ON_LEAVE: self.on_parent_leave_recv}

        self.cap_link_emit = PySIEffect.String2FunctionMap({
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            Button.POSITION: {Button.POSITION: self.set_position_from_position}
        })

    def set_position_from_position(self, rel_x, rel_y):
        self.x += rel_x
        self.y += rel_y

        self.delta_x = rel_x
        self.delta_y = rel_y

        return 0

    def on_btn_enter_emit(self, other):
        return "", ""

    def on_btn_continuous_emit(self, other):
        if self.is_triggered and not self.is_triggered_reported:
            self.is_triggered_reported = True
            return self._uuid, self.value

        return "", ""

    def on_btn_leave_emit(self, other):
        return "", ""

    def on_click_enter_recv(self):
        self.is_triggered = False
        self.is_triggered_reported = False

        return 0

    def on_click_continuous_recv(self):
        self.is_triggered = True
        return 0

    def on_click_leave_recv(self):
        self.is_triggered = False
        return 0

    def on_parent_enter_recv(self, parent_id):
        self.link_relations.append([parent_id, Button.POSITION, self._uuid, Button.POSITION])

        return 0

    def on_parent_leave_recv(self, parent_id):
        lr = PySIEffect.LinkRelation(parent_id, Button.POSITION, self._uuid, Button.POSITION)

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]

        return 0
