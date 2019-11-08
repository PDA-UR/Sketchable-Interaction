from libPySI import PySIEffect


class Canvas(PySIEffect):
    def __init__(self):
        super(Canvas, self).__init__()

        self.name = "Canvas"
        self.region_type = "canvas"