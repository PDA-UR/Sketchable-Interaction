from libPySI import PySI

from plugins.standard_environment_library import SIEffect


region_type = PySI.EffectType.SI_CUSTOM
region_name = PySI.EffectName.SI_STD_NAME_TAG
region_display_name = "Tag"

class Tag(SIEffect.SIEffect):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Tag, self).__init__(shape, uuid, "res/tag.png", kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_TAG
        self.region_type = PySI.EffectType.SI_CUSTOM
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/tag/Tag.qml"
        self.color = PySI.Color(255, 0, 0, 255)
