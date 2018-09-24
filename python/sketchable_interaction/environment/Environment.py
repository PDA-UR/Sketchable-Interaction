
from PyQt5 import QtWidgets, QtCore, QtGui
import time
from sketchable_interaction.interaction.Artifact import Artifact, Sketch, Cursor, InteractiveRegion


class Environment(QtWidgets.QWidget):
    def __init__(self):
        super(Environment, self).__init__()

        self.resize(1920, 1080)
        self.setFocusPolicy(QtCore.Qt.StrongFocus)
        self.drawing = False

        self.heart_beat = HeartBeat(33.0)
        self.heart_beat.heart_beat.connect(self.update_all)
        self.heart_beat.start()

        self.painter = QtGui.QPainter()

        self.previous_pointer_location = None

        self.current_drawn_points = []
        self.artifacts = []

        self.mouse_cursor = Cursor()

    def update_all(self):
        Interaction.process()

        self.update()

    def point_collides_with_artifacts(self, p):
        """
        In the future requires more thought of which to select or if multiple should be allowed
        """

        for a in self.artifacts:
            if a.collides_with_point(p):
                return a

        return None

    def move_region(self, q):
        if self.previous_pointer_location is None:
            self.previous_pointer_location = q

        vector = (q[0] - self.previous_pointer_location[0], q[1] - self.previous_pointer_location[1])

        artifact = self.point_collides_with_artifacts(q)

        if not self.drawing:
            if artifact is not None:
                artifact.move_interactive_region(vector)

    def __handle_left_button(self, ev):
        self.previous_pointer_location = None

        p = (ev.x(), ev.y())

        artifact = self.point_collides_with_artifacts(p)

        if artifact is None:
            self.current_drawn_points = []
            self.drawing = True
        else:
            artifact.set_interactive_region_linked(True, self.mouse_cursor.id)

    def __handle_right_button(self, ev):
        p = (ev.x(), ev.y())

        for artifact in self.artifacts:
            if artifact.collides_with_point(p):
                print("Hit with Point")

        for i in range(len(self.artifacts)):
            for k in range(i + 1, len(self.artifacts)):
                if self.artifacts[i].collides_with_other(self.artifacts[k]):
                    print("Intersection")

    def keyPressEvent(self, ev):
        if ev.key() == QtCore.Qt.Key_Escape:
            self.close()

    def mousePressEvent(self, ev):
        if ev.button() == QtCore.Qt.LeftButton:
            self.__handle_left_button(ev)
        elif ev.button() == QtCore.Qt.RightButton:
            self.__handle_right_button(ev)

    def mouseReleaseEvent(self, ev):
        if ev.button() == QtCore.Qt.LeftButton:
            self.previous_pointer_location = None
            self.drawing = False

            if len(self.current_drawn_points) > 0:
                s = Sketch()
                s.set_interactive_region_from_ordered_point_set(self.current_drawn_points,
                                                                InteractiveRegion.EffectType.NONE.value,
                                                                InteractiveRegion.RoleType.NONE.value,
                                                                InteractiveRegion.Direction.UNIDIRECTIONAL.value,
                                                                self)

                self.artifacts.append(s)

            self.current_drawn_points = []

    def mouseMoveEvent(self, ev):
        p = (ev.x(), ev.y())

        self.move_region(p)

        if self.drawing:
            self.current_drawn_points.append(p)

        self.previous_pointer_location = p

    def poly(self, pts):
        return QtGui.QPolygonF(map(lambda p: QtCore.QPointF(*p), pts))

    def paintEvent(self, ev):
        self.painter.begin(self)
        self.painter.setBrush(QtGui.QColor(0, 0, 0))
        self.painter.drawRect(ev.rect())
        self.painter.setBrush(QtGui.QColor(0, 255, 0))
        self.painter.setPen(QtGui.QPen(QtGui.QColor(0, 155, 0), 5))

        self.painter.drawPolyline(self.poly(self.current_drawn_points))

        for artifact in self.artifacts:
            self.painter.drawPolyline(self.poly(artifact.get_interactive_region_contour()))

        self.painter.end()


class Interaction:

    @staticmethod
    def process():
        pass


class HeartBeat(QtCore.QThread):
    heart_beat = QtCore.pyqtSignal()

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
