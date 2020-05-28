from libPySI import PySI

from plugins.standard_environment_library import SIEffect


class Selector(SIEffect.SIEffect):
    regionname = PySI.EffectName.SI_STD_NAME_SELECTOR
    regiontype = PySI.EffectType.SI_SELECTOR

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Selector, self).__init__(shape, uuid, "", Selector.regiontype, Selector.regionname, kwargs)
        self.source = "libStdSI"
        self.qml_path = "plugins/standard_environment_library/palette/Selector.qml"

        self.color = kwargs["target_color"]
        self.target_display_name = kwargs["target_display_name"]
        self.target_name = kwargs["target_name"]
        self.target_texture_path = kwargs["target_texture"]

        self.disable_effect(PySI.CollisionCapability.DELETION, self.RECEPTION)
        self.disable_effect(PySI.CollisionCapability.MOVE, self.RECEPTION)
        self.enable_effect(PySI.CollisionCapability.ASSIGN, self.EMISSION, None, self.on_assign_continuous_emit, None)

        self.parent = kwargs["parent"]

        self.create_link(self.parent, PySI.LinkingCapability.POSITION, self._uuid, PySI.LinkingCapability.POSITION)

    def on_assign_continuous_emit(self, other):
        return self.target_name, self.target_display_name, {}
