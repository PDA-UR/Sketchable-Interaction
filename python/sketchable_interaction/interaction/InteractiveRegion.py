
from enum import Enum
import numpy as np
from PIL import Image, ImageDraw


class InteractiveRegion:
    class Direction(Enum):
        UNIDIRECTIONAL = 0
        BIDIRECTIONAL = 1

    class EffectType(Enum):
        NONE = 0

    class RoleType(Enum):
        NONE = 0

    def __init__(self, point_set, effect, role, directionality, parent_artifact, linkage, links):
        self.point_set = point_set + [point_set[0]]
        self.aabb = self.__compute_aabb()
        self.center = self.__compute_center()
        self.effect = effect
        self.role = role
        self.directionality = directionality
        self.parent_artifact = parent_artifact
        self.linkage = linkage
        self.links = links

        self.collision_mask = Mask.create_mask(self.point_set)

    def get_contour(self):
        return self.point_set

    def get_aabb(self):
        return self.aabb

    def move(self, vector):
        for i in range(len(self.point_set)):
            self.point_set[i] = (self.point_set[i][0] + vector[0], self.point_set[i][1] + vector[1])

        self.aabb = self.__compute_aabb()
        self.center = self.__compute_center()
        self.collision_mask = Mask.create_mask(self.point_set)

    def __compute_aabb(self):
        if len(self.point_set) > 0:
            minx, miny = float("inf"), float("inf")
            maxx, maxy = float("-inf"), float("-inf")

            for x, y in self.point_set:
                if x < minx:
                    minx = x
                if y < miny:
                    miny = y
                if x > maxx:
                    maxx = x
                elif y > maxy:
                    maxy = y

        return [(minx, miny), (minx, maxy), (maxx, maxy), (maxx, miny)]

    def __compute_center(self):
        x_values = [vertex[0] for vertex in self.point_set]
        y_values = [vertex[1] for vertex in self.point_set]

        return sum(x_values) / len(self.point_set), sum(y_values) / len(self.point_set)


class Mask:

    @staticmethod
    def create_mask(contour):
        img = Image.new('L', (1920, 1080), 0)

        ImageDraw.Draw(img).polygon(contour, 1, 1)

        return np.array(img)
