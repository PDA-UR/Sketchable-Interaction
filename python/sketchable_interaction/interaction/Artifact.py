
from enum import Enum
from sketchable_interaction.interaction.InteractiveRegion import InteractiveRegion
import uuid
import math
from PyQt5 import QtCore


class Artifact(QtCore.QObject):
    class ArtifactType(Enum):
        SKETCH = 0
        FINGER = 1
        TANGIBLE = 2
        SMARTPHONE = 3
        CURSOR = 4

    def __init__(self, artifact_type):
        super(Artifact, self).__init__()
        self.id = uuid.uuid4()
        self.type = artifact_type  # finger, sketched region, tangible, etc.
        self.interactive_region = None

        self.last_intersections = []

        self.is_intersecting = False

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
        print("emit effect if able")

    def on_receive_effect(self):
        if self.is_intersecting:
            print("receive effect if able")

    def on_intersection(self):
        if not self.is_intersecting:
            self.is_intersecting = True
            print("intersection happening")

    def on_disjunction(self):
        if self.is_intersecting:
            self.is_intersecting = False
            print("intersection lifted")


class Sketch(Artifact):
    def __init__(self):
        super(Sketch, self).__init__(Artifact.ArtifactType.SKETCH.value)


class Cursor(Artifact):
    def __init__(self, x, y):
        super(Cursor, self).__init__(Artifact.ArtifactType.CURSOR.value)

        self.left_clicked = False
        self.right_clicked = False

        self.pressed_moved = False

        self.current_position = None
        self.last_position = None

        self.__create_interactive_region(x, y, 8)

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
                                                           InteractiveRegion.EffectType.SKETCH.value,
                                                           InteractiveRegion.RoleType.BRUSH.value,
                                                           InteractiveRegion.Direction.UNIDIRECTIONAL.value, True,
                                                           [str(self.id)])


class Tangible(Artifact):
    def __init__(self):
        super(Tangible, self).__init__(Artifact.ArtifactType.TANGIBLE.value)


class Finger(Artifact):
    def __init__(self):
        super(Finger, self).__init__(Artifact.ArtifactType.FINGER.value)
