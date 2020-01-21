from libPySI import PySIEffect, PySICapability

class Cursor(PySIEffect):
    def __init__(self):
        super(Cursor, self).__init__()

        self.name = "Cursor"
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