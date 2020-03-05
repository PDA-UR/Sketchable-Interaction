from libPySI import PySIEffect, PySICapability
import Entry


class Directory(Entry.Entry):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Directory, self).__init__(shape, aabb, uuid, kwargs)
        self.name = "stdSIDir"
        self.region_type = PySIEffect.EffectType.SI_DIRECTORY
        self.qml_path = "plugins/standard_environment_library/filesystem/Directory.qml"
        self.preview_width = 400
        self.preview_height = 600
        self.is_icon_visible = True
        self.is_opened_visible = False
        self.children_paths = list(kwargs["children"]) if len(kwargs.keys()) else []
        self.children = []
        self.num_children_per_page = 6

        self.children_paths.sort()
        self.current_page = 0

        self.browse_pages = []
        for i in range(len(self.children_paths)):
            if i % self.num_children_per_page == 0:
                self.browse_pages.append(PySIEffect.StringVector())

            self.browse_pages[-1].append(self.children_paths[i])

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
        self.add_data("page_name", "1 / " + str(len(self.browse_pages)), PySIEffect.DataType.STRING)

        if not self.is_child:
            self.cap_emit["PARENT"] = {"on_enter": self.on_child_enter_emit, "on_continuous": None, "on_leave": self.on_child_leave_emit}

        self.cap_recv["BTN"] = {"on_enter": self.on_btn_enter_recv, "on_continuous": self.on_btn_continuous_recv, "on_leave": self.on_btn_leave_recv}
        self.cap_recv["OPEN_ENTRY"] = {"on_enter": self.on_open_entry_enter_recv, "on_continuous": self.on_open_entry_continuous_recv, "on_leave": self.on_open_entry_leave_recv}

        self.cap_link_emit["__position__"] = self.position

    def on_child_enter_emit(self, child):
        return self._uuid

    def on_child_leave_emit(self, child):
        return self._uuid

    def set_folder_contents_page(self, value):
        self.btn_presses = self.btn_presses - 1 if value else self.btn_presses + 1
        self.current_page = self.btn_presses % len(self.browse_pages)

        if self.btn_presses % len(self.browse_pages) is 0:
            self.btn_presses = 0

    def position(self):
        return self.x - self.last_x, self.y - self.last_y

    def on_btn_trigger(self, sender, value):
        self.set_folder_contents_page(value)

        for child in self.children:
            if child.region_type is not int(PySIEffect.EffectType.SI_BUTTON):
                child.signal_deletion()

        self.add_data("page_name", str(self.current_page + 1) + "/" + str(len(self.browse_pages)), PySIEffect.DataType.STRING)

        self.show_folder_contents_page(self.browse_pages[self.current_page], self._uuid, False)

    def on_btn_enter_recv(self, cursor_id, link_attrib):
        return 0

    def on_btn_continuous_recv(self, cursor_id, value):
        if cursor_id is not "" and value is not "":
            self.on_btn_trigger(cursor_id, value)

        return 0

    def on_btn_leave_recv(self, cursor_id, link_attrib):
        return 0

    def on_open_entry_enter_recv(self):
        return 0

    def on_open_entry_continuous_recv(self):
        if not self.is_child and not self.is_open_entry_capability_blocked and not self.is_under_user_control:
            x = self.aabb[0].x
            y = self.aabb[0].y

            self.width = self.preview_width
            self.height = self.preview_height

            self.shape = PySIEffect.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])
            self.notify_shape_changed(True)

            self.is_icon_visible = False
            self.is_opened_visible = True

            self.color = PySIEffect.Color(10, 0, 0, 255)
            self.add_data("container_width", self.width, PySIEffect.DataType.INT)
            self.add_data("container_height", self.height, PySIEffect.DataType.INT)
            self.add_data("is_icon_visible", self.is_icon_visible, PySIEffect.DataType.BOOL)
            self.add_data("is_opened_visible", self.is_opened_visible, PySIEffect.DataType.BOOL)
            self.show_folder_contents_page(self.browse_pages[self.current_page], self._uuid, True)

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
            self.color = PySIEffect.Color(25, 0, 0, 0)
            self.add_data("container_width", self.width, PySIEffect.DataType.INT)
            self.add_data("container_height", self.height, PySIEffect.DataType.INT)
            self.add_data("is_icon_visible", self.is_icon_visible, PySIEffect.DataType.BOOL)
            self.add_data("is_opened_visible", self.is_opened_visible, PySIEffect.DataType.BOOL)
            self.is_open_entry_capability_blocked = False

            for child in self.children:
                child.signal_deletion()

        return 0
