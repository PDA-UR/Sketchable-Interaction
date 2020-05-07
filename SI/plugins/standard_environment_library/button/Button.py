from libPySI import PySIEffect
from plugins.standard_environment_library import SIEffect


region_type = PySIEffect.EffectType.SI_BUTTON
region_name = PySIEffect.SI_STD_NAME_BUTTON
region_width = 100
region_height = 100

class Button(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Button, self).__init__(shape, uuid, "res/next.png", kwargs)
        self.name = PySIEffect.SI_STD_NAME_BUTTON
        self.region_type = PySIEffect.EffectType.SI_BUTTON
        self.qml_path = "plugins/standard_environment_library/button/Button.qml"
        self.color = PySIEffect.Color(192, 192, 192, 0)

        self.disable_effect(PySIEffect.MOVE, False)

        self.value = kwargs["value"] if len(kwargs) else True

        self.is_triggered = False
        self.is_triggered_reported = False

        self.icon_width = 100
        self.icon_height = 100
        self.width = region_width
        self.height = region_height

        self.add_QML_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_QML_data("icon_height", self.icon_height, PySIEffect.DataType.INT)

        self.enable_effect(PySIEffect.BTN, self.EMISSION, self.on_btn_enter_emit, self.on_btn_continuous_emit, self.on_btn_leave_emit)
        self.enable_effect(PySIEffect.CLICK, self.RECEPTION, self.on_click_enter_recv, self.on_click_continuous_recv, self.on_click_leave_recv)
        self.enable_effect(PySIEffect.PARENT, self.RECEPTION, self.on_parent_enter_recv, None, self.on_parent_leave_recv)

    def on_btn_enter_emit(self, other):
        return "", ""

    def on_btn_continuous_emit(self, other):
        if self.is_triggered and not self.is_triggered_reported and not other.is_child:
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
        self.create_link(parent_id, PySIEffect.POSITION, self._uuid, PySIEffect.POSITION)

    def on_parent_leave_recv(self, parent_id):
        self.remove_link(parent_id, PySIEffect.POSITION, self._uuid, PySIEffect.POSITION)
