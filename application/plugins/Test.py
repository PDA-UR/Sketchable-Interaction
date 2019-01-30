from PyQt5 import QtWidgets, QtCore
from si import SIRegion


class Test(SIRegion):
    def __init__(self):
        super(Test, self).__init__( self.on_region_enter, self.on_region_continuous, self.on_region_leave)

    def on_region_enter(self, uuid):
        print("Hello Enter Python")
        return 0

    def on_region_continuous(self, uuid):
        print("Hello Conti Python")
        return 0

    def on_region_leave(self, uuid):
        print("Hello Leave Python")

        return 0