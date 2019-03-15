from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *


class Region(QWidget):
    posi = pyqtSignal(int, int, object)

    def __init__(self, x, y, parent=None):
        super(Region, self).__init__(parent)

        self.setGeometry(x, y, 200, 125)

        self.links = []

        p = QPoint(self.x() + self.width() / 2, self.y())
        q = QPoint(self.x(), self.y() + self.height() / 2)
        r = QPoint(self.x() + self.width() / 2, self.y() + self.height())
        s = QPoint(self.x() + self.width(), self.y() + self.height() / 2)

        self.link_visualization_handles = [p, q, r, s]

        self.link_events = {}
        self.link_capabilities = []

        self.show()

    def set_is_linked_to(self, other, attribute):
        linked, priority = self.is_linked_to(other, attribute)

        if linked:
            idx1, idx2 = self.get_linkage_indices(other)

            self.links[idx1] = (self.links[idx1][0], self.links[idx1][1], 0)
            other.links[idx2] = (other.links[idx2][0], other.links[idx2][1], 0)
        else:
            self.links.append((other, attribute, 0))
            other.links.append((self, attribute, 1))

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

    def is_linked_to(self, other, attribute):
        for link in self.links:
            linked_other = link[0]
            linked_attribute = link[1]
            linked_priority = link[2]

            if linked_other == other and linked_attribute == attribute:
                return True, linked_priority

        return False, -1

    def delete_link_to(self, other, attribute):
        for i in range(len(self.links) - 1, -1, -1):
            linked_other = self.links[i][0]
            linked_attribute = self.links[i][1]
            linked_priority = self.links[i][2]

            if linked_other == other and linked_attribute == attribute:
                del self.links[i]

    def paintEvent(self, ev):
        qp = QPainter()
        qp.begin(self)

        qp.setBrush(self.color)
        qp.drawRect(ev.rect())

        qp.end()
