from libPySI import PySIEffect

from SI.plugins.standard_environment_library import SIEffect


class Entry(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Entry, self).__init__(shape, aabb, uuid, self.TEXTURE_PATH_NONE, kwargs)
        self.name = PySIEffect.SI_STD_NAME_ENTRY
        self.region_type = PySIEffect.EffectType.SI_ENTRY
        self.source = "libstdSI"
        self.qml_path = ""
        self.width = 130
        self.height = 125
        self.icon_width = 65
        self.icon_height = 75
        self.text_height = 50
        self.color = PySIEffect.Color(255, 10, 0, 0)
        self.text_color = "#FFFFFFFF"
        self.path = kwargs["cwd"] if len(kwargs.keys()) else ""
        self.is_child = bool(kwargs["is_child"]) if len(kwargs.keys()) else False
        self.filename = ""
        self.is_visible = True
        self.is_under_user_control = False
        self.is_open_entry_capability_blocked = False

        self.disable_effect(PySIEffect.DELETION, self.RECEPTION)

        if self.path is not "":
            self.filename = self.path[self.path.rfind("/") + 1:]

        self.is_container_visible = True

        self.add_QML_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_QML_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_QML_data("color", self.text_color, PySIEffect.DataType.STRING)
        self.add_QML_data("name", self.filename, PySIEffect.DataType.STRING)

        self.enable_effect(PySIEffect.OPEN_ENTRY, self.RECEPTION, self.on_open_entry_enter_recv, self.on_open_entry_continuous_recv, self.on_open_entry_leave_recv)

        if self.is_child:
            self.enable_effect(PySIEffect.PARENT, self.RECEPTION, self.on_parent_enter_recv, None, self.on_parent_leave_recv)

    def on_parent_enter_recv(self, parent_id):
        self.create_link(parent_id, PySIEffect.POSITION, self._uuid, PySIEffect.POSITION)

    def on_parent_leave_recv(self, parent_id):
        self.is_child = False

        if self.region_type == int(PySIEffect.EffectType.SI_DIRECTORY):
            self.enable_effect(PySIEffect.PARENT, self.EMISSION, self.on_child_enter_emit, None, self.on_child_leave_emit)

        self.remove_link(parent_id, PySIEffect.POSITION, self._uuid, PySIEffect.POSITION)

    def on_open_entry_enter_recv(self, is_other_controlled):
        return 0

    def on_open_entry_continuous_recv(self, is_other_controlled):
        if not self.is_child and not self.is_open_entry_capability_blocked and not self.is_under_user_control and not is_other_controlled:
            self.start_standard_application(self._uuid, self.path)
            self.is_open_entry_capability_blocked = True

        return 0

    def on_open_entry_leave_recv(self, is_other_controlled):
        self.close_standard_application(self._uuid)
        self.is_open_entry_capability_blocked = False

        return 0
