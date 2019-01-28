from PyQt5 import QtWidgets, QtCore
from si import SIPlugin


class Test(SIPlugin):
    def __init__(self):
        super(Test, self).__init__("trackable", self.on_region_enter, self.on_region_continuous, self.on_region_leave, self.on_create, self.on_destroy)

    def on_region_enter(self, integer):
        self.setObjectName("hello")
        return 0

    def on_region_continuous(self):
        return 0

    def on_region_leave(self, integer):
        print(int(self.winId()) == integer)

        return 0

    def on_create(self):
        return 0

    def on_destroy(self):
        return 0
