from libPySI import PySIEffect
import Entry


class TextFile(Entry.Entry):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(TextFile, self).__init__(shape, aabb, uuid, kwargs)
        self.name = "stdSITextFile"
        self.region_type = PySIEffect.EffectType.SI_TEXT_FILE
        self.qml_path = "plugins/standard_environment_library/filesystem/TextFile.qml"

        self.add_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_data("text_height", self.text_height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/file_icon.png", PySIEffect.DataType.STRING)
        self.add_data("color", self.text_color, PySIEffect.DataType.STRING)
        self.add_data("name", self.filename, PySIEffect.DataType.STRING)

        self.cap_recv[TextFile.OPEN_ENTRY] = {TextFile.ON_ENTER: self.on_open_entry_enter_recv, TextFile.ON_CONTINUOUS: self.on_open_entry_continuous_recv, TextFile.ON_LEAVE: self.on_open_entry_leave_recv}

    def on_open_entry_enter_recv(self):
        return 0

    def on_open_entry_continuous_recv(self):
        if not self.is_child and not self.is_open_entry_capability_blocked and not self.is_under_user_control:
            self.embed_file_standard_appliation_into_context(self._uuid, self.path)
            self.is_open_entry_capability_blocked = True

        return 0

    def on_open_entry_leave_recv(self):
        self.destroy_embedded_window(self._uuid)
        self.is_open_entry_capability_blocked = False

        return 0