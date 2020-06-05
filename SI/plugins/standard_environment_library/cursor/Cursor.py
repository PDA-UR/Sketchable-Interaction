from libPySI import PySI
from plugins.standard_environment_library import SIEffect


class Cursor(SIEffect.SIEffect):
    regiontype = PySI.EffectType.SI_MOUSE_CURSOR
    regionname = PySI.EffectName.SI_STD_NAME_MOUSE_CURSOR
    region_width = 18
    region_height = 24

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Cursor, self).__init__(shape, uuid, "", Cursor.regiontype, Cursor.regionname, kwargs)
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/cursor/Cursor.qml"
        self.color = PySI.Color(0, 0, 0, 0)
        self.assigned_effect = ""

        self.width = Cursor.region_width
        self.height = Cursor.region_height

        self.clicks = 0

        self.add_QML_data("width", self.width, PySI.DataType.INT)
        self.add_QML_data("height", self.height, PySI.DataType.INT)

        self.parent_canvas = None
        self.move_target = None
        self.btn_target = None

        self.left_mouse_active = False
        self.right_mouse_active = False

        self.disable_effect(PySI.CollisionCapability.DELETION, self.RECEPTION)

        self.enable_effect(PySI.CollisionCapability.ASSIGN, self.RECEPTION, None, self.on_assign_continuous_recv, None)

        self.enable_link_emission(PySI.LinkingCapability.POSITION, self.position)
        self.enable_link_reception(PySI.LinkingCapability.POSITION, PySI.LinkingCapability.POSITION, self.set_position_from_position)

    def position(self):
        return self.x - self.last_x, self.y - self.last_y, self.x, self.y

    def set_position_from_position(self, rel_x, rel_y, abs_x, abs_y):
        self.last_x = self.x
        self.last_y = self.y

        self.move(abs_x, abs_y)
        # return abs_x, abs_y

    def self_on_sketch_enter_emit(self, other):
        self.parent_canvas = other

        return 0, 0, self._uuid

    def on_sketch_continuous_emit(self, other):
        return self.x, self.y, self._uuid

    def on_sketch_leave_emit(self, other):
        self.parent_canvas = None

        return 0, 0, self._uuid

    def on_move_enter_emit(self, other):
        if self.move_target is None:
            self.move_target = other

        if self.move_target is other:
            return self._uuid, PySI.LinkingCapability.POSITION

        return "", ""

    def on_move_continuous_emit(self, other):
        pass

    def on_move_leave_emit(self, other):
        if self.move_target is other:
            self.move_target = None
            return self._uuid, PySI.LinkingCapability.POSITION

        return "", ""

    def on_btn_press_enter_emit(self, other):
        if self.btn_target is None:
            self.btn_target = other

    def on_btn_press_continuous_emit(self, other):
        pass

    def on_btn_press_leave_emit(self, other):
        if self.btn_target is other:
            self.btn_target = None

    def on_left_mouse_click(self, is_active):
        self.left_mouse_active = is_active

        if is_active:
            if PySI.CollisionCapability.CLICK not in self.cap_emit.keys():
                self.enable_effect(PySI.CollisionCapability.CLICK, True, self.on_btn_press_enter_emit, self.on_btn_press_continuous_emit, self.on_btn_press_leave_emit)

            if self.assigned_effect != "":
                if PySI.CollisionCapability.SKETCH not in self.cap_emit.keys():
                    self.enable_effect(PySI.CollisionCapability.SKETCH, True, self.self_on_sketch_enter_emit, self.on_sketch_continuous_emit, self.on_sketch_leave_emit)
        else:
            if PySI.CollisionCapability.SKETCH in self.cap_emit.keys():
                self.disable_effect(PySI.CollisionCapability.SKETCH, True)

                if self.parent_canvas is not None:
                    self.parent_canvas.on_sketch_leave_recv(*self.on_sketch_leave_emit(self.parent_canvas))
                self.parent_canvas = None

            if PySI.CollisionCapability.CLICK in self.cap_emit.keys():
                self.disable_effect(PySI.CollisionCapability.CLICK, True)
                if self.btn_target is not None:
                    self.btn_target.on_click_leave_recv()

    def on_right_mouse_click(self, is_active):
        self.right_mouse_active = is_active

        if is_active:
            if PySI.CollisionCapability.MOVE not in self.cap_emit.keys():
                self.enable_effect(PySI.CollisionCapability.MOVE, True, self.on_move_enter_emit, self.on_move_continuous_emit, self.on_move_leave_emit)
        elif PySI.CollisionCapability.MOVE in self.cap_emit.keys():
            self.disable_effect(PySI.CollisionCapability.MOVE, True)
            if self.move_target is not None:
                self.move_target.on_move_leave_recv(*self.on_move_leave_emit(self.move_target))

    def on_assign_continuous_recv(self, effect_to_assign, effect_display_name, kwargs):
        if self.left_mouse_active:
            if self.assigned_effect != effect_to_assign:
                self.assigned_effect = effect_to_assign
                self.assign_effect(self.assigned_effect, effect_display_name, kwargs)
