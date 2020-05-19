from libPySI import PySI
from plugins.standard_environment_library import SIEffect


region_type = PySI.EffectType.SI_BUTTON
region_name = PySI.EffectName.SI_STD_NAME_BUTTON
region_width = 100
region_height = 100


class Button(SIEffect.SIEffect):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Button, self).__init__(shape, uuid, "res/next.png", kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_BUTTON
        self.region_type = PySI.EffectType.SI_BUTTON
        self.qml_path = "plugins/standard_environment_library/button/Button.qml"
        self.color = PySI.Color(192, 192, 192, 0)

        self.disable_effect(PySI.CollisionCapability.MOVE, False)

        self.value = kwargs["value"] if len(kwargs.keys()) else False
        self.parent = str(kwargs["parent"]) if len(kwargs.keys()) else ""

        self.is_triggered = False
        self.is_triggered_reported = False

        self.icon_width = 100
        self.icon_height = 100
        self.width = region_width
        self.height = region_height

        self.add_QML_data("icon_width", self.icon_width, PySI.DataType.INT)
        self.add_QML_data("icon_height", self.icon_height, PySI.DataType.INT)

        self.enable_effect(PySI.CollisionCapability.BTN, self.EMISSION, self.on_btn_enter_emit, self.on_btn_continuous_emit, self.on_btn_leave_emit)
        self.enable_effect(PySI.CollisionCapability.CLICK, self.RECEPTION, self.on_click_enter_recv, self.on_click_continuous_recv, self.on_click_leave_recv)
        self.enable_effect(PySI.CollisionCapability.PARENT, self.RECEPTION, self.on_parent_enter_recv, None, self.on_parent_leave_recv)
        self.parent = ""
        self.is_open_entry_capability_blocked = False

    def on_btn_enter_emit(self, other):
        return "", ""

    def on_btn_continuous_emit(self, other):
        if self.is_triggered and not self.is_triggered_reported and other.is_open_entry_capability_blocked:
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
        if self.parent == "":
            self.parent = parent_id
            self.create_link(parent_id, PySI.LinkingCapability.POSITION, self._uuid, PySI.LinkingCapability.POSITION)

    def on_parent_leave_recv(self, parent_id):
        if self.parent == parent_id:
            self.remove_link(parent_id, PySI.LinkingCapability.POSITION, self._uuid, PySI.LinkingCapability.POSITION)
