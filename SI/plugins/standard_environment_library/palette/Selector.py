from libPySI import PySIEffect

from plugins.standard_environment_library import SIEffect

region_name = PySIEffect.SI_STD_NAME_SELECTOR
region_type = PySIEffect.EffectType.SI_SELECTOR


class Selector(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Selector, self).__init__(shape, uuid, "", kwargs)
        self.name = PySIEffect.SI_STD_NAME_SELECTOR
        self.region_type = PySIEffect.EffectType.SI_SELECTOR
        self.source = "libStdSI"
        self.qml_path = "plugins/standard_environment_library/palette/Selector.qml"

        self.color = kwargs["target_color"]
        self.target_display_name = kwargs["target_display_name"]
        self.target_name = kwargs["target_name"]
        self.target_texture_path = kwargs["target_texture"]

        self.disable_effect(PySIEffect.DELETION, self.RECEPTION)
        self.disable_effect(PySIEffect.MOVE, self.RECEPTION)
        self.enable_effect(PySIEffect.ASSIGN, self.EMISSION, None, self.on_assign_continuous_emit, None)

    def on_assign_continuous_emit(self, other):
        return self.target_name, self.target_display_name, {}
