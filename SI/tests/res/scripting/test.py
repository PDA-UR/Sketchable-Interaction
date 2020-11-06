
from libPySI import PySI


class Test(PySI.Effect):
    def __init__(self):
        super(Test, self).__init__(PySI.PointVector(), "", "", {})

        self.x = 5