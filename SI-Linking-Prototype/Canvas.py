from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from MoveableRegion import MoveableRegion
from ScalableRegion import ScalableRegion
from Region import Region

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

        ScalableRegion(500, 500, self)
        ScalableRegion(800, 500, self)

        self.heart_beat = HeartBeat(33.0)
        self.heart_beat.heart_beat.connect(self.update_all)
        self.heart_beat.start()

        w1, w2, w3, w4, w5 = self.findChildren(Region)

        self.make_link(w1, w2, Capability.POSITION, Capability.POSITION)
        self.make_link(w2, w3, Capability.POSITION, Capability.POSITION)
        self.make_link(w3, w2, Capability.POSITION, Capability.POSITION)
        self.make_link(w1, w4, Capability.POSITION, Capability.SCALE)
        self.make_link(w4, w5, Capability.SCALE, Capability.SCALE)
        self.make_link(w5, w4, Capability.SCALE, Capability.SCALE)

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
            pass
        elif ev.key() == Qt.Key_Return:
           pass
        elif ev.key() == Qt.Key_Space:
            self.toggle_linkage_display()

    def make_link(self, w1, w2, attribute_w1, attribute_w2):
        if w1.has_capability(attribute_w1) and w2.has_capability(attribute_w2):
            w1.set_is_linked_to(w2, attribute_w1, attribute_w2)

            eval('w1.' + w1.get_capability_signal(attribute_w1) + '.connect(w2.' + w2.get_capability_function(attribute_w2) + ')')

    def undo_link(self, w1, w2, attribute):
        """
        linked, how = w1.is_linked_to(w2, attribute)

        if linked:
            if how == 0:
                w1.posi.disconnect(w2.position)
            else:
                w2.posi.disconnect(w1.position)

        linked, how = w2.is_linked_to(w1, attribute)

        if linked:
            if how == 0:
                w2.posi.disconnect(w1.position)
            else:
                w1.posi.disconnect(w2.position)

        w1.delete_link_to(w2, attribute)
        w2.delete_link_to(w1, attribute)
        """

    def paintEvent(self, ev):
        qp = QPainter()
        qp.begin(self)

        qp.setBrush(QColor(0, 0, 0))
        qp.drawRect(ev.rect())

        self.draw_links(qp)

        qp.end()

    def draw_links(self, qp):
        if self.is_linkage_shown:
            for w1 in self.findChildren(Region):
                for w2 in self.findChildren(Region):
                    if w1 == w2:
                        continue

                    self.visualize_position_position_link(qp, w1, w2)
                    self.visualize_position_scale_link(qp, w1, w2)
                    self.visualize_scale_scale_link(qp, w1, w2)

    def visualize_position_position_link(self, qp, w1, w2):
        is_linked, priority = w1.is_linked_to(w2, Capability.POSITION, Capability.POSITION)  # potential any attribute necessary which then checks for all?

        if is_linked:
            qp.setPen(QPen(QColor(0, 255, 0), 5))

            if priority == 0:
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

    def visualize_position_scale_link(self, qp, w1, w2):
        is_linked, priority = w1.is_linked_to(w2, Capability.POSITION, Capability.SCALE)

        if is_linked:
            qp.setPen(QPen(QColor(255, 0, 0), 5))

            if priority == 0:
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

                self.visualize_arrow(qp, p, q)

    def visualize_scale_scale_link(self, qp, w1, w2):
        is_linked, priority = w1.is_linked_to(w2, Capability.SCALE, Capability.SCALE)

        if is_linked:
            qp.setPen(QPen(QColor(255, 0, 0), 5))

            if priority == 0:
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

                self.visualize_arrow(qp, p, q)

    def visualize_arrow(self, qp, p, q):
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

        qp.fillPath(qpp, QColor(255, 0, 0))
