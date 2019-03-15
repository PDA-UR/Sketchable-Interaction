from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from MoveableRegion import MoveableRegion
from Capabiliy import Capability
from Math import Math

import math

import time
import inspect


class HeartBeat(QThread):
    heart_beat = pyqtSignal()

    def __init__(self, frequency, sleep=True):
        super(HeartBeat, self).__init__()
        self.is_running = True
        self.frequency = frequency
        self.sleep = sleep

    def run(self):
        while self.is_running:
            if self.sleep:
                time.sleep(1.0 / self.frequency)

            self.heart_beat.emit()


class Canvas(QMainWindow):
    def __init__(self, width, height):
        super(Canvas, self).__init__()

        self.setGeometry(0, 0, width, height)
        self.showFullScreen()

        self.is_linkage_shown = True

        MoveableRegion(0, 0, self)
        MoveableRegion(450, 0, self)
        MoveableRegion(900, 0, self)

        self.heart_beat = HeartBeat(33.0)
        self.heart_beat.heart_beat.connect(self.update_all)
        self.heart_beat.start()

        w1, w2, w3 = self.findChildren(MoveableRegion)

        self.make_link(w1, w2, Capability.POSITION)
        self.make_link(w2, w1, Capability.POSITION)
        self.make_link(w2, w3, Capability.POSITION)
        self.make_link(w3, w2, Capability.POSITION)
        self.make_link(w3, w2, Capability.POSITION)

        self.show()

    def update_all(self):

        self.update()
        pass

    def toggle_linkage_display(self):
        self.is_linkage_shown = not self.is_linkage_shown

    def keyPressEvent(self, ev):
        if ev.key() == Qt.Key_Escape:
            self.close()
        elif ev.key() == Qt.Key_A:
            # spawn new widget at 0, 0 (or space where it has room)
            pass
        elif ev.key() == Qt.Key_Return:
            w1, w2, w3 = self.findChildren(MoveableRegion)

            linked, how = w1.is_linked_to(w2, Capability.POSITION)

            if linked:
                self.undo_link(w1, w2, Capability.POSITION)
            else:
                self.make_link(w2, w1, Capability.POSITION)
        elif ev.key() == Qt.Key_Space:
            self.toggle_linkage_display()

    def make_link(self, w1, w2, attribute):
        if attribute == Capability.POSITION:
            w1.set_is_linked_to(w2, attribute)

            w1.posi.connect(w2.position)

    def undo_link(self, w1, w2, attribute):
        linked, how = w1.is_linked_to(w2, attribute)

        if linked:
            if how == 0:
                w1.posi.disconnect(w2.position)
            else:
                w2.posi.disconnect(w1.position)

        w1.delete_link_to(w2, attribute)
        w2.delete_link_to(w1, attribute)

    def paintEvent(self, ev):
        qp = QPainter()
        qp.begin(self)

        qp.setBrush(QColor(0, 0, 0))
        qp.drawRect(ev.rect())

        self.draw_links(qp)

        qp.end()

    def draw_links(self, qp):
        if self.is_linkage_shown:
            qp.setPen(QPen(QColor(0, 255, 0), 5))

            for w1 in self.findChildren(MoveableRegion):
                for w2 in self.findChildren(MoveableRegion):
                    if w1 == w2:
                        continue

                    is_linked, priority = w1.is_linked_to(w2, Capability.POSITION)  # potential any attribute necessary which then checks for all?

                    if is_linked:
                        if priority == 0:
                            radius = 8

                            distance_min = 9001
                            p = QPoint()
                            q = QPoint()

                            for r in w1.link_visualization_handles:
                                for s in w2.link_visualization_handles:
                                    t = s - r

                                    distance = Math.vector_norm([t.x(), t.y()])

                                    if distance < distance_min:
                                        distance_min = distance
                                        p = r
                                        q = s

                            qp.drawLine(p, q)

                            v = q - p

                            nv = Math.normalize_vector([v.x(), v.y()])
                            r = QPointF(nv[0], nv[1])

                            nr = QPointF(-r.y(), r.x())

                            qp.setPen(QPen(QColor(0, 255, 0), 5))

                            arrow_p = q - r * 35 + nr * 20
                            arrow_q = q - r * 35 - nr * 20

                            qpp = QPainterPath()

                            qpf = QPolygonF()
                            qpf.append(arrow_p)
                            qpf.append(q)
                            qpf.append(arrow_q)
                            qpp.addPolygon(qpf)

                            qp.fillPath(qpp, QColor(0, 255, 0))

