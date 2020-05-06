from libPySI import PySIEffect

from plugins.standard_environment_library import SIEffect


class Tag(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Tag, self).__init__(shape, uuid, "res/tag.png", kwargs)
        self.name = PySIEffect.SI_STD_NAME_TAG
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/tag/Tag.qml"
        self.color = PySIEffect.Color(255, 0, 0, 255)
