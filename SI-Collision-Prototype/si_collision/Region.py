from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Capabiliy import Capability

import uuid


class Region(QWidget):
    def __init__(self, x, y, parent=None):
        super(Region, self).__init__(parent)

        self.setGeometry(x, y, 200, 125)

        self.links = []

        self.shape = QRectF()
        self.update_shape()

        self.link_visualization_handles = []
        self.update_visualization_handles()

        self.link_events = {}
        self.link_capabilities = {}

        self.color = self.randomize_color()

        self.show()

    def add_capability(self, attribute, signal, func):
        self.link_capabilities[attribute] = (signal, func)

    def add_link_event(self, capability, uid):
        self.link_events[capability] = uid

    def randomize_color(self):
        # r, g, b = list(np.random.choice(range(150), size=3))
        r, g, b = 0x55, 0x55, 0x55

        return QColor(r, g, b)

    def set_is_linked_to(self, other, attribute_self, attribute_other):
        if self.has_capability(attribute_self) and other.has_capability(attribute_other):
            linked, priority = self.is_linked_to(other, attribute_self, attribute_other)

            if linked:
                idx1, idx2 = self.get_linkage_indices(other)

                self.links[idx1] = (self.links[idx1][0], self.links[idx1][1], self.links[idx1][2], 0)
                other.links[idx2] = (other.links[idx2][0], other.links[idx2][1], other.links[idx2][2], 0)
            else:
                self.links.append((other, attribute_self, attribute_other, 0))
                other.links.append((self, attribute_self, attribute_other, 1))

    def get_linkage_indices(self, other):
        idx1, idx2 = -1, -1

        for i in range(len(self.links)):
            if self.links[i][0] == other:
                idx1 = i
                break

        for i in range(len(other.links)):
            if other.links[i][0] == self:
                idx2 = i
                break

        return idx1, idx2

    def is_linked_to(self, other, attribute_self, attribute_other):
        for link in self.links:
            linked_other = link[0]
            linked_attribute_self = link[1]
            linked_attribute_other = link[2]
            linked_priority = link[3]

            if linked_other == other and linked_attribute_other == attribute_other and linked_attribute_self == attribute_self:
                return True, linked_priority

        return False, -1

    def delete_link_to(self, other, attribute):
        for i in range(len(self.links) - 1, -1, -1):
            linked_other = self.links[i][0]
            linked_attribute_self = self.links[i][1]
            linked_attribute_other = self.links[i][2]
            linked_priority = self.links[i][3]

            # if linked_other == other and linked_attribute == attribute:
                # del self.links[i]

    def has_capability(self, attribute):
        return attribute in self.link_capabilities

    def mousePressEvent(self, ev):
        self.movable = True

        self.oldpos = ev.globalPos()

    def mouseMoveEvent(self, ev):
        if self.movable and Capability.POSITION in self.link_capabilities:
            delta = QPoint(ev.globalPos() - self.oldpos)

            self.move(self.x() + delta.x(), self.y() + delta.y())

            self.oldpos = ev.globalPos()

            self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")

            self.add_link_event(Capability.POSITION, uuid.uuid4())

            eval('self.' + self.get_capability_signal(Capability.POSITION)).emit(delta.x(), delta.y(), self.link_events[Capability.POSITION])

            self.update_shape()
            self.update_visualization_handles()

    def mouseReleaseEvent(self, ev):
        self.movable = False

    def update_shape(self):
        p = QPoint(self.x(), self.y())
        r = QPoint(self.x() + self.width(), self.y() + self.height())

        self.shape = QRectF(p, r)

    def update_visualization_handles(self):
        p = QPoint(self.x() + self.width() / 2, self.y())
        q = QPoint(self.x(), self.y() + self.height() / 2)
        r = QPoint(self.x() + self.width() / 2, self.y() + self.height())
        s = QPoint(self.x() + self.width(), self.y() + self.height() / 2)

        self.link_visualization_handles = [p, q, r, s]

    def paintEvent(self, ev):
        qp = QPainter()
        qp.begin(self)

        qp.setBrush(self.color)
        qp.drawRect(ev.rect())

        qp.end()

    def set_capability_function(self, function, attribute):
        self.link_capabilities[attribute] = (self.link_capabilities[attribute][0], function)

    def get_capability_function(self, attribute):
        return self.link_capabilities[attribute][1]

    def set_capability_signal(self, signal, attribute):
        self.link_capabilities[attribute] = (signal, self.link_capabilities[attribute][1])

    def get_capability_signal(self, attribute):
        return self.link_capabilities[attribute][0]
