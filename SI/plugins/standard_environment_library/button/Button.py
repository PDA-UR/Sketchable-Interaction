from libPySI import PySIEffect, PySICapability


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
        self.color = PySIEffect.Color(192, 192, 192, 255)

        self.value = kwargs["value"] if len(kwargs) else True

        self.is_triggered = False

        self.icon_width = 100
        self.icon_height = 100
        self.width = 100
        self.height = 100

        self.add_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/next.png", PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({
            "BTN_PRESS": {"on_enter": self.on_btn_press_enter_emit, "on_continuous": self.on_btn_press_continuous_emit, "on_leave": self.on_btn_press_leave_emit}
        })

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "CLICK": {"on_enter": self.on_click_enter_recv, "on_continuous": self.on_click_continuous_recv, "on_leave": self.on_click_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap({
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {"__position__": self.set_position_from_position}
        })

    def set_position_from_position(self, rel_x, rel_y):
        self.x += rel_x
        self.y += rel_y

        return 0

    def trigger(self):
        return self.is_triggered

    def on_btn_press_enter_emit(self, other):
        return self.is_triggered, self.value

    def on_btn_press_continuous_emit(self, other):
        return self.is_triggered, self.value

    def on_btn_press_leave_emit(self, other):
        return self.is_triggered, self.value

    def on_click_enter_recv(self):
        self.is_triggered = True
        return 0

    def on_click_continuous_recv(self):
        return 0

    def on_click_leave_recv(self, _):
        self.is_triggered = False
        return 0
