
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

        self.mouse_cursor = Cursor(self.cursor().pos().x(), self.cursor().pos().y())
        self.update_mouse_cursor()  # appears to be required for the interactive region to be on the right spot at certain spawning contexts

    def update_all(self):
        self.update_mouse_cursor()

        Interaction.process(self)

        self.update()
        self.mouse_cursor.set_last_position(self.mouse_cursor.get_current_position())

    def update_mouse_cursor(self):
        self.mouse_cursor.set_current_position((self.cursor().pos().x(), self.cursor().pos().y()))

        if self.mouse_cursor.get_last_position() is None:
            self.mouse_cursor.set_last_position(self.mouse_cursor.get_current_position())

        vector = (self.mouse_cursor.get_current_position()[0] - self.mouse_cursor.get_last_position()[0], self.mouse_cursor.get_current_position()[1] - self.mouse_cursor.get_last_position()[1])

        self.mouse_cursor.move_interactive_region(vector)

    def point_collides_with_artifacts(self, p):
        """
        In the future requires more thought of which to select or if multiple should be allowed
        """

        for a in self.artifacts:
            if a.collides_with_point(p):
                return a

        return None

    def get_artifact_by_id(self, _id):
        for i, artifact in enumerate(self.artifacts):
            if artifact.id == _id:
                return i, artifact

        return -1, None

    def move_artifact(self, index):
        if not self.drawing:
            vector = (self.mouse_cursor.get_current_position()[0] - self.mouse_cursor.get_last_position()[0], self.mouse_cursor.get_current_position()[1] - self.mouse_cursor.get_last_position()[1])

            self.artifacts[index].move_interactive_region(vector)

    def add_new_contour_point(self):
        if self.drawing:
            self.current_drawn_points.append(self.mouse_cursor.current_position)

    def register_new_sketched_interactive_region(self):
        self.drawing = False
        s = Sketch()
        s.set_interactive_region_from_ordered_point_set(self.current_drawn_points,
                                                        InteractiveRegion.EffectType.NONE.value,
                                                        InteractiveRegion.RoleType.NONE.value,
                                                        InteractiveRegion.Direction.UNIDIRECTIONAL.value,
                                                        False, [])

        self.artifacts.append(s)

        self.current_drawn_points = []

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
            self.mouse_cursor.left_clicked = True
            self.current_drawn_points = []
            self.mouse_cursor.pressed_moved = False
        if ev.button() == QtCore.Qt.RightButton:
            self.__handle_right_button(ev)

    def mouseReleaseEvent(self, ev):
        if ev.button() == QtCore.Qt.LeftButton:
            self.mouse_cursor.pressed_moved = False
            self.mouse_cursor.left_clicked = False

    def mouseMoveEvent(self, ev):
        self.mouse_cursor.pressed_moved = True

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

        self.painter.drawPolyline(self.poly(self.mouse_cursor.get_interactive_region_contour()))

        self.painter.end()


class Interaction:

    @staticmethod
    def process(target_environment):
        Interaction.process_cursor_vs_artifacts(target_environment)
        Interaction.process_artifact_vs_artifacts(target_environment)

    @staticmethod
    def process_cursor_vs_artifacts(target_environment):
        if target_environment.mouse_cursor.pressed_moved:
            collides_with_other, other = Interaction.__check_link_state_of_artifacts_to_cursor(target_environment)

            if not collides_with_other:
                Interaction.__draw_sketch_contour_point(target_environment)
            else:
                if other == -1:
                    Interaction.__draw_sketch_contour_point(target_environment)
                else:
                    Interaction.__move_artifact(target_environment, other)
        else:
            if len(target_environment.current_drawn_points) > 0:
                Interaction.__finish_sketching(target_environment)

    @staticmethod
    def __check_link_state_of_artifacts_to_cursor(target_environment):
        collides_with_other = False
        other = -1

        for i, artifact in enumerate(target_environment.artifacts):
            if artifact.is_linked():
                if target_environment.mouse_cursor.id in artifact.get_linked_artifacts():
                    if target_environment.artifacts[i].collides_with_other(target_environment.mouse_cursor):
                        other = i
                        collides_with_other = True
                    else:
                        target_environment.artifacts[i].remove_link_by_id(target_environment.mouse_cursor.id)
                    break

        if other == -1 and target_environment.drawing is False:
            for i, artifact in enumerate(target_environment.artifacts):
                if target_environment.artifacts[i].collides_with_other(target_environment.mouse_cursor):
                    target_environment.artifacts[i].set_interactive_region_linked(True, target_environment.mouse_cursor.id)
                    other = i
                    collides_with_other = True
                    break

        return collides_with_other, other

    @staticmethod
    def __draw_sketch_contour_point(target_environment):
        target_environment.drawing = True
        target_environment.add_new_contour_point()

    @staticmethod
    def __move_artifact(target_environment, other):
        target_environment.drawing = False
        target_environment.move_artifact(other)

    @staticmethod
    def __finish_sketching(target_environment):
        target_environment.register_new_sketched_interactive_region()
        target_environment.drawing = False

    @staticmethod
    def process_artifact_vs_artifacts(target_environment):
        for i in range(len(target_environment.artifacts)):
            b = []

            for k in range(len(target_environment.artifacts)):
                if i == k:
                    continue

                if target_environment.artifacts[i].collides_with_other(target_environment.artifacts[k]):
                    b.append(target_environment.artifacts[k].id)

            # check for lost intersections
            # for each lost intersection call on_intersection_ended

            last = target_environment.artifacts[i].get_last_intersections_list()

            if len(last) > 0:
                for _id in last:
                    index, artifact = target_environment.get_artifact_by_id(_id)

                    if _id in b:
                        target_environment.artifacts[index].on_intersection()
                    else:
                        target_environment.artifacts[index].on_disjunction()

                target_environment.artifacts[i].clear_last_intersections_list()

            for _id in b:
                # do this once and not twice for each intersection
                # check for previous artifacts whether this id is in last intersections?

                index, artifact = target_environment.get_artifact_by_id(_id)

                target_environment.artifacts[index].on_receive_effect()
                target_environment.artifacts[i].on_receive_effect()
                target_environment.artifacts[i].add_intersection_to_last_intersections_list(_id)


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
