from libPySI import PySIEffect

from SI.plugins.standard_environment_library import SIEffect


class OpenEntry(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(OpenEntry, self).__init__(shape, uuid, "res/open_entry.png", kwargs)
        self.name = PySIEffect.SI_STD_NAME_OPEN_ENTRY
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.qml_path = "plugins/standard_environment_library/filesystem/OpenEntry.qml"
        self.color = PySIEffect.Color(0, 0, 255, 255)

        self.enable_effect(PySIEffect.OPEN_ENTRY, self.EMISSION, self.on_open_entry_emit, self.on_open_entry_emit, self.on_open_entry_emit)

    def on_open_entry_emit(self, other):
        return self.is_under_user_control
