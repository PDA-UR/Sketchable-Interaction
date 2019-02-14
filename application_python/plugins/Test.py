from PyQt5 import QtGui
from lib.si import SIRegion


class Test(SIRegion):
    def __init__(self):
        super(Test, self).__init__(self.on_region_enter, self.on_region_continuous, self.on_region_leave)
        self.show()

    def on_region_enter(self, uuid):
        print("Hello Enter Python")
        return 0

    def on_region_continuous(self, uuid):
        print("Hello Conti Python")
        return 0

    def on_region_leave(self, uuid):
        print("Hello Leave Python")

        return 0

    def paintEvent(self, ev):
        qp = QtGui.QPainter()

        qp.begin(self)

        qp.end()

