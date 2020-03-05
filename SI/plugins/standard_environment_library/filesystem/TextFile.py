from libPySI import PySIEffect, PySICapability
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

        self.cap_recv["OPEN_ENTRY"] = {"on_enter": self.on_open_entry_enter_recv, "on_continuous": None, "on_leave": self.on_open_entry_leave_recv}

        # self.cap_link_emit[CAPABILITY] = {...}
        # self.cap_link_recv[CAPABILITY] = {...}

    def on_open_entry_enter_recv(self):
        return 0

    def on_open_entry_leave_recv(self):
        return 0