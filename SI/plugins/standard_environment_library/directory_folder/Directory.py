from libPySI import PySIEffect, PySICapability


class Directory(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid

        self.name = "stdSIDir"
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/directory/dir.png"
        self.color = PySIEffect.Color(0, 0, 0, 0)

        self.width = 50
        self.height = 25

        self.add_data("width", self.width, PySIEffect.DataType.INT)
        self.add_data("height", self.height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/dir.png", PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({

        })

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({

        })

        self.cap_link_emit = PySIEffect.String2FunctionMap({

        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({

        })

    # functions for collision events
    # ...

    # functions for linking events
    # ...