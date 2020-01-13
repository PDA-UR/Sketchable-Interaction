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

        self.cap_link_emit = {
            "__position__": self.position,
            "__scale__": self.scale
        }

        self.cap_link_recv = {
            "__position__": {"__position__": self.set_position_from_position},
            "__color__": {"__position__": self.set_position_from_color}
        }

    def __repr__(self):
        return ""

    def position(self):
        return [1, 1]

    def scale(self):
        return [1]

    def set_position_from_position(self, args):
        print("From Position 1", args)
        return 0

    def set_position_from_color(self, args):
        print("From Color 1", args)
        return 0