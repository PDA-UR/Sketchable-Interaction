from libPySI import PySI
from plugins.standard_environment_library.filesystem import Entry


region_type = PySI.EffectType.SI_TEXT_FILE
region_name = PySI.EffectName.SI_STD_NAME_TEXTFILE


class TextFile(Entry.Entry):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(TextFile, self).__init__(shape, uuid, kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_TEXTFILE
        self.region_type = PySI.EffectType.SI_TEXT_FILE
        self.qml_path = "plugins/standard_environment_library/filesystem/TextFile.qml"

        self.add_QML_data("text_height", self.text_height, PySI.DataType.INT)
        self.add_QML_data("img_path", "res/file_icon.png", PySI.DataType.STRING)
