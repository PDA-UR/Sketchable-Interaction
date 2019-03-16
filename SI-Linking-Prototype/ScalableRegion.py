from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Region import Region
from Capabiliy import Capability

import numpy as np
import uuid


class ScalableRegion(Region):
    scali = pyqtSignal()

    def __init__(self, x, y, parent):
        super(ScalableRegion, self).__init__(x, y, parent)

        self.add_capability(Capability.SCALE, "scali", "scale")

        self.label = QLabel()
        self.label.setParent(self)

        self.label.setText("scale: (" + str(self.width()) + ", " + str(self.height()) + ")")
        self.label.setGeometry(15, -self.height() / 2 + 15, self.width(), self.height())
        self.label.show()

    def __repr__(self):
        return "ScalableRegion: " + str(int(self.winId()))

    def scale(self, x, y, uid):
        print("scale:", x, y)
