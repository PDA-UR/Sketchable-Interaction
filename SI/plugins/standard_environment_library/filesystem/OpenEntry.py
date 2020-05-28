from libPySI import PySI

from plugins.standard_environment_library import SIEffect


class OpenEntry(SIEffect.SIEffect):
    regiontype = PySI.EffectType.SI_CUSTOM
    regionname = PySI.EffectName.SI_STD_NAME_OPEN_ENTRY
    region_display_name = "Open Folder/File"

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(OpenEntry, self).__init__(shape, uuid, "res/open_entry.png", OpenEntry.regiontype, OpenEntry.regionname, kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_OPEN_ENTRY
        self.region_type = PySI.EffectType.SI_CUSTOM
        self.qml_path = "plugins/standard_environment_library/filesystem/OpenEntry.qml"
        self.color = PySI.Color(0, 0, 255, 255)

        self.enable_effect(PySI.CollisionCapability.OPEN_ENTRY, self.EMISSION, self.on_open_entry_emit, self.on_open_entry_emit, self.on_open_entry_emit)

    def on_open_entry_emit(self, other):
        return self.is_under_user_control
