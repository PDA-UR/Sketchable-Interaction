from libPySI import PySIEffect, PySICapability


class Directory(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Directory, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid

        self.name = "stdSIDir"
        self.region_type = PySIEffect.EffectType.SI_DIRECTORY
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/filesystem/Directory.qml"
        self.color = PySIEffect.Color(25, 0, 0, 0)
        self.icon_width = 65
        self.icon_height = 75
        self.text_height = 50
        self.preview_width = 400
        self.preview_height = 600
        self.is_visible = True
        self.is_icon_visible = True
        self.is_opened_visible = False
        self.text_color ="#FFFFFFFF"
        self.path = kwargs["cwd"] if len(kwargs.keys()) else ""
        self.children_paths = list(kwargs["children"]) if len(kwargs.keys()) else []
        self.is_child = bool(kwargs["is_child"]) if len(kwargs.keys()) else False
        self.children = []

        self.is_open_entry_capability_blocked = False

        self.children_paths.sort()
        self.current_page = 0
        self.width = self.icon_width * 2
        self.height = self.icon_height + self.text_height
        self.last_x = 0
        self.last_y = 0
        self.filename = ""

        if self.path is not "":
            self.filename = self.path[self.path.rfind("/") + 1:]

        self.browse_pages = []
        for i in range(len(self.children_paths)):
            if i % 6 == 0:
                self.browse_pages.append(PySIEffect.StringVector())

            self.browse_pages[-1].append(self.children_paths[i])

        self.last_triggered = False
        self.btn_presses = 0

        self.add_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_data("text_height", self.text_height, PySIEffect.DataType.INT)
        self.add_data("container_width", self.width, PySIEffect.DataType.INT)
        self.add_data("container_height", self.height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/dir.png", PySIEffect.DataType.STRING)
        self.add_data("color", self.text_color, PySIEffect.DataType.STRING)
        self.add_data("fullname", self.path, PySIEffect.DataType.STRING)
        self.add_data("name", self.filename, PySIEffect.DataType.STRING)
        self.add_data("is_visible", self.is_visible, PySIEffect.DataType.BOOL)
        self.add_data("is_icon_visible", self.is_icon_visible, PySIEffect.DataType.BOOL)
        self.add_data("is_opened_visible", self.is_opened_visible, PySIEffect.DataType.BOOL)
        self.add_data("page_name", "0 / 0", PySIEffect.DataType.STRING)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map({

        })

        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map({
            "MOVE": {"on_enter": self.on_move_enter_recv, "on_continuous": self.on_move_continuous_recv, "on_leave": self.on_move_leave_recv},
            "BTN_PRESS": {"on_enter": self.on_btn_press_enter_recv, "on_continuous": self.on_btn_press_continuous_recv, "on_leave": self.on_btn_press_leave_recv},
            "OPEN_ENTRY": {"on_enter": self.on_open_entry_enter_recv, "on_continuous": None, "on_leave": self.on_open_entry_leave_recv}
        })

        self.cap_link_emit = PySIEffect.String2FunctionMap({
            "__position__": self.position
        })

        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map({
            "__position__": {"__position__": self.set_position_from_position},
        })

    def set_folder_contents_page(self, value):
        if value:
            self.btn_presses += 1
        else:
            self.btn_presses -= 1

        page = self.btn_presses % len(self.browse_pages)

        if page is 0:
            self.btn_presses = 0

        self.current_page = page

    def position(self):
        return self.x - self.last_x, self.y - self.last_y

    def set_position_from_position(self, rel_x, rel_y):
        self.last_x = self.x
        self.last_y = self.y

        self.x += rel_x
        self.y += rel_y
        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])

        return 0

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

        if lr in self.link_relations:
            del self.link_relations[self.link_relations.index(lr)]

        return 0

    def on_btn_press_enter_recv(self, triggered, value):
        return 0

    def on_btn_press_continuous_recv(self, triggered, value):
        if triggered and not self.last_triggered:
            self.set_folder_contents_page(value)

            self.add_data("page_name", str(self.current_page + 1) + " / " + str(len(self.browse_pages)), PySIEffect.DataType.STRING)
            self.last_triggered = True
        elif not triggered:
            self.last_triggered = False

        return 0

    def on_btn_press_leave_recv(self, triggered, value):
        return 0

    def on_open_entry_enter_recv(self):
        if not self.is_child and not self.is_open_entry_capability_blocked:
            x = self.aabb[0].x
            y = self.aabb[0].y

            self.width = self.preview_width
            self.height = self.preview_height

            self.shape = PySIEffect.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])
            self.notify_shape_changed(True)

            self.is_icon_visible = False
            self.is_opened_visible = True

            self.color = PySIEffect.Color(255, 0, 0, 255)
            self.add_data("container_width", self.width, PySIEffect.DataType.INT)
            self.add_data("container_height", self.height, PySIEffect.DataType.INT)
            self.add_data("is_icon_visible", self.is_icon_visible, PySIEffect.DataType.BOOL)
            self.add_data("is_opened_visible", self.is_opened_visible, PySIEffect.DataType.BOOL)

            self.show_folder_contents_page(self.browse_pages[self.current_page], self._uuid)

            self.is_open_entry_capability_blocked = True

        return 0

    def on_open_entry_leave_recv(self):
        if not self.is_child and self.is_open_entry_capability_blocked:

            x = self.aabb[0].x
            y = self.aabb[0].y

            self.width = self.icon_width * 2
            self.height = self.icon_height + self.text_height

            self.shape = PySIEffect.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])
            self.notify_shape_changed(True)

            self.is_icon_visible = True
            self.is_opened_visible = False
            self.color = PySIEffect.Color(10, 0, 0, 0)
            self.add_data("container_width", self.width, PySIEffect.DataType.INT)
            self.add_data("container_height", self.height, PySIEffect.DataType.INT)
            self.add_data("is_icon_visible", self.is_icon_visible, PySIEffect.DataType.BOOL)
            self.add_data("is_opened_visible", self.is_opened_visible, PySIEffect.DataType.BOOL)
            self.is_open_entry_capability_blocked = False

            for child in self.children:
                child.signal_deletion()

        return 0
