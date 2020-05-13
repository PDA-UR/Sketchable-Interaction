from libPySI import PySIEffect

## @package SIEffect
# Documentation for this module / class
#
# Used as central entry point for all SIGRun plugins
# Each plugin is required to contain these module level attributes region_name, region_display_name, region_type

## the internal name of the region
region_name = ""

## the name of the region to show to users
region_display_name = ""

## the type of the region - if the region has a type pre-registered to SIGRun the use of that type is required else PySIEffect.EffectType.SI_CUSTOM is to be used
region_type = PySIEffect.EffectType.SI_CUSTOM


## Super Class from which all subsequent plugins are derived
#
# This Class itself is derived from PySIEffect written in C++ which is documented separately within SIGRun
class SIEffect(PySIEffect.PySIEffect):

    ## member constant to mark an effect or link emittable
    EMISSION = True

    ## static member attribute to mark an effect or link receivable
    RECEPTION = False

    ## static member attribute to signal that it's associated effect does not display an icon (texture) when drawn as a region
    TEXTURE_PATH_NONE = ""

    ## static member attribute to notify SIGRun to resample a region's shape when changed from PySI
    RESAMPLING = True

    ## static member attribute to notify SIGRun to not resample a region's shape when changed from PySI
    # Use with caution!
    # May lead to unexpected / barely debugable behaviour!
    NO_RESAMPLING = False

    ## constructor
    #
    # Constructs a new SIEffect object based on the given arguments.
    #
    # @param self the object pointer
    # @param shape the contour of the drawn region (PySIEffect.PointVector)
    # @param aabb the axis-aligned bounding-box of the drawn region (PySIEffect.PointVector)
    # @param uuid the universally unique identifier of the drawn region (str)
    # @param texture_path the path to an image intended to be used as an icon for the drawn region (str)
    # @param kwargs keyworded arguments which may necessary for more specific implementations of region effects (dict)
    def __init__(self, shape, uuid, texture_path, kwargs):
        super(SIEffect, self).__init__(shape, uuid, texture_path, kwargs)

        ## member attribute variable containing the shape (contour) of a drawn region as a PySIEffect.PointVector
        self.shape = shape

        ## member attribute variable containing the axis-aligned bounding-box (aabb) of a drawn region as a PySIEffect.PointVector
        #
        # This variable is automatically computed when shape is changed.
        # It is recommended to use this variable read-only.
        self.aabb

        ## member variable containing the maximum width of the region
        #
        # computed via aabb
        self.width = int(self.region_width())

        ## member variable containing the maximum height of the region
        #
        # computed via aabb
        self.height = int(self.region_height())

        ## member attribute variable containing the universally unique identifier (uuid) of a drawn region as a str
        self._uuid = uuid

        ## member attribute variable containing the name of a drawn region as a str
        self.name = PySIEffect.SI_STD_NAME_PLACEHOLDER

        ## member attribute variable containing the type of effect of a drawn region as a PySIEffect.EffectType
        #
        # Effect implementation which are currently not part of the Standard Environment Library of SIGRun are required to be of type SI_CUSTOM
        self.region_type = PySIEffect.EffectType.SI_CUSTOM

        ## member attribute variable containing the source of effect of a drawn region as a str
        #
        # Effect implementation which are currently not part of the Standard Environment Library of SIGRun are encouraged to not start with "libStdSI"
        self.source = "libStdSI"

        ## member attribute variable containing the path to a QML file for styling of a drawn region as a str
        #
        # This value can be left empty if no visualization of the region is intended (e.g. Container-Regions for External Applications or MouseCursor)
        # @see Container
        # @see MouseCursor
        self.qml_path = ""

        ## member attribute variable containing the last relative movement of the region according to the x axis as a float
        self.delta_x = 0

        ## member attribute variable containing the last relative movement of the region according to the y axis as a float
        self.delta_y = 0

        ## member attribute variable containing the last absolute x coordinate as a float
        self.last_x = 0

        ## member attribute variable containing the last absolute y coordinate as a float
        self.last_y = 0

        ## member attribute variable containing the fill color of a region in RGBA as a PySIEffect.Color
        self.color = PySIEffect.Color(33, 33, 33, 127)

        ## member attribute variable which is true when an user directly controls the region (e.g. moving it around) as a bool
        self.is_under_user_control = False

        ## member attribute variable storing the uuids of present cursors once a region drawing is to be registered as a PySIEffect.StringVector
        self.__registered_regions__ = PySIEffect.StringVector()

        ## member attribute variable storing the path to the image file used as texture for a region
        self.texture_path = texture_path

        if self.texture_path != "":
            ## member attribute variable storing the width of a texture of a region drawing as a float
            #
            # This value is only set if texture_path is a valid path
            self.texture_width = 75

            ## member attribute variable storing the height of a texture of a region drawing as a float
            #
            # This value is only set if texture_path is a valid path
            self.texture_height = 75

            self.__add_data__("img_width", self.texture_width, PySIEffect.DataType.INT)
            self.__add_data__("img_heaight", self.texture_height, PySIEffect.DataType.INT)
            self.__add_data__("img_path", self.texture_path, PySIEffect.DataType.STRING)
            self.__add_data__("widget_width", self.width, PySIEffect.DataType.FLOAT)
            self.__add_data__("widget_height", self.height, PySIEffect.DataType.FLOAT)

        ## member attribute variable storing keys to functions which are called when collision events occur for emitting data to receiving regions
        #
        # This variable is a PySIEffect.String2_String2FunctionMap_Map (c++-bindings) and uses capabilities (str) as keys to the inner String2FunctionMap.
        # The inner String2FunctionMap uses collision event names (PySIEffect.ON_ENTER ("on_enter"), PySIEffect:ON_CONTINUOUS ("on_continuous"), PySIEffect.ON_LEAVE ("on_leave")) as keys to their corresponding functions as values
        #
        # Example:
        #
        # self.cap_emit["CAPABILITY"] = {PySIEffect.ON_ENTER: self.<function_enter>, PySIEffect:ON_CONTINUOUS: self.<function_continuous>, PySIEffect.ON_LEAVE: self.<function_leave>
        #
        # Therefore, this example allows a region to emit an effect of CAPABILITY once a collision event occurred
        self.cap_emit = PySIEffect.String2_String2FunctionMap_Map()

        ## member attribute variable storing keys to functions which are called when collision events occur for receiving data from emitting regions
        #
        # This variable is a PySIEffect.String2_String2FunctionMap_Map (c++-bindings) and uses capabilities (str) as keys to the inner String2FunctionMap.
        # The inner String2FunctionMap uses collision event names (PySIEffect.ON_ENTER ("on_enter"), PySIEffect:ON_CONTINUOUS ("on_continuous"), PySIEffect.ON_LEAVE ("on_leave")) as keys to their corresponding functions as values
        #
        # Example:
        #
        # self.cap_recv["CAPABILITY"] = {PySIEffect.ON_ENTER: self.<function_enter>, PySIEffect:ON_CONTINUOUS: self.<function_continuous>, PySIEffect.ON_LEAVE: self.<function_leave>
        #
        # Therefore, this example allows a region to receive an effect of CAPABILITY once a collision event occurred
        self.cap_recv = PySIEffect.String2_String2FunctionMap_Map()

        ## member attribute variable storing keys to functions which are called when linking events occur for emitting data to receiving regions
        #
        # This variable is a String2FunctionMap (c++-bindings) containing capabilities (str) as keys and functions as values
        #
        # Example with SI-integrated linking of positions for emission case:
        # self.cap_link_emit[PySIEffect.POSITION] = self.<function_position_emit>
        # Therefore, this example emits the positional data of the region to a linked region.

        # Example with custom capability for linking:
        # self.cap_link_emit[<name of capability>] = self.<corresponding function>
        # Therefore, this example emits some data of the region to a linked region based on the capability
        self.cap_link_emit = PySIEffect.String2FunctionMap()

        ## member attribute variable storing keys to functions which are called when linking events occur for emitting data to receiving regions
        #
        # This variable is a PySIEffect.String2_String2FunctionMap_Map (c++-bindings) and uses linking event capability names (str) as keys to the inner String2FunctionMap.
        # The inner String2FunctionMap uses linking event capability names (PySIEffect.POSITION, <own name as str>) as keys to their corresponding functions as values.
        # The outer key corresponds to the emission capability.
        # The inner key corresponds to the reception capability of the targeted region and points towards the function which is to be called during the linking event
        # Therefore, it is possible to map e.g. incomimg positional data to the color of the receiving region.
        #
        # Example with SI-integrated linking of positions for reception case:
        # self.cap_link_recv[PySIEffect.POSITION][PySIEffect.POSITION] = self.<function_position_emit>
        # self.cap_link_recv[PySIEffect.POSITION][PySIEffect.COLOR] = self.<function_color_emit>
        # Therefore, this example receives the positional data of a linked region and can apply this data to other categories of data according to the linking relationship.

        # Example with custom capability for linking:
        # self.cap_link_recv[<name of emission capability>][<name of reception capability>] = self.<corresponding function>
        # Therefore, this example receives some data of a linked region and can apply this data to other categories of data according to the linking relationship .
        self.cap_link_recv = PySIEffect.String2_String2FunctionMap_Map()

        ## member attribute variable storing the x position of the mouse cursor
        self.mouse_x = 0

        ## member attribute variable storing the y position of the mouse cursor
        self.mouse_y = 0

        self.enable_effect(PySIEffect.MOVE, self.RECEPTION, self.on_move_enter_recv, self.on_move_continuous_recv, self.on_move_leave_recv)
        self.enable_effect(PySIEffect.DELETION, self.RECEPTION, None, None, None)

        self.enable_link_reception(PySIEffect.POSITION, PySIEffect.POSITION, self.set_position_from_position)

    ## member function for retrieving the maximum width of a region
    def region_width(self):
        return self.aabb[3].x - self.aabb[0].x

    ## member function for retrieving the maximum height of a region
    def region_height(self):
        return self.aabb[1].y - self.aabb[0].y

    ## member function for setting the position of a region based on the positional data of another region.
    #
    # This function is used as a reception function for linking events where positional data is emitted by another region and applied to the position of a region.
    #
    # @param self the object pointer
    # @param rel_x the relative positional change on the x axis (float)
    # @param rel_y the relative positional change on the y axis (float)
    # @param abs_x the absolute position on the x axis (float)
    # @param abs_y the absolute position on the y axis (float)
    def set_position_from_position(self, rel_x, rel_y, abs_x, abs_y):
        self.x += rel_x
        self.y += rel_y

        self.delta_x, self.delta_y = rel_x, rel_y

        if self.is_under_user_control:
            self.mouse_x = abs_x
            self.mouse_y = abs_y
        else:
            self.mouse_x = 0
            self.mouse_y = 0

    ## member function forgetting the x coordinate of the parent region's top left corner
    #
    # @param self the object pointer
    def x_pos(self):
        return self.aabb[0].x

    ## member function forgetting the y coordinate of the parent region's top left corner
    #
    # @param self the object pointer
    def y_pos(self):
        return self.aabb[0].y

    ## member function for receiving data from the PySIEffect.MOVE capability for the PySIEffect.ON_ENTER collision event
    #
    # @param self the object pointer
    # @param cursor_id the cursor which is intended to move the region (str)
    # @param link_attribute the linking attribute defining how the cursor and the region are intended to be linked (str)
    def on_move_enter_recv(self, cursor_id, link_attrib):
        if not cursor_id is "" and not link_attrib is "":
            self.link_relations.append([cursor_id, link_attrib, self._uuid, link_attrib])
            self.is_under_user_control = True

    ## member function for the PySIEffect.MOVE capability for the PySIEffect.ON_CONTINUOUS collision event
    #
    # @param self the object pointer
    def on_move_continuous_recv(self):
        pass

    ## member function for receiving data from the PySIEffect.MOVE capability for the PySIEffect.ON_LEAVE collision event
    #
    # @param self the object pointer
    # @param cursor_id the cursor which is intended to move the region (str)
    # @param link_attribute the linking attribute defining how the cursor and the region are intended to be linked (str)
    def on_move_leave_recv(self, cursor_id, link_attrib):
        if not cursor_id is "" and not link_attrib is "":

            lr = PySIEffect.LinkRelation(cursor_id, link_attrib, self._uuid, link_attrib)

            if lr in self.link_relations:
                del self.link_relations[self.link_relations.index(lr)]

            self.is_under_user_control = False

        return 0

    ## member function for enabling the emission or reception of an effect
    #
    # @param self the object pointer
    # @param capability the capability of the collision event (str)
    # @param is_emit the variable depicting if a region emits (True) or receives (False) an effect (bool)
    # @param on_enter the function to be called for the collision event PySIEffect.ON_ENTER
    # @param on_continuous the function to be called for the collision event PySIEffect.ON_CONTINUOUS
    # @param on_leave the function to be called for the collision event PySIEffect.ON_LEAVE
    def enable_effect(self, capability, is_emit, on_enter, on_continuous, on_leave):
       if is_emit:
           self.cap_emit[capability] = {PySIEffect.ON_ENTER: on_enter, PySIEffect.ON_CONTINUOUS: on_continuous, PySIEffect.ON_LEAVE: on_leave}
       else:
           self.cap_recv[capability] = {PySIEffect.ON_ENTER: on_enter, PySIEffect.ON_CONTINUOUS: on_continuous, PySIEffect.ON_LEAVE: on_leave}

    def is_effect_enabled(self, capability, is_emit):
        if is_emit:
            return capability in self.cap_emit
        else:
            return capability in self.cap_recv

    ## member function for overriding the emission or reception of an effect
    #
    # @param self the object pointer
    # @param capability the capability of the collision event (str)
    # @param is_emit the variable depicting if a region emits (True) or receives (False) an effect (bool)
    # @param on_enter the function to be called for the collision event PySIEffect.ON_ENTER
    # @param on_continuous the function to be called for the collision event PySIEffect.ON_CONTINUOUS
    # @param on_leave the function to be called for the collision event PySIEffect.ON_LEAVE
    #
    # This function then calls self.enable_effect(capability, is_emit, on_enter, on_continuous, on_leave)
    # @see self.enable_effect(capability, is_emit, on_enter, on_continuous, on_leave)
    def override_effect(self, capability, is_emit, on_enter, on_continuous, on_leave):
        self.enable_effect(capability, is_emit, on_enter, on_continuous, on_leave)

    ## member function for disabling the emission or reception of an effect
    #
    # @param self the object pointer
    # @param capability the capability of the collision event (str)
    # @param is_emit the variable depicting if a region emits (True) or receives (False) an effect (bool)
    def disable_effect(self, capability, is_emit):
        if is_emit:
            if capability in self.cap_emit:
                del self.cap_emit[capability]
        else:
            if capability in self.cap_recv:
                del self.cap_recv[capability]

    ## member function for enabling the emission of data in the context of a link event
    #
    # @param self the object pointer
    # @param emission_capability the capability of the linking event (str)
    # @param emission_function the function to be called for emitting data
    def enable_link_emission(self, emission_capability, emission_function):
        self.cap_link_emit[emission_capability] = emission_function

    ## member function for enabling the emission of data in the context of a link event
    #
    # @param self the object pointer
    # @param emission_capability the capability of the linking event used by the emitting region (str)
    # @param reception_capability the capability of the linking event of a receiving region (str)
    # @param reception_function the function to be called for receiving data
    def enable_link_reception(self, emission_capability, reception_capability, reception_function):
        self.cap_link_recv[emission_capability] = {reception_capability: reception_function}

    ## member function for disabling the emission of data in the context of a link event
    #
    # @param self the object pointer
    # @param emission_capability the capability of the linking event used by the emitting region (str)
    def disable_link_emission(self, emission_capability):
        del self.cap_link_emit[emission_capability]

    ## member function for disabling the reception of data in the context of a link event
    #
    # @param self the object pointer
    # @param emission_capability the capability of the linking event used by the emitting region (str)
    # @param reception_capability the capability of the linking event of a receiving region with default value "" (str)
    #
    # If no reception_capability is specified, the emission_capability is deleted from self.cap_link_recv.
    # If reception_capability is specified and present in self.cap_link_recv, the specified relation is deleted from emission_capability.
    # @see self.cap_link_recv
    def disable_link_reception(self, emission_capability, reception_capability=""):
        if reception_capability is "":
            if emission_capability in self.cap_link_recv:
                del self.cap_link_recv[emission_capability]
        else:
            if emission_capability in self.cap_link_recv:
                if reception_capability in self.cap_link_recv[emission_capability]:
                    del self.cap_link_recv[emission_capability][reception_capability]

    ## member function for establishing a specified link between two regions according to given attributes
    #
    # @param self the object pointer
    # @param sender_uuid the uuid of the emitting region (str)
    # @param sender_attribute the attribute to be linked by the emitting region (str)
    # @param receiver_uuid the uuid of the receiving region (str)
    # @param receiver_attribute the attribute to be linked by the receiving region (str)
    def create_link(self, sender_uuid, sender_attribute, receiver_uuid, receiver_attribute):
        if sender_uuid is not "" and sender_attribute is not "" and receiver_uuid is not "" and receiver_attribute is not "":
            self.link_relations.append([sender_uuid, sender_attribute, receiver_uuid, receiver_attribute])

    ## member function for removing a specified link between two regions according to given attributes
    #
    # @param self the object pointer
    # @param sender_uuid the uuid of the emitting region (str)
    # @param sender_attribute the attribute to be linked by the emitting region (str)
    # @param receiver_uuid the uuid of the receiving region (str)
    # @param receiver_attribute the attribute to be linked by the receiving region (str)
    def remove_link(self, sender_uuid, sender_attribute, receiver_uuid, receiver_attribute):
        if sender_uuid is not "" and sender_attribute is not "" and receiver_uuid is not "" and receiver_attribute is not "":
            lr = PySIEffect.LinkRelation(sender_uuid, sender_attribute, receiver_uuid, receiver_attribute)

            if lr in self.link_relations:
                del self.link_relations[self.link_relations.index(lr)]

    ## member function for setting data in the associated qml file of a region effect
    #
    # @param self the object pointer
    # @param key the variable specified in the qml file (str)
    # @param value the value to set in the variable in the qml file (variant)
    # @param datatype the data type of the value (PySIEffect.INT, PySIEffect.FLOAT, ...) (int)
    #
    # Calls the function __add_data__ (c++-bindings)
    def add_QML_data(self, key, value, datatype):
        self.__add_data__(key, value, datatype)

    ## member function for adding a point to a region drawing based on a cursor id.
    #
    # @param self the object pointer
    # @param x the x coordinate of the cursor (float)
    # @param y the y coordinate of the cursor (float)
    # @param cursor_id the id of cursor currently drawing (str)
    #
    # This function is specific to effects of PySIEffect.EffectType.SI_CANVAS.
    # Therefore, this function does nothing when called with other effect types.
    #
    # This function uses self.__partial_regions__ (c++-bindings)
    def add_point_to_region_drawing(self, x, y, cursor_id):
        if self.region_type is int(PySIEffect.EffectType.SI_CANVAS):
            if cursor_id not in self.__partial_regions__.keys():
                self.__partial_regions__[cursor_id] = PySIEffect.PointVector()

            self.__partial_regions__[cursor_id].append([x, y])

    ## member function for registering a region drawing according to a cursor id
    #
    # @param self the object pointer
    # @param cursor_id the id of the cursor which is currently drawing (str)
    #
    # This function is specific to effects of PySIEffect.EffectType.SI_CANVAS.
    # Therefore, this function does nothing when called with other effect types.
    #
    # This function uses self.__registered_regions__ (c++-bindings)
    def register_region_from_drawing(self, cursor_id):
        if self.region_type is int(PySIEffect.EffectType.SI_CANVAS):
            self.__registered_regions__.append(cursor_id)

    ## member function for starting the standard application of a file given its uuid as a region and its path in the filesystem
    #
    # @param self the object pointer
    # @param file_uuid the uuid of the region associated to a file icon representing a file of the filesystem (str)
    # @param file_path the path of the file in the filesystem (str)
    #
    # This function calls self.__embed_file_standard_appliation_into_context__ (c++-bindings)
    def start_standard_application(self, file_uuid, file_path):
        self.__embed_file_standard_appliation_into_context__(file_uuid, file_path)

    ## member function for closing the standard application of a file given its uuid as a region and its path in the filesystem
    #
    # @param self the object pointer
    # @param file_uuid the uuid of the region associated to a file icon representing a file of the filesystem (str)
    #
    # This function calls self.__destroy_embedded_window__ (c++-bindings)
    def close_standard_application(self, file_uuid):
        self.__destroy_embedded_window__(file_uuid)

    ## member function for displaying the contents of a folder in the filesystem as pages of other filesystem entries
    #
    # @param self the object pointer
    # @param source_uuid the uuid of the region associated to a folder icon representing a folder of the filesystem (str)
    # @param with_buttons a flag depicting whether buttons for browsing pages is wanted (True) or not (False) (bool)
    #
    # This function calls self.__show_folder_contents_page__ (c++-bindings)
    def display_folder_contents_page(self, page, source_uuid, with_buttons=True):
        self.__show_folder_contents_page__(page, source_uuid, with_buttons)

    ## member function for deleting a region
    #
    # @param self the object pointer
    def delete(self):
        self.__signal_deletion__()

    ## member function for creating a new region
    #
    # @param self the object pointer
    # @param shape the shape / contour of the region as a PySIEffect.PointVector or list [[x1, x1], [x2, y2], ... [xn, yn]]
    # @param effect_name the name (region_name) of the effect which shall be assigned to the region (region_display_name does not work)
    def create_region(self, shape, effect_name, as_selector=False, kwargs={}):
        self.__create_region__(shape, effect_name, as_selector, kwargs)

    ## member function for retrieving the plugins which are available for sketching as a dict of names.
    # This dict of names uses region_name attributes as keys and region_display_name attributes as values
    #
    # @param self the object pointer
    def available_plugins(self):
        return list(self.__available_plugins_by_name__())

    ## member function for snapping a region's center to the mouse cursor
    #
    # @param self the object pointer
    def snap_to_mouse(self):
        self.x = self.mouse_x - self.x_pos() - self.width / 2
        self.y = self.mouse_y - self.y_pos() - self.height / 2

    ## member function for retrieving the dimensions of the active SI-Context (width in px, and height in px)
    #
    # @param self the object pointer
    def context_dimensions(self):
        return self.__context_dimensions__

    def assign_effect(self, effect_name_to_assign, effect_display_name, kwargs):
        self.__assign_effect__(self._uuid, effect_name_to_assign, effect_display_name, kwargs)
