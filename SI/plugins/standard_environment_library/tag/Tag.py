from libPySI import PySI

from plugins.standard_environment_library.SIEffect import SIEffect


class Tag(SIEffect):
    regiontype = PySI.EffectType.SI_CUSTOM
    regionname = PySI.EffectName.SI_STD_NAME_TAG
    region_display_name = "Tag"

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Tag, self).__init__(shape, uuid, "res/tag.png", Tag.regiontype, Tag.regionname, kwargs, "libstdSI")
        self.qml_path = "plugins/standard_environment_library/tag/Tag.qml"
        self.color = PySI.Color(255, 0, 0, 255)

        self.enable_effect("tagging", SIEffect.EMISSION, self.on_tag_enter_emit, None, None)

    def on_tag_enter_emit(self, other):
        return True
