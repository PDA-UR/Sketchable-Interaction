from libPySI import PySIEffect
from plugins.standard_environment_library import SIEffect


class Palette(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Palette, self).__init__(shape=shape, uuid=uuid, texture_path="", kwargs=kwargs)
        self.name = PySIEffect.SI_STD_NAME_PALETTE
        self.region_type = PySIEffect.EffectType.SI_PALETTE
        self.source = "libStdSI"
        self.qml_path = ""
        available_plugins = list(self.available_plugins())

        # if len(self.aabb):
        #     self.width = int(self.aabb[3].x - self.aabb[0].x)
        #     self.height = int(self.aabb[1].y - self.aabb[0].y)
        #
        #     x = self.aabb[0].x
        #     y = self.aabb[0].y
        #
        #     print(x)
        #     print(y)
        #     print(self.width)
        #     print(self.height)
