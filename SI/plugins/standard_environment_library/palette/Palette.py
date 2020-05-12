from libPySI import PySIEffect
from plugins.standard_environment_library import SIEffect


region_type = PySIEffect.EffectType.SI_PALETTE
region_name = PySIEffect.SI_STD_NAME_PALETTE


class Palette(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Palette, self).__init__(shape=shape, uuid=uuid, texture_path="", kwargs=kwargs)
        self.name = PySIEffect.SI_STD_NAME_PALETTE
        self.region_type = PySIEffect.EffectType.SI_PALETTE
        self.source = "libStdSI"
        self.qml_path = ""
        available_plugins = self.available_plugins()

        print(available_plugins)