from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Region import Region
from Capabiliy import Capability

import numpy as np
import uuid


class MoveableRegion(Region):
    def __init__(self, x, y, parent=None):
        super(MoveableRegion, self).__init__(x, y, parent)

        self.link_capabilities.append(Capability.POSITION)

        self.movable = False
        self.oldpos = QPoint()

        self.color = self.randomize_color()

        self.label = QLabel()
        self.label.setParent(self)

        self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")
        self.label.setGeometry(15, -self.height() / 2 + 15, self.width(), self.height())
        self.label.show()

    def __repr__(self):
        return "MoveableRegion: " + str(int(self.winId()))

    def mousePressEvent(self, ev):
        self.movable = True

        self.oldpos = ev.globalPos()

    def mouseMoveEvent(self, ev):
        if self.movable:
            delta = QPoint(ev.globalPos() - self.oldpos)

            self.move(self.x() + delta.x(), self.y() + delta.y())

            self.oldpos = ev.globalPos()

            self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")

            self.link_events[Capability.POSITION] = uuid.uuid4()

            self.posi.emit(delta.x(), delta.y(), self.link_events[Capability.POSITION])  # is this deleted when link is undone?

            p = QPoint(self.x() + self.width() / 2, self.y())
            q = QPoint(self.x(), self.y() + self.height() / 2)
            r = QPoint(self.x() + self.width() / 2, self.y() + self.height())
            s = QPoint(self.x() + self.width(), self.y() + self.height() / 2)

            self.link_visualization_handles = [p, q, r, s]

    def mouseReleaseEvent(self, ev):
        self.movable = False

    def randomize_color(self):
        r, g, b = list(np.random.choice(range(150), size=3))

        return QColor(r, g, b)

    def position(self, x, y, uid):
        if Capability.POSITION in self.link_events.keys():
            if uid != self.link_events[Capability.POSITION]:
                self.move(self.x() + x, self.y() + y)
                self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")

                self.link_events[Capability.POSITION] = uid

                self.posi.emit(x, y, uid)

                self.update_visualization_handles()

        self.link_events[Capability.POSITION] = uid

    def update_visualization_handles(self):
        p = QPoint(self.x() + self.width() / 2, self.y())
        q = QPoint(self.x(), self.y() + self.height() / 2)
        r = QPoint(self.x() + self.width() / 2, self.y() + self.height())
        s = QPoint(self.x() + self.width(), self.y() + self.height() / 2)

        self.link_visualization_handles = [p, q, r, s]
