from libPySI import PySIEffect
from plugins.standard_environment_library.filesystem import Entry


region_type = PySIEffect.EffectType.SI_TEXT_FILE
region_name = PySIEffect.SI_STD_NAME_TEXTFILE


class TextFile(Entry.Entry):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(TextFile, self).__init__(shape, uuid, kwargs)
        self.name = PySIEffect.SI_STD_NAME_TEXTFILE
        self.region_type = PySIEffect.EffectType.SI_TEXT_FILE
        self.qml_path = "plugins/standard_environment_library/filesystem/TextFile.qml"

        self.add_QML_data("text_height", self.text_height, PySIEffect.DataType.INT)
        self.add_QML_data("img_path", "res/file_icon.png", PySIEffect.DataType.STRING)
