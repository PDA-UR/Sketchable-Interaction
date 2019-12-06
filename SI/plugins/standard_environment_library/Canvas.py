from libPySI import PySIEffect, PySICapability


class Canvas(PySIEffect):
    def __init__(self):
        super(Canvas, self).__init__()

        self.name = "Canvas"
        self.region_type = "custom"
        self.texture_path = "src/siren/res/textures/scale_texture.png"

        self.cap_emit = {
        }

        self.cap_recv = {
        }

    def __repr__(self):
        return ""