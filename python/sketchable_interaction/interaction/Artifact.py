
from enum import Enum
from sketchable_interaction.interaction.InteractiveRegion import InteractiveRegion
import uuid


class Artifact:
    class ArtifactType(Enum):
        SKETCH = 0
        FINGER = 1
        TANGIBLE = 2
        SMARTPHONE = 3
        CURSOR = 4

    def __init__(self, artifact_type):
        self.id = uuid.uuid4()
        self.type = artifact_type  # finger, sketched region, tangible, etc.
        self.interactive_region = None

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

    def collides_with_point(self, p):
        if int(p[0]) > 1919 or int(p[0]) < 0 or int(p[1]) > 1079 or int(p[1]) < 0:
            return False

        return self.interactive_region.collision_mask[int(p[1])][int(p[0])] == 1

    def collides_with_other(self, other):
        if self.__collides_with_aabb(other):
            for p in other.get_interactive_region_contour():
                if int(p[0]) > 1919 or int(p[1]) > 1079:
                    continue

                if self.collides_with_point(p):
                    return True

        return False

    def __collides_with_aabb(self, other):
        aabb1 = self.get_interactive_region_aabb()
        aabb2 = other.get_interactive_region_aabb()

        return aabb1[0][0] < aabb2[0][0] + aabb2[3][0] - aabb2[0][0] and \
               aabb1[0][0] + aabb1[3][0] - aabb1[0][0] > aabb2[0][0] and \
               aabb1[0][1] < aabb2[0][1] + aabb2[1][1] - aabb2[0][1] and \
               aabb1[0][1] + aabb1[1][1] - aabb1[0][1] > aabb2[0][1]


class Sketch(Artifact):
    def __init__(self):
        super(Sketch, self).__init__(Artifact.ArtifactType.SKETCH.value)


class Cursor(Artifact):
    def __init__(self):
        super(Cursor, self).__init__(Artifact.ArtifactType.CURSOR.value)

        self.left_clicked = False
        self.right_clicked = False

        self.pressed_moved = False


class Tangible(Artifact):
    def __init__(self):
        super(Tangible, self).__init__(Artifact.ArtifactType.TANGIBLE.value)


class Finger(Artifact):
    def __init__(self):
        super(Finger, self).__init__(Artifact.ArtifactType.FINGER.value)
