
from enum import Enum
from sketchable_interaction.interaction.InteractiveRegion import InteractiveRegion
import uuid
import math
from PyQt5 import QtCore, QtWidgets, QtGui
import vectormath as vm


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
        self.effect_color = QtGui.QColor(0, 0, 0)
        self.is_child = False
        self.is_palette_parent_sketch = False

        # raise exceptions in other methods if interactive region is None

    def get_id(self):
        return self.id

    def compute_center_of_polygon(self, polygon):
        if len(polygon) > 0:
            x_values = [vertex[0] for vertex in polygon]
            y_values = [vertex[1] for vertex in polygon]

            return sum(x_values) / len(polygon), sum(y_values) / len(polygon)

        return -1, -1

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

    def set_parent_uuid(self, uuid):
        if self.is_child:
            self.parent_uuid = uuid

    def get_parent_uuid(self):
        return self.parent_uuid

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
        for p in other.get_interactive_region_contour():
            if int(p[0]) > 1919 or int(p[1]) > 1079:
                continue

            if self.collides_with_point(p):
                return True

        return False

    def collides_with_others_aabb(self, other):
        if self.__collides_with_aabb(other):
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
            pass

        # unsure on how to use this
        # can be used to do stuff before the effect is emitted if needed
        # actual emission is in conjunction with the on_receive_effect() function where effects are actually applied and transfered

    def on_receive_effect(self, source_artifact_index):
        if source_artifact_index == -1:
            source_artifact = self.parent().mouse_cursor
        else:
            source_artifact = self.parent().artifacts[source_artifact_index]

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
                pass
        elif _type == Artifact.ArtifactType.FILE.value:
            print("file collision")
        else:
            raise TypeError("Currently not supported artifact type", _type)

    def on_intersection(self):
        if not self.is_intersecting:
            self.is_intersecting = True
            print("intersection happening")

    def on_disjunction(self):
        if self.is_intersecting:
            self.is_intersecting = False
            print("intersection lifted")

    def render(self, painter):
        pass

    def poly(self, pts):
        return QtGui.QPolygonF(map(lambda p: QtCore.QPointF(*p), pts))


class Sketch(Artifact):
    def __init__(self, point_set, effect, role, directionality, linkage, links, parent, is_child=False):
        super(Sketch, self).__init__(Artifact.ArtifactType.SKETCH.value, parent)

        self.effect_type = effect
        self.role_type = role

        self.effect_eligibility = {
            "emit": [],
            "receive": [
                InteractiveRegion.EffectType.MOVE.value,
                InteractiveRegion.EffectType.EFFECT_PALETTE.value
            ]
        }

        #self.center = self.__compute_center_of_polygon(point_set)
        self.is_child = is_child

        self.set_interactive_region_from_ordered_point_set(point_set,
                                                           effect,
                                                           role,
                                                           directionality, linkage,
                                                           links)

    def render(self, painter):
        painter.drawPolyline(self.poly(self.get_interactive_region_contour()))

        path = QtGui.QPainterPath()
        first_point = self.get_interactive_region_contour()[0]
        path.moveTo(first_point[0], first_point[1])

        for point in self.get_interactive_region_contour()[1:]:
            path.lineTo(point[0], point[1])

        path.lineTo(first_point[0], first_point[1])

        painter.fillPath(path, self.effect_color)


"""
Here are the classes derived from sketch; potentially required in this way for better decomposition and adding of effects at runtime
"""


class EffectPaletteSketch(Sketch):
    class TransferEffectTypes(Enum):  # standard library effects
        DELETE = InteractiveRegion.EffectType.DELETE.value
        PREVIEW = InteractiveRegion.EffectType.PREVIEW.value
        TAG = InteractiveRegion.EffectType.TAG.value

    class TransferEffectColor(Enum):
        DELETE = QtGui.QColor("#BB3333")
        PREVIEW = QtGui.QColor("#33BB33")
        TAG = QtGui.QColor("#A05A09")

    def __init__(self, point_set, parent, is_child=False, child_num=-1):
        super(EffectPaletteSketch, self).__init__(point_set, InteractiveRegion.EffectType.EFFECT_PALETTE.value, InteractiveRegion.RoleType.EFFECT_PALETTE.value, InteractiveRegion.Direction.UNIDIRECTIONAL.value, False, [], parent, is_child)

        self.effect_collection = EffectPaletteSketch.TransferEffectTypes
        self.effect_collection_colors = EffectPaletteSketch.TransferEffectColor
        self.effect_collection_size = len(EffectPaletteSketch.TransferEffectTypes)

        self.sub_point_sets = []
        self.parent_uuid = -1
        self.point_resampling_size = 84
        self.effect_palette_modulo_divisor = self.point_resampling_size / self.effect_collection_size
        self.is_child = is_child
        self.center = self.compute_center_of_polygon(point_set)
        self.transfer_effect = None

        if is_child:
            self.is_palette_parent_sketch = False

            if child_num != -1:
                for i, n in enumerate(self.effect_collection_colors):
                    if child_num == i:
                        self.effect_color = n.value

                for i, n in enumerate(self.effect_collection):
                    if child_num == i:
                        self.transfer_effect = n.value
        else:
            self.is_palette_parent_sketch = True

        self.__create_effect_palette(point_set)

    # add a new type to the enum at runtime
    def add_transfer_type_to_enum(self, _type_name, color):
        names = [m.name for m in self.effect_collection] + [_type_name]

        self.effect_collection = Enum('TransferEffectTypes', names)

        self.effect_palette_modulo_divisor = self.point_resampling_size / len(names)

        names = [m.name for m in self.effect_collection_colors] + [_type_name]
        values = [m.value for m in self.effect_collection_colors] + [color]

        d = {}

        for i in range(len(self.effect_collection_colors)):
            d[names[i]] = values[i]

        self.effect_collection_colors = Enum('TransferEffectColors', d)

    def get_sub_point_sets(self):
        return self.sub_point_sets

    def __create_effect_palette(self, points):
        if not self.is_child:
            points = self.__resample_points(self.__compute_circle(self.center[0], self.center[1], int(sum((vm.Vector2(p[0], p[1]) - vm.Vector2(self.center[0], self.center[1])).length for p in points)) / len(points)), self.point_resampling_size)

            self.set_interactive_region_from_ordered_point_set(points, self.effect_type, self.role_type, InteractiveRegion.Direction.UNIDIRECTIONAL.value)

            if len(self.get_interactive_region_contour()) > 0:
                temp = [self.get_interactive_region_contour()[0]]

                for i, p in enumerate(self.get_interactive_region_contour()[1:]):
                    temp.append(p)

                    if i > 0 and i % self.effect_palette_modulo_divisor == 0:
                        self.sub_point_sets.append([self.center] + temp + [self.center])
                        temp = [p]

                self.sub_point_sets.append([self.center] + temp + [self.center])

            self.is_palette_parent_sketch = True
        else:
            self.set_interactive_region_from_ordered_point_set(points, self.effect_type, self.role_type, InteractiveRegion.Direction.UNIDIRECTIONAL.value)

    def __compute_circle(self, x, y, r):
        temp1 = []
        temp2 = []

        for x_ in range(int(-r), int(r)):
            y_ = int(math.sqrt(int(r * r) - x_ * x_) + 0.5)

            temp1.append((x + x_, y + y_))
            temp2.append((x + x_, y - y_))

        return temp1 + list(reversed(temp2))

    def __resample_points(self, points, num_desired_points=64):
        I = self.__path_length(points) / (num_desired_points - 1)
        D = 0.0

        new_points = [points[0]]

        i = 1

        while i < len(points):
            d = vm.Vector2(points[i - 1][0] - points[i][0], points[i - 1][1] - points[i][1]).length

            if (D + d) >= I:
                qx = points[i - 1][0] + ((I - D) / d) * (points[i][0] - points[i - 1][0])
                qy = points[i - 1][1] + ((I - D) / d) * (points[i][1] - points[i - 1][1])
                new_points.append((qx, qy))
                points.insert(i, (qx, qy))

                D = 0.0
            else:
                D += d

            i += 1

        if len(new_points) == num_desired_points - 1:
            new_points.append(points[-1])

        return new_points

    def __path_length(self, points):
        d = 0.0

        for i in range(1, len(points)):
            d += vm.Vector2(points[i - 1][0] - points[i][0], points[i - 1][1] - points[i][1]).length

        return d

    def on_emit_effect(self):
        print("emit override", self.transfer_effect)

    def on_receive_effect(self, source_artifact_index):
        if source_artifact_index == -1:
            source_artifact = self.parent().mouse_cursor
        else:
            source_artifact = self.parent().artifacts[source_artifact_index]

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

    def on_intersection(self):
        if not self.is_intersecting:
            print("intersect override")
            self.is_intersecting = True

    def on_disjunction(self):
        if self.is_intersecting:
            print("disjunct override")
            self.is_intersecting = False


class DeletionSketch(Sketch):
    def __init__(self, point_set, parent, is_child):
        super(DeletionSketch, self).__init__(point_set, InteractiveRegion.EffectType.DELETE.value, InteractiveRegion.RoleType.DELETE.value, InteractiveRegion.Direction.UNIDIRECTIONAL.value, False, [], parent, is_child)

        self.effect_color = QtGui.QColor("#BB3333")

        self.effect_eligibility = {
            "emit": [
                InteractiveRegion.EffectType.DELETE.value
            ],

            "receive": [

            ]
        }

    def on_emit_effect(self):
        print("delete emit override")

    def on_receive_effect(self, source_artifact_index):
        print("receive override")

    def on_intersection(self):
        if not self.is_intersecting:
            print("intersect override")
            self.is_intersecting = True

    def on_disjunction(self):
        if self.is_intersecting:
            print("disjunct override")
            self.is_intersecting = False

    @staticmethod
    def get_effect_color():
        return QtGui.QColor("#BB3333")


class PreviewSketch(Sketch):
    def __init__(self, point_set, parent, is_child):
        super(PreviewSketch, self).__init__(point_set, InteractiveRegion.EffectType.PREVIEW.value, InteractiveRegion.RoleType.PREVIEW.value, InteractiveRegion.Direction.UNIDIRECTIONAL.value, False, [], parent, is_child)

        self.effect_color = QtGui.QColor("#33BB33")

        self.effect_eligibility = {
            "emit": [
                InteractiveRegion.EffectType.PREVIEW.value
            ],

            "receive": [

            ]
        }

    def on_emit_effect(self):
        print("preview emit override")

    def on_receive_effect(self, source_artifact_index):
        print("receive override")

    def on_intersection(self):
        if not self.is_intersecting:
            print("intersect override")
            self.is_intersecting = True

    def on_disjunction(self):
        if self.is_intersecting:
            print("disjunct override")
            self.is_intersecting = False

    @staticmethod
    def get_effect_color():
        return QtGui.QColor("#33BB33")


class TagSketch(Sketch):
    def __init__(self, point_set, parent, is_child):
        super(TagSketch, self).__init__(point_set, InteractiveRegion.EffectType.TAG.value, InteractiveRegion.RoleType.TAG.value, InteractiveRegion.Direction.UNIDIRECTIONAL.value, False, [], parent, is_child)

        self.effect_color = QtGui.QColor("#A05A09")

        self.effect_eligibility = {
            "emit": [
                InteractiveRegion.EffectType.TAG.value
            ],

            "receive": [

            ]
        }

    def on_emit_effect(self):
        print("tag emit override")

    def on_receive_effect(self, source_artifact_index):
        print("receive override")

    def on_intersection(self):
        if not self.is_intersecting:
            print("intersect override")
            self.is_intersecting = True

    def on_disjunction(self):
        if self.is_intersecting:
            print("disjunct override")
            self.is_intersecting = False

    @staticmethod
    def get_effect_color():
        return QtGui.QColor("#A05A09")


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
                InteractiveRegion.EffectType.DELETE.value,
                InteractiveRegion.EffectType.PREVIEW.value,
                InteractiveRegion.EffectType.TAG.value
            ]
        }

        self.transferable_effect_type = InteractiveRegion.EffectType.NONE.value

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

    def on_emit_effect(self):
        if self.is_intersecting:
            pass

    def on_receive_effect(self, source_artifact_index):
        source_artifact = self.parent().artifacts[source_artifact_index]

        _id = source_artifact.id
        effect = source_artifact.get_effect()
        role = source_artifact.get_role()
        directionality = source_artifact.get_directionality()
        _type = source_artifact.type
        linked = source_artifact.is_linked()
        links = source_artifact.get_linked_artifacts()
        emits = source_artifact.effect_eligibility["emit"]
        receptions = source_artifact.effect_eligibility["receive"]

        if self.get_role() == InteractiveRegion.RoleType.BRUSH.value:
            self.set_effect(source_artifact.transfer_effect)

    def on_intersection(self):
        if not self.is_intersecting:
            self.is_intersecting = True
            print("intersection happening")

    def on_disjunction(self):
        if self.is_intersecting:
            self.is_intersecting = False
            print("intersection lifted")

    def render(self, painter):
        painter.drawPolyline(self.poly(self.get_interactive_region_contour()))


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
