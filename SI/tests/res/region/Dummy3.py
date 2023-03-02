from libPySI import PySI

from plugins.standard_environment_library.SIEffect import SIEffect


class Dummy3(SIEffect):
    regiontype = PySI.EffectType.SI_CUSTOM
    regionname = "__ Dummy3 __"
    region_display_name = "Dummy3"

    def __init__(self, shape: PySI.PointVector = PySI.PointVector(), uuid: str = "", kwargs: dict = {}) -> None:
        super().__init__(shape, uuid, "", Dummy3.regiontype, Dummy3.regionname, kwargs)
        self.source = "libStdSI"
        self.qml_path = ""