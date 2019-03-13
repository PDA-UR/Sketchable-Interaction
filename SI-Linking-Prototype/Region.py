from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

import numpy as np


class Region(QWidget):
    posi = pyqtSignal(int, int)

    def __init__(self, x, y, parent=None):
        super(Region, self).__init__(parent)

        self.setGeometry(x, y, 200, 125)

        self.movable = False
        self.oldpos = QPoint()

        self.color = self.randomize_color()
        print(self.color)

        self.label = QLabel()
        self.label.setParent(self)

        self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")
        self.label.setGeometry(0, 0, self.width(), self.height())
        self.label.show()

        self.show()

    def paintEvent(self, ev):
        qp = QPainter()
        qp.begin(self)

        qp.setBrush(self.color)
        qp.drawRect(ev.rect())

        qp.end()

    def mousePressEvent(self, ev):
        self.movable = True

        self.oldpos = ev.globalPos()

    def mouseMoveEvent(self, ev):
        if self.movable:
            delta = QPoint(ev.globalPos() - self.oldpos)

            self.move(self.x() + delta.x(), self.y() + delta.y())

            self.oldpos = ev.globalPos()

            self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")

            self.posi.emit(delta.x(), delta.y())

            self.update()

    def mouseReleaseEvent(self, ev):
        self.movable = False

    def randomize_color(self):
        color = list(np.random.choice(range(235), size=3))

        return QColor(color[0], color[1], color[2])

    def position(self, x, y):
        self.move(self.x() + x, self.y() + y)
        self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")
