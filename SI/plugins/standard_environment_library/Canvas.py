from libPySI import PySIEffect, PySICapability


class Canvas(PySIEffect):
    def __init__(self):
        super(Canvas, self).__init__()

        self.name = "Canvas"
        self.region_type = "libstdSI"
        self.texture_path = ""

        self.cap_emit = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}

        }

        self.cap_recv = {
            # capability: {"on_enter": self.function, "on_continuous": self.function, "on_leave": self.function}

        }

        self.cap_link_emit = {
            # attr: self.get_function,
        }

        self.cap_link_recv = {
            # "source_attr": {"recv_attr": self.set_function},
        }