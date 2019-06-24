from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Region import Region
from Capabiliy import Capability
import numpy as np
import uuid


class ScalableRegion(Region):
    scali = pyqtSignal(int, int, object)

    def __init__(self, x, y, parent):
        super(ScalableRegion, self).__init__(x, y, parent)

        self.add_capability(Capability.SCALE, "scali", "scale")

        self.label = QLabel()
        self.label.setParent(self)

        self.label.setText("scale: (" + str(self.width()) + "x, " + str(self.height()) + "x)")
        self.label.setGeometry(15, -self.height() / 2 + 15, self.width(), self.height())
        self.label.show()

    def __repr__(self):
        return "ScalableRegion: " + str(int(self.winId()))

    def scale(self, x, y, uid):
        if Capability.SCALE in self.link_events.keys():
            if uid != self.link_events[Capability.SCALE]:

                # actually required from app programmer, rest is safeguards against infinite recursion
                self.setGeometry(self.x(), self.y(), self.width() * float(x / 100 + 1), self.height() * float(y / 100 + 1))
                self.label.setText("scale: (" + str(self.width()) + "x, " + str(self.height()) + "x)")
                ######################################################################################

                self.add_link_event(Capability.SCALE, uid)
                eval('self.' + self.get_capability_signal(Capability.SCALE)).emit(x, y, uid)
                self.update_visualization_handles()

        self.add_link_event(Capability.SCALE, uid)
