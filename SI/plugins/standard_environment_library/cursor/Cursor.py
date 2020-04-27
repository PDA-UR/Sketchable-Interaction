from libPySI import PySIEffect
from SI.plugins.standard_environment_library import SIEffect


class MouseCursor(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(MouseCursor, self).__init__(shape, aabb, uuid, "", kwargs)
        self.name = PySIEffect.SI_STD_NAME_MOUSE_CURSOR
        self.region_type = PySIEffect.EffectType.SI_MOUSE_CURSOR
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/cursor/Cursor.qml"
        self.color = PySIEffect.Color(0, 0, 0, 0)

        self.width = 18
        self.height = 24

        self.clicks = 0

        self.add_QML_data("width", self.width, PySIEffect.DataType.INT)
        self.add_QML_data("height", self.height, PySIEffect.DataType.INT)

        self.parent_canvas = None
        self.move_target = None
        self.btn_taget = None

        self.disable_effect(PySIEffect.DELETION, self.RECEPTION)

        self.enable_link_emission(PySIEffect.POSITION, self.position)
        self.enable_link_reception(PySIEffect.POSITION, PySIEffect.POSITION, self.set_position_from_position)

    def position(self):
        return self.x - self.last_x, self.y - self.last_y, self.x, self.y

    def set_position_from_position(self, rel_x, rel_y, abs_x, abs_y):
        self.last_x = self.x
        self.last_y = self.y
        self.x = abs_x
        self.y = abs_y

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
            return self._uuid, PySIEffect.POSITION

        return "", ""

    def on_move_continuous_emit(self, other):
        pass

    def on_move_leave_emit(self, other):
        if self.move_target is other:
            self.move_target = None
            return self._uuid, PySIEffect.POSITION

        return "", ""

    def on_btn_press_enter_emit(self, other):
        if self.btn_taget is None:
            self.btn_taget = other

    def on_btn_press_continuous_emit(self, other):
        pass

    def on_btn_press_leave_emit(self, other):
        if self.btn_taget is other:
            self.btn_taget = None

    def on_left_mouse_click(self, is_active):
        if is_active:
            if PySIEffect.CLICK not in self.cap_emit.keys():
                self.enable_effect(PySIEffect.CLICK, True, self.on_btn_press_enter_emit, self.on_btn_press_continuous_emit, self.on_btn_press_leave_emit)

            if PySIEffect.SKETCH not in self.cap_emit.keys():
                self.enable_effect(PySIEffect.SKETCH, True, self.self_on_sketch_enter_emit, self.on_sketch_continuous_emit, self.on_sketch_leave_emit)
        else:
            if PySIEffect.SKETCH in self.cap_emit.keys():
                self.disable_effect(PySIEffect.SKETCH, True)

                if self.parent_canvas is not None:
                    self.parent_canvas.on_sketch_leave_recv(*self.on_sketch_leave_emit(self.parent_canvas))
                self.parent_canvas = None

            if PySIEffect.CLICK in self.cap_emit.keys():
                self.disable_effect(PySIEffect.CLICK, True)
                if self.btn_taget is not None:
                    self.btn_taget.on_click_leave_recv()

    def on_right_mouse_click(self, is_active):
        if is_active:
            if PySIEffect.MOVE not in self.cap_emit.keys():
                self.enable_effect(PySIEffect.MOVE, True, self.on_move_enter_emit, self.on_move_continuous_emit, self.on_move_leave_emit)
        elif PySIEffect.MOVE in self.cap_emit.keys():
            self.disable_effect(PySIEffect.MOVE, True)
            if self.move_target is not None:
                self.move_target.on_move_leave_recv(*self.on_move_leave_emit(self.move_target))
