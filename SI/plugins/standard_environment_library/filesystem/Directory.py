from libPySI import PySI
from plugins.standard_environment_library.filesystem import Entry
from plugins.standard_environment_library.button import Button


region_type = PySI.EffectType.SI_DIRECTORY
region_name = PySI.EffectName.SI_STD_NAME_DIRECTORY
region_width = 130
region_height = 125


class Directory(Entry.Entry):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Directory, self).__init__(shape, uuid, kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_DIRECTORY
        self.region_type = PySI.EffectType.SI_DIRECTORY
        self.qml_path = "plugins/standard_environment_library/filesystem/Directory.qml"
        self.preview_width = 400
        self.preview_height = 600
        self.width = region_width
        self.height = region_height
        self.is_icon_visible = True
        self.is_opened_visible = False
        self.children_paths_and_types = [(t[0], t[1] if t[1] != int(PySI.EffectType.SI_UNKNOWN_FILE) else int(PySI.EffectType.SI_TEXT_FILE)) for t in kwargs["children"]]

        self.children = []
        self.num_children_per_page = 6

        self.children_paths_and_types.sort()
        self.current_page = 0

        self.browse_pages = []
        for i in range(len(self.children_paths_and_types)):
            if i % self.num_children_per_page == 0:
                self.browse_pages.append([])

            self.browse_pages[-1].append(self.children_paths_and_types[i])

        self.btn_presses = 0

        self.add_QML_data("container_width", self.width, PySI.DataType.INT)
        self.add_QML_data("container_height", self.height, PySI.DataType.INT)
        self.add_QML_data("img_path", "res/dir.png", PySI.DataType.STRING)
        self.add_QML_data("fullname", self.path, PySI.DataType.STRING)
        self.add_QML_data("is_visible", self.is_visible, PySI.DataType.BOOL)
        self.add_QML_data("is_icon_visible", self.is_icon_visible, PySI.DataType.BOOL)
        self.add_QML_data("is_opened_visible", self.is_opened_visible, PySI.DataType.BOOL)
        self.add_QML_data("page_name", "1 / " + str(len(self.browse_pages)), PySI.DataType.STRING)

        self.enable_effect(PySI.CollisionCapability.PARENT, self.EMISSION, self.on_parent_enter_emit, None, self.on_parent_leave_emit)
        self.enable_effect(PySI.CollisionCapability.BTN, self.RECEPTION, self.on_btn_enter_recv, self.on_btn_continuous_recv, self.on_btn_leave_recv)
        self.enable_effect(PySI.CollisionCapability.OPEN_ENTRY, self.RECEPTION, self.on_open_entry_enter_recv, self.on_open_entry_continuous_recv, self.on_open_entry_leave_recv)

        self.enable_link_emission(PySI.LinkingCapability.POSITION, self.position)
        self.is_open_entry_capability_blocked = False

    def set_folder_contents_page(self, value):
        self.btn_presses = self.btn_presses - 1 if value else self.btn_presses + 1
        self.current_page = self.btn_presses % len(self.browse_pages)

        if self.btn_presses % len(self.browse_pages) is 0:
            self.btn_presses = 0

    def position(self):
        x = self.x - self.last_x
        y = self.y - self.last_y

        self.last_x = self.x
        self.last_y = self.y

        return x, y, self.x, self.y

    def on_btn_trigger(self, sender, value):
        self.set_folder_contents_page(value)

        for child in self.children:
            child.delete()

        self.add_QML_data("page_name", str(self.current_page + 1) + "/" + str(len(self.browse_pages)), PySI.DataType.STRING)

        self.show_current_folder_contents_page()

    def on_btn_enter_recv(self, cursor_id, link_attrib):
        pass

    def on_btn_continuous_recv(self, cursor_id, value):
        if cursor_id is not "" and value is not "":
            self.on_btn_trigger(cursor_id, value)

    def on_btn_leave_recv(self, cursor_id, link_attrib):
        pass

    def on_open_entry_enter_recv(self, is_other_controlled):
        pass

    def on_open_entry_continuous_recv(self, is_other_controlled):
        if self.parent == "" and not self.is_open_entry_capability_blocked and not self.is_under_user_control and not is_other_controlled:
            x = self.relative_x_pos()
            y = self.relative_y_pos()

            self.width = self.preview_width
            self.height = self.preview_height

            self.shape = PySI.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])

            self.is_icon_visible = False
            self.is_opened_visible = True

            self.color = PySI.Color(10, 0, 0, 255)
            self.add_QML_data("container_width", self.width, PySI.DataType.INT)
            self.add_QML_data("container_height", self.height, PySI.DataType.INT)
            self.add_QML_data("is_icon_visible", self.is_icon_visible, PySI.DataType.BOOL)
            self.add_QML_data("is_opened_visible", self.is_opened_visible, PySI.DataType.BOOL)

            self.show_current_folder_contents_page()

            self.is_open_entry_capability_blocked = True

    def on_open_entry_leave_recv(self, is_other_controlled):
        # if self.parent == "" and self.is_open_entry_capability_blocked:
            x = self.relative_x_pos()
            y = self.relative_y_pos()

            self.width = self.icon_width * 2
            self.height = self.icon_height + self.text_height

            self.shape = PySI.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])

            self.is_icon_visible = True
            self.is_opened_visible = False
            self.color = PySI.Color(25, 0, 0, 0)
            self.add_QML_data("container_width", self.width, PySI.DataType.INT)
            self.add_QML_data("container_height", self.height, PySI.DataType.INT)
            self.add_QML_data("is_icon_visible", self.is_icon_visible, PySI.DataType.BOOL)
            self.add_QML_data("is_opened_visible", self.is_opened_visible, PySI.DataType.BOOL)
            self.is_open_entry_capability_blocked = False

            for child in self.children:
                child.delete()

            self.snap_to_mouse()

    def on_parent_enter_emit(self, other):
        if self.is_open_entry_capability_blocked and self.parent == "" and not other.is_open_entry_capability_blocked:
            if other not in self.children:
                self.children.append(other)

            return self._uuid

        return ""

    def on_parent_leave_emit(self, other):
        if self.is_open_entry_capability_blocked and self.parent == "" and not other.is_open_entry_capability_blocked:
            if other in self.children:
                del self.children[self.children.index(other)]

            return self._uuid

        return ""

    def on_parent_enter_recv(self, _uuid):
        if _uuid != "" and not self.is_open_entry_capability_blocked:
            if self.parent == "":
                self.parent = _uuid
                self.create_link(_uuid, PySI.LinkingCapability.POSITION, self._uuid, PySI.LinkingCapability.POSITION)

    def on_parent_leave_recv(self, _uuid):
        if _uuid != "" and not self.is_open_entry_capability_blocked:
            if self.parent == _uuid:
                self.remove_link(self.parent, PySI.LinkingCapability.POSITION, self._uuid, PySI.LinkingCapability.POSITION)
                self.parent = ""

    def show_current_folder_contents_page(self):
        dir_x = self.absolute_x_pos()
        dir_y = self.absolute_y_pos()

        self.add_child_entries(dir_x, dir_y)
        self.add_child_buttons(dir_x, dir_y)

    def add_child_entries(self, dir_x, dir_y):
        dir_width = self.icon_width * 2
        dir_height = self.icon_height + self.text_height
        x_offset = self.preview_width / 10
        y_offset = self.preview_height / 6
        y_offset2 = dir_height / 8

        i = 0
        y = -1
        x = 1

        for i in range(len(self.browse_pages[self.current_page])):
            entry = self.browse_pages[self.current_page][i]

            if i & 1:
                x += 1
            else:
                x -= 1
                y += 1

            entry_shape = [[((x_offset + dir_width) * x) + (dir_x + x_offset), ((y_offset2 + dir_height) * y) + (dir_y + y_offset)],
                           [((x_offset + dir_width) * x) + (dir_x + x_offset), ((y_offset2 + dir_height) * y) + (dir_y + y_offset + dir_height)],
                           [((x_offset + dir_width) * x) + (dir_x + x_offset + dir_width), ((y_offset2 + dir_height) * y) + (dir_y + y_offset + dir_height)],
                           [((x_offset + dir_width) * x) + (dir_x + x_offset + dir_width), ((y_offset2 + dir_height) * y) + (dir_y + y_offset)]]

            kwargs = {}

            kwargs["parent"] = self._uuid
            kwargs["cwd"] = entry[0]

            self.create_region_via_id(entry_shape, entry[1], kwargs)

    def add_child_buttons(self, dir_x, dir_y):
        btn_width = Button.region_width
        btn_height = Button.region_height

        shape_btn1 = [[dir_x + self.preview_width - btn_width, dir_y + self.preview_height - btn_height],
                      [dir_x + self.preview_width - btn_width, dir_y + self.preview_height],
                      [dir_x + self.preview_width, dir_y + self.preview_height],
                      [dir_x + self.preview_width, dir_y + self.preview_height - btn_height]]

        kwargs_btn1 = {}
        kwargs_btn1["parent"] = self._uuid
        kwargs_btn1["value"] = False

        self.create_region_via_id(shape_btn1, PySI.EffectType.SI_BUTTON, kwargs_btn1)

        shape_btn2 = [[dir_x, dir_y + self.preview_height - btn_height],
                      [dir_x, dir_y + self.preview_height],
                      [dir_x + btn_width, dir_y + self.preview_height],
                      [dir_x + btn_width, dir_y + self.preview_height - btn_height]]

        kwargs_btn2 = {}
        kwargs_btn2["parent"] = self._uuid
        kwargs_btn2["value"] = True

        self.create_region_via_id(shape_btn2, PySI.EffectType.SI_BUTTON, kwargs_btn2)
