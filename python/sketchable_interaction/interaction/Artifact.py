
from enum import Enum
from sketchable_interaction.interaction.InteractiveRegion import InteractiveRegion
import uuid
import math
from PyQt5 import QtCore, QtWidgets, QtGui


class Artifact(QtCore.QObject):
    class ArtifactType(Enum):
        SKETCH = 0
        FINGER = 1
        TANGIBLE = 2
        SMARTPHONE = 3
        CURSOR = 4
        FILE = 5

    def __init__(self, artifact_type, parent):
        super(Artifact, self).__init__()
        self.id = uuid.uuid4()
        self.type = artifact_type  # finger, sketched region, tangible, etc.
        self.interactive_region = None
        self.setParent(parent)

        self.last_intersections = []

        self.is_intersecting = False
        self.effect_eligibility = {}

        # raise exceptions in other methods if interactive region is None

    def set_interactive_region_from_ordered_point_set(self, points, effect, role, directionality, linkage=False, links=[]):
        self.interactive_region = InteractiveRegion(points, effect, role, directionality, self, linkage, links)

    def set_interactive_region_with_interactive_region(self, interactive_region):
        # check interactive region for required properties

        # needs a region
        # needs a mask
        # needs direction specification
        # linkage
        # effect
        # role

        self.interactive_region = interactive_region

    def set_interactive_region_linked(self, toggle, link_id):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            self.interactive_region.set_linked(toggle, link_id)

    def set_effect(self, effect):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            self.interactive_region.effect = effect

    def get_effect(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.effect

    def set_role(self, role):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            self.interactive_region.role = role

    def get_role(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.role

    def get_directionality(self, directionality):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            self.interactive_region.directionality = directionality

    def get_directionality(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.directionality

    def get_interactive_region(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region

    def get_interactive_region_aabb(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.get_aabb()

    def get_interactive_region_contour(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.get_contour()

    def move_interactive_region(self, vector):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            self.interactive_region.move(vector)

            if self.type == Artifact.ArtifactType.FILE.value:
                self.move_widget(vector)

    def is_linked(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.linkage

    def get_linked_artifacts(self):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.get_linked_artifact_ids()

    def remove_link_by_id(self, _id):
        if self.interactive_region is None:
            raise ValueError("Interactive Region is None")
        else:
            return self.interactive_region.remove_link_by_id(_id)

    def collides_with_point(self, p):
        return (self.interactive_region.collision_mask[int(p[1])][int(p[0])] == 1) if not (int(p[0]) > 1919 or int(p[0]) < 0 or int(p[1]) > 1079 or int(p[1]) < 0) else False

    def collides_with_other(self, other):
        if self.__collides_with_aabb(other):
            for p in other.get_interactive_region_contour():
                if int(p[0]) > 1919 or int(p[1]) > 1079:
                    continue

                if self.collides_with_point(p):
                    return True

        return False

    def get_last_intersections_list(self):
        return self.last_intersections

    def set_last_intersections_list(self, intersections):
        self.last_intersections = intersections

    def add_intersection_to_last_intersections_list(self, intersection):
        self.last_intersections.append(intersection)

    def clear_last_intersections_list(self):
        self.last_intersections = []

    def __collides_with_aabb(self, other):
        aabb1 = self.get_interactive_region_aabb()
        aabb2 = other.get_interactive_region_aabb()

        return aabb1[0][0] < aabb2[0][0] + aabb2[3][0] - aabb2[0][0] and \
               aabb1[0][0] + aabb1[3][0] - aabb1[0][0] > aabb2[0][0] and \
               aabb1[0][1] < aabb2[0][1] + aabb2[1][1] - aabb2[0][1] and \
               aabb1[0][1] + aabb1[1][1] - aabb1[0][1] > aabb2[0][1]

    def on_emit_effect(self):
        if self.is_intersecting:
            print("emit effect if able")

        # unsure on how to use this
        # can be used to do stuff before the effect is emitted if needed
        # actual emission is in conjunction with the on_receive_effect() function where effects are actually applied and transfered

    def on_receive_effect(self, source_artifact_index):
        if source_artifact_index == -1:
            source_artifact = self.parent().mouse_cursor
        else:
            source_artifact = self.parent().sketch_artifacts[source_artifact_index]

        _id = source_artifact.id
        effect = source_artifact.get_effect()
        role = source_artifact.get_role()
        directionality = source_artifact.get_directionality()
        _type = source_artifact.type
        linked = source_artifact.is_linked()
        links = source_artifact.get_linked_artifacts()
        emits = source_artifact.effect_eligibility["emit"]
        receptions = source_artifact.effect_eligibility["receive"]

        if _type == Artifact.ArtifactType.CURSOR.value:
            if self.id in links:
                if role == InteractiveRegion.RoleType.MOVE.value:
                    if InteractiveRegion.EffectType.MOVE.value in emits and InteractiveRegion.EffectType.MOVE.value in self.effect_eligibility["receive"]:
                        self.move_interactive_region(self.parent().get_mouse_cursor_delta())

        elif _type == Artifact.ArtifactType.SKETCH.value:
            if self.is_intersecting:
                print("receive effect if able")
        else:
            raise TypeError("Currently not supported artifact type %", _type)


    def on_intersection(self):
        if not self.is_intersecting:
            self.is_intersecting = True
            print("intersection happening")

    def on_disjunction(self):
        if self.is_intersecting:
            self.is_intersecting = False
            print("intersection lifted")


class Sketch(Artifact):
    def __init__(self, parent):
        super(Sketch, self).__init__(Artifact.ArtifactType.SKETCH.value, parent)

        self.effect_eligibility = {
            "emit": [],
            "receive": [
                InteractiveRegion.EffectType.MOVE.value
            ]
        }


class Cursor(Artifact):
    def __init__(self, x, y, parent):
        super(Cursor, self).__init__(Artifact.ArtifactType.CURSOR.value, parent)

        self.left_clicked = False
        self.right_clicked = False

        self.pressed_moved = False

        self.current_position = None
        self.last_position = None

        self.__create_interactive_region(x, y, 8)

        self.effect_eligibility = {
            "emit": [
                InteractiveRegion.EffectType.MOVE.value,
                InteractiveRegion.EffectType.SKETCH.value],
            "receive": [

            ]
        }

    def set_current_position(self, p):
        self.current_position = p

    def get_current_position(self):
        return self.current_position

    def set_last_position(self, p):
        self.last_position = p

    def get_last_position(self):
        return self.last_position

    def __create_interactive_region(self, x, y, radius):
        temp1 = []
        temp2 = []

        for x_ in range(-radius, radius):
            y_ = int(math.sqrt(int(radius * radius) - x_ * x_) + 0.5)

            temp1.append((x + x_, y + y_))
            temp2.append((x + x_, y - y_))

        self.set_interactive_region_from_ordered_point_set(temp1 + list(reversed(temp2)),
                                                           InteractiveRegion.EffectType.NONE.value,
                                                           InteractiveRegion.RoleType.BRUSH.value,
                                                           InteractiveRegion.Direction.UNIDIRECTIONAL.value, True,
                                                           [str(self.id)])


class Tangible(Artifact):
    def __init__(self, parent):
        super(Tangible, self).__init__(Artifact.ArtifactType.TANGIBLE.value, parent)


class Finger(Artifact):
    def __init__(self, parent):
        super(Finger, self).__init__(Artifact.ArtifactType.FINGER.value, parent)


class File(Artifact):
    class FileType(Enum):
        TEXT = 0
        IMAGE = 1

    def __init__(self, _id, x, y, _type, parent):
        super(File, self).__init__(Artifact.ArtifactType.FILE.value, parent)

        self.default_width_icon = 65
        self.default_height_icon = 92

        self.widget = QtWidgets.QLabel()
        self.widget.setParent(parent)
        self.widget.setGeometry(x, y, self.default_width_icon, self.default_height_icon)

        self.widget.setAttribute(QtCore.Qt.WA_TranslucentBackground)
        self.widget.setWindowFlags(QtCore.Qt.WindowStaysOnTopHint | QtCore.Qt.FramelessWindowHint)

        self.widget.setScaledContents(False)

        if _type == File.FileType.TEXT.value:
            self.widget.setPixmap(QtGui.QPixmap('res/img/file_icon.png').scaledToWidth(self.default_width_icon))
        elif _type == File.FileType.IMAGE.value:
            # programmatically get dimension of image and scale it accordingly for icon and preview
            # self.widget.setPixmap(QtGui.QPixmap('res/img/file_icon.png').scaledToWidth(self.default_width_icon))
            pass
        else:
            raise ValueError("Currently unsupported file type")

        self.set_interactive_region_from_ordered_point_set([(x, y), (x, y + self.default_height_icon), (x + self.default_width_icon, y + self.default_height_icon), (x + self.default_width_icon, y)],
                                                           InteractiveRegion.EffectType.NONE.value,
                                                           InteractiveRegion.RoleType.NONE.value,
                                                           InteractiveRegion.Direction.UNIDIRECTIONAL.value)

        self.effect_eligibility = {
            "emit": [],
            "receive": [InteractiveRegion.EffectType.MOVE.value]
        }

    def move_widget(self, vector):
        self.widget.setGeometry(self.widget.x() + vector[0], self.widget.y() + vector[1], self.widget.width(), self.widget.height())
