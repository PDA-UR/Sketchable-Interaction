
import time
from sketchable_interaction.interaction.Artifact import *


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

        self.mouse_cursor = Cursor(self.cursor().pos().x(), self.cursor().pos().y(), self)
        self.mouse_cursor.set_role(InteractiveRegion.RoleType.BRUSH.value)
        self.mouse_cursor.set_effect(InteractiveRegion.EffectType.EFFECT_PALETTE.value)
        self.update_mouse_cursor()  # appears to be required for the interactive region to be on the right spot at certain spawning contexts

        self.artifacts.append(File(1, 50, 50, File.FileType.TEXT.value, self))

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
        In the future that requires more thought of which to select or if multiple should be allowed
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

    def get_mouse_cursor_delta(self):
        return self.mouse_cursor.get_current_position()[0] - self.mouse_cursor.get_last_position()[0], self.mouse_cursor.get_current_position()[1] - self.mouse_cursor.get_last_position()[1]

    def add_new_contour_point(self):
        if self.drawing and self.mouse_cursor.get_role() == InteractiveRegion.RoleType.BRUSH.value:
            self.current_drawn_points.append(self.mouse_cursor.current_position)

    def register_new_sketched_interactive_region(self, effect_type):
        self.drawing = False

        if len(self.current_drawn_points) > 0:
            if effect_type == InteractiveRegion.EffectType.EFFECT_PALETTE.value:
                parent_sketch = EffectPaletteSketch(self.current_drawn_points, self)
                self.artifacts.append(parent_sketch)

                parent_index = len(self.artifacts) - 1

                for i, child in enumerate(parent_sketch.get_sub_point_sets()):
                    child_sketch = EffectPaletteSketch(child, self, True, i)
                    child_sketch.is_palette_parent_sketch = False
                    child_sketch.set_parent_uuid(parent_sketch.get_id())
                    child_sketch.set_interactive_region_linked(True, parent_sketch.get_id())

                    self.artifacts[parent_index].set_interactive_region_linked(True, child_sketch.get_id())

                    self.artifacts.append(child_sketch)

                self.mouse_cursor.set_role(InteractiveRegion.RoleType.BRUSH.value)
                self.mouse_cursor.set_effect(InteractiveRegion.EffectType.NONE.value)
            else:
                # think about blank region integration which is programmable

                effect = self.mouse_cursor.get_effect()

                if effect == InteractiveRegion.EffectType.DELETE.value:
                    self.artifacts.append(DeletionSketch(self.current_drawn_points, self, False))
                elif effect == InteractiveRegion.EffectType.PREVIEW.value:
                    self.artifacts.append(PreviewSketch(self.current_drawn_points, self, False))
                elif effect == InteractiveRegion.EffectType.TAG.value:
                    self.artifacts.append(TagSketch(self.current_drawn_points, self, False))

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

        self.painter.setPen(QtGui.QPen(self.mouse_cursor.get_color_pen(), 5))

        self.painter.drawPolyline(self.poly(self.current_drawn_points))

        for i, artifact in enumerate(self.artifacts):
            artifact.render(self.painter)

        self.mouse_cursor.render(self.painter)

        self.painter.end()


class Interaction:

    @staticmethod
    def process(target_environment):
        Interaction.process_cursor_vs_artifacts(target_environment)
        Interaction.process_artifacts_vs_artifacts(target_environment)

    @staticmethod
    def process_cursor_vs_artifacts(target_environment):
        if target_environment.mouse_cursor.pressed_moved:
            collides_with_other, other = Interaction.__check_link_state_of_artifacts_to_cursor(target_environment)

            if not collides_with_other:
                target_environment.mouse_cursor.set_role(InteractiveRegion.RoleType.BRUSH.value)
                Interaction.__draw_sketch_contour_point(target_environment)
            else:
                if other == -1:
                    target_environment.mouse_cursor.set_role(InteractiveRegion.RoleType.BRUSH.value)
                    Interaction.__draw_sketch_contour_point(target_environment)
                else:
                    target_environment.mouse_cursor.set_role(InteractiveRegion.RoleType.MOVE.value)

                    Interaction.__move_artifact(target_environment, other)
        else:
            if len(target_environment.current_drawn_points) > 0:
                Interaction.__finish_sketching(target_environment)
                target_environment.mouse_cursor.set_role(InteractiveRegion.RoleType.BRUSH.value)
            else:
                for i, artifact in enumerate(target_environment.artifacts):
                    if artifact.is_child:
                        if artifact.collides_with_other(target_environment.mouse_cursor):
                            if target_environment.mouse_cursor.left_clicked:
                                target_environment.mouse_cursor.set_role(InteractiveRegion.RoleType.BRUSH.value)
                                artifact.on_emit_effect()

                                target_environment.mouse_cursor.on_receive_effect(i)

                                return

    @staticmethod
    def __check_link_state_of_artifacts_to_cursor(target_environment):
        collides_with_other = False
        other = -1

        for i, artifact in enumerate(target_environment.artifacts):
            if artifact.is_linked():
                if target_environment.mouse_cursor.id in artifact.get_linked_artifacts():
                    if target_environment.artifacts[i].collides_with_others_aabb(target_environment.mouse_cursor):
                        if target_environment.artifacts[i].collides_with_other(target_environment.mouse_cursor):
                            other = i
                            collides_with_other = True
                        else:
                            target_environment.artifacts[i].remove_link_by_id(target_environment.mouse_cursor.id)
                        break

        if other == -1 and target_environment.drawing is False:
            for i, artifact in enumerate(target_environment.artifacts):
                if target_environment.artifacts[i].collides_with_others_aabb(target_environment.mouse_cursor):
                    if target_environment.artifacts[i].collides_with_other(target_environment.mouse_cursor):
                        target_environment.artifacts[i].set_interactive_region_linked(True, target_environment.mouse_cursor.id)
                        target_environment.mouse_cursor.set_interactive_region_linked(True, target_environment.artifacts[i].id)
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

        if not target_environment.artifacts[other].is_child:
            target_environment.artifacts[other].on_receive_effect(-1)

        Interaction.__move_effect_palette_components(target_environment, other)

    @staticmethod
    def __move_effect_palette_components(target_environment, other):
        if target_environment.artifacts[other].is_palette_parent_sketch:
            for i, artifact in enumerate(target_environment.artifacts):
                if i == other:
                    continue

                if artifact.is_child and artifact.parent_uuid == target_environment.artifacts[other].get_id():
                    target_environment.mouse_cursor.set_interactive_region_linked(True, artifact.get_id())
                    target_environment.artifacts[i].on_receive_effect(-1)

    @staticmethod
    def __finish_sketching(target_environment):
        target_environment.register_new_sketched_interactive_region(target_environment.mouse_cursor.get_effect())
        target_environment.drawing = False

    @staticmethod
    def process_artifacts_vs_artifacts(target_environment):
        for i in range(len(target_environment.artifacts)):
            current = Interaction.__compute_current_intersections(target_environment, i)

            if len(current) > 0:
                Interaction.__handle_intersection_start_and_ending_occurrences(target_environment, i, current)
                Interaction.__handle_effect_emission_and_reception(target_environment, i, current)

    @staticmethod
    def __compute_current_intersections(target_environment, index):
        ret = []

        if target_environment.artifacts[index].is_palette_parent_sketch:
            return ret

        for i in range(len(target_environment.artifacts)):
            if i == index:
                continue

            if target_environment.artifacts[index].is_child and target_environment.artifacts[i].is_palette_parent_sketch:
                return ret
            else:
                if target_environment.artifacts[index].collides_with_other(target_environment.artifacts[i]):
                    ret.append(target_environment.artifacts[i].id)

        return ret

    @staticmethod
    def __handle_intersection_start_and_ending_occurrences(target_environment, index, current_intersections):
        last = target_environment.artifacts[index].get_last_intersections_list()

        if len(last) > 0:
            for _id in last:
                i, artifact = target_environment.get_artifact_by_id(_id)

                if _id in current_intersections:
                    target_environment.artifacts[i].on_intersection()
                else:
                    target_environment.artifacts[i].on_disjunction()

            target_environment.artifacts[index].clear_last_intersections_list()

    @staticmethod
    def __handle_effect_emission_and_reception(target_environment, index, current_intersections):
        for _id in current_intersections:
            for i in range(index):
                if not (_id in target_environment.artifacts[i].get_last_intersections_list()):
                    _i, artifact = target_environment.get_artifact_by_id(_id)

                    target_environment.artifacts[_i].on_emit_effect()
                    target_environment.artifacts[_i].on_receive_effect(index)

                    target_environment.artifacts[index].on_emit_effect()
                    target_environment.artifacts[index].on_receive_effect(i)

            target_environment.artifacts[index].add_intersection_to_last_intersections_list(_id)


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
