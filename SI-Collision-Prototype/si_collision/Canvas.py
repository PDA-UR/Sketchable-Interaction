from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from MoveableRegion import MoveableRegion

import time
import itertools


class Canvas(QMainWindow):
    def __init__(self, width, height):
        super(Canvas, self).__init__()

        self.setGeometry(0, 0, width, height)
        self.showFullScreen()

        """
        MoveableRegion(100, 50, self)
        MoveableRegion(550, 50, self)
        MoveableRegion(1000, 50, self)
        MoveableRegion(600, 550, self)
        MoveableRegion(900, 550, self)
        """

        l = int(1920 / (200 + 30)) + 1
        m = int(1080 / (125 + 30)) + 2

        for i in range(l):
            for k in range(m):
                x = 15 + i * 215
                y = 15 + k * 130

                MoveableRegion(x, y, self)

        self.timer = QTimer()
        self.timer.setInterval(33.0)
        self.timer.timeout.connect(self.update_all)
        self.timer.start()

        self.show()

    def update_all(self):
        self.collide()
        self.update()

    def collide(self):
        for r1, r2 in itertools.combinations(self.findChildren(MoveableRegion), 2):
            if r1.shape.intersects(r2.shape):
                r1.color = Qt.red
                r2.color = Qt.green
            else:
                r1.color = QColor(0x55, 0x55, 0x55)
                r2.color = QColor(0x55, 0x55, 0x55)

    def keyPressEvent(self, ev):
        if ev.key() == Qt.Key_Escape:
            self.close()

    def paintEvent(self, ev):
        qp = QPainter()
        qp.begin(self)

        qp.setBrush(QColor(0, 0, 0))
        qp.drawRect(ev.rect())

        qp.end()
