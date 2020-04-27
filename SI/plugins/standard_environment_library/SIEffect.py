from libPySI import PySIEffect


class SIEffect(PySIEffect.PySIEffect):
    EMISSION = True
    RECEPTION = False
    TEXTURE_PATH_NONE = ""
    RESAMPLING = True
    NO_RESAMPLING = False

    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", texture_path="", kwargs={}):
        super(SIEffect, self).__init__()
        self.shape = shape
        self.aabb = aabb
        self._uuid = uuid
        self.name = PySIEffect.SI_STD_NAME_PLACEHOLDER
        self.region_type = PySIEffect.EffectType.SI_CUSTOM
        self.source = "libStdSI"
        self.qml_path = ""
        self.delta_x = 0
        self.delta_y = 0
        self.last_x = 0
        self.last_y = 0
        self.color = PySIEffect.Color(33, 33, 33, 127)
        self.is_under_user_control = False
        self.__registered_regions__ = PySIEffect.StringVector()
        self.width = 0
        self.height = 0

        if texture_path != "":
            self.texture_width = 75
            self.texture_height = 75

            self.__add_data__("img_width", self.texture_width, PySIEffect.DataType.INT)
            self.__add_data__("img_heaight", self.texture_height, PySIEffect.DataType.INT)
            self.__add_data__("img_path", texture_path, PySIEffect.DataType.STRING)

            if len(self.aabb):
                if len(self.aabb):
                    self.width = int(self.aabb[3].x - self.aabb[0].x)
                    self.height = int(self.aabb[1].y - self.aabb[0].y)
                self.__add_data__("widget_width", self.width, PySIEffect.DataType.FLOAT)
                self.__add_data__("widget_height", self.height, PySIEffect.DataType.FLOAT)

        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()
        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map()

        self.enable_effect(PySIEffect.MOVE, self.RECEPTION, self.on_move_enter_recv, self.on_move_continuous_recv, self.on_move_leave_recv)
        self.enable_effect(PySIEffect.DELETION, self.RECEPTION, None, None, None)

        self.cap_link_emit = PySIEffect.String2FunctionMap()
        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map()

        self.enable_link_reception(PySIEffect.POSITION, PySIEffect.POSITION, self.set_position_from_position)

    # default positional linking
    def set_position_from_position(self, rel_x, rel_y, abs_x, abs_y):
        self.x += rel_x
        self.y += rel_y

        self.last_delta_x = rel_x
        self.last_delta_y = rel_y

        return 0

    def on_move_enter_recv(self, cursor_id, link_attrib):
        if not cursor_id is "" and not link_attrib is "":
            self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])
            self.is_under_user_control = True

    def on_move_continuous_recv(self):
        return 0

    def on_move_leave_recv(self, cursor_id, link_attrib):
        if not cursor_id is "" and not link_attrib is "":

            lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

            if lr in self.link_relations:
                del self.link_relations[self.link_relations.index(lr)]

            self.is_under_user_control = False

        return 0

    # SI Python API
    def enable_effect(self, effect, is_emit, on_enter, on_continuous, on_leave):
       if is_emit:
           self.cap_emit[effect] = {PySIEffect.ON_ENTER: on_enter, PySIEffect.ON_CONTINUOUS: on_continuous, PySIEffect.ON_LEAVE: on_leave}
       else:
           self.cap_recv[effect] = {PySIEffect.ON_ENTER: on_enter, PySIEffect.ON_CONTINUOUS: on_continuous, PySIEffect.ON_LEAVE: on_leave}

    def override_effect(self, effect, is_emit, on_enter, on_continuous, on_leave):
        self.enable_effect(effect, is_emit, on_enter, on_continuous, on_leave)

    def disable_effect(self, effect, is_emit):
        if is_emit:
            del self.cap_emit[effect]
        else:
            del self.cap_recv[effect]

    def enable_link_emission(self, emission_capability, emission_function):
        self.cap_link_emit[emission_capability] = emission_function

    def enable_link_reception(self, emission_capability, reception_capability, reception_function):
        self.cap_link_recv[emission_capability] = {reception_capability: reception_function}

    def disable_link_emission(self, emission_capability):
        del self.cap_link_emit[emission_capability]

    def disable_link_reception(self, emission_capability, reception_capability=""):
        if reception_capability is "":
            if emission_capability in self.cap_link_recv:
                del self.cap_link_recv[emission_capability]
        else:
            if emission_capability in self.cap_link_recv:
                if reception_capability in self.cap_link_recv[emission_capability]:
                    del self.cap_link_recv[emission_capability][reception_capability]

    def create_link(self, sender_uuid, sender_attribute, receiver_uuid, receiver_attribute):
        if sender_uuid is not "" and sender_attribute is not "" and receiver_uuid is not "" and receiver_attribute is not "":
            self.link_relations.append([sender_uuid, sender_attribute, receiver_uuid, receiver_attribute])

    def remove_link(self, sender_uuid, sender_attribute, receiver_uuid, receiver_attribute):
        if sender_uuid is not "" and sender_attribute is not "" and receiver_uuid is not "" and receiver_attribute is not "":
            lr = PySIEffect.LinkRelation(sender_uuid, sender_attribute, receiver_uuid, receiver_attribute)

            if lr in self.link_relations:
                del self.link_relations[self.link_relations.index(lr)]

    def add_QML_data(self, key, value, datatype):
        self.__add_data__(key, value, datatype)

    def register_shape_change(self, with_resampling=True):
        self.__notify_shape_changed__(with_resampling)

    # canvas specific API
    def add_point_to_region_drawing(self, x, y, cursor_id):
        if self.region_type is int(PySIEffect.EffectType.SI_CANVAS):
            if cursor_id not in self.__partial_regions__.keys():
                self.__partial_regions__[cursor_id] = PySIEffect.PointVector()

            self.__partial_regions__[cursor_id].append([x, y])

    def register_region_from_drawing(self, cursor_id):
        if self.region_type is int(PySIEffect.EffectType.SI_CANVAS):
            self.__registered_regions__.append(cursor_id)

    # entry specific API
    def start_standard_application(self, file_uuid, file_path):
        self.__embed_file_standard_appliation_into_context__(file_uuid, file_path)

    def close_standard_application(self, file_uuid):
        self.__destroy_embedded_window__(file_uuid)

    def display_folder_contents_page(self, page, source_uuid, with_buttons=True):
        self.__show_folder_contents_page__(page, source_uuid, with_buttons)