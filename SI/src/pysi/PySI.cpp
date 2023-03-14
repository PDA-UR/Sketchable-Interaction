
#include "PySIEffect.hpp"
#include "pysi/stl_container_exposure/MapExposure.hpp"
#include "pysi/stl_container_exposure/VectorExposure.hpp"
#include "PySIStartup.hpp"
#include <sigrun/SIConstants.hpp>
#include <pysi/pickling/PickleSuits.hpp>

BOOST_PYTHON_MODULE(libPySI)
{
    //PyEval_InitThreads();

    { // scope opened for everything which shall be part of effect_scope
        bp::scope effect_scope = bp::class_<PySIEffect>("PySI",bp::init<const std::vector<glm::vec3> &, const std::string &, const std::string &, const bp::dict &>());

        bp::class_<glm::vec2>("Point2", bp::init<float, float>())
                .def_pickle(Point2PickleSuit())

                .def_readwrite("x", &glm::vec2::x)
                .def_readwrite("y", &glm::vec2::y)
                ;

        bp::class_<glm::vec3>("Point3", bp::init<float, float, float>())
                .def_pickle(Point3PickleSuit())

                .def_readwrite("x", &glm::vec3::x)
                .def_readwrite("y", &glm::vec3::y)
                .def_readwrite("z", &glm::vec3::z)
                ;

        bp::class_<glm::vec4>("Color", bp::init<float, float, float, float>())
                .def_pickle(ColorPickleSuit())

                .def_readwrite("r", &glm::vec4::r)
                .def_readwrite("g", &glm::vec4::g)
                .def_readwrite("b", &glm::vec4::b)
                .def_readwrite("a", &glm::vec4::a)
                ;

        bp::class_<LinkCandidate>("LinkRelation",bp::init<const std::string &, const std::string &, const std::string &, const std::string &>())
                .def_pickle(LinkRelationPickleSuit())

                .def_readwrite("sender", &LinkCandidate::sender)
                .def_readwrite("sender_attrib", &LinkCandidate::sender_attrib)
                .def_readwrite("recv", &LinkCandidate::recv)
                .def_readwrite("recv_attrib", &LinkCandidate::recv_attrib)
                ;

        create_vector<VectorExposureVec3, std::vector<glm::vec3>, PointVectorPickleSuite>("PointVector");
        create_vector<VectorExposureLinkRelation, std::vector<LinkCandidate>, LinkRelationVectorPickleSuite>("LinkRelationVector");
        create_vector<VectorExposureString, std::vector<std::string>, StringVectorPickleSuite>("StringVector");
        create_vector<VectorExposureStringVector, std::vector<std::vector<std::string>>, StringVectorVectorPickleSuite>("StringVectorVector");

        create_map<MapExposurePartialContour, std::unordered_map<std::string, std::vector<glm::vec3>>, PartialContourPickleSuite>("PartialContour");
        create_map<MapExposureString2Function, std::unordered_map<std::string, bp::object>, String2FunctionMapPickleSuite>("String2FunctionMap");
        create_map<MapExposureString2_String2FunctionMap_Map, std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>, String2String2FunctionMapMapPickleSuite>("String2String2FunctionMapMap");

        bp::class_<PySIEffect>("Effect",bp::init<const std::vector<glm::vec3> &, const std::string &, const std::string &, const bp::dict &>())
                .def_pickle(EffectPickleSuit())

                .def("__set_data__", &PySIEffect::__set_data__)
                .def("__signal_deletion__", &PySIEffect::__signal_deletion__)
                .def("__signal_deletion_by_uuid__", &PySIEffect::__signal_deletion_by_uuid__)
                .def("__embed_file_standard_appliation_into_context__", &PySIEffect::__embed_file_standard_appliation_into_context__)
                .def("__destroy_embedded_window__", &PySIEffect::__destroy_embedded_file_standard_appliation_in_context__)
                .def < void(PySIEffect::*)(const std::vector<glm::vec3>&, const std::string&, bool, bp::dict &)>("__create_region__", &PySIEffect::__create_region__)
                .def < void(PySIEffect::*)(const bp::list&, const std::string&, bool,bp::dict &)>("__create_region__", &PySIEffect::__create_region__)
                .def < void(PySIEffect::*)(const bp::list&, int, bp::dict &)>("__create_region__", &PySIEffect::__create_region__)
                .def < void(PySIEffect::*)(const bp::object&, const bp::dict&)>("__create_region__", &PySIEffect::__create_region__)
                .def < void(PySIEffect::*)(const bp::list &, bp::object &, bp::dict &)>("__create_region__", &PySIEffect::__create_region__)
                .def("__available_plugins_by_name__", &PySIEffect::__available_plugins_by_name__)
                .def("__context_dimensions__", &PySIEffect::__context_dimensions__)
                .def("__assign_effect__", &PySIEffect::__assign_effect__)
                .def("__emit_linking_action__", &PySIEffect::__emit_linking_action__)
                .def("__data__", &PySIEffect::__data__)
                .def("__logger_messages__", &PySIEffect::__logger_messages__)
                .def("__qml_data_keys_and_types__", &PySIEffect::__qml_data_keys_and_types__)
                .def("__current_regions__", &PySIEffect::__current_regions__)
                .def("__excluded_plugins__", &PySIEffect::__excluded_plugins__)
                .def("__conditional_variables__", &PySIEffect::__conditional_variables__)
                .def("__set_drawing_additions__", &PySIEffect::__set_drawing_additions__)
                .def("__drawing_additions__", &PySIEffect::__drawing_additions__)
                .def("__selected_effects_by_cursor_id__", &PySIEffect::__selected_effects_by_cursor_id__)
                .def("__set_cursor_stroke_width_by_cursorid__", &PySIEffect::__set_cursor_stroke_width_by_cursorid__)
                .def("__set_cursor_stroke_color_by_cursorid__", &PySIEffect::__set_cursor_stroke_color_by_cursorid__)
                .def("__on_destroy__", &PySIEffect::__on_destroy__)
                .def("__click_mouse__", &PySIEffect::__click_mouse__)
                .def("__dbl_click_mouse__", &PySIEffect::__dbl_click_mouse__)

                // used for sending stuff back over network
                .def("__notify__", &PySIEffect::__notify__)
                .def("__current_tangible_selection__", &PySIEffect::__current_tangible_selection__)
                .def("__add_multiple_regions__", &PySIEffect::__add_multiple_regions__)
                .def("__move_hard__", &PySIEffect::__move_hard__)

                .add_property("current_collisions", &PySIEffect::get_collisions, &PySIEffect::set_collisions)
                .add_property("shape", &PySIEffect::get_shape, &PySIEffect::set_shape)

                .def_readwrite("__is_flagged_for_deletion__", &PySIEffect::d_flagged_for_deletion)
                .def_readwrite("aabb", &PySIEffect::d_aabb)
                .def_readwrite("__recompute_collision_mask__", &PySIEffect::d_recompute_mask)
                .def_readwrite("__partial_regions__", &PySIEffect::d_partial_regions)
                .def_readwrite("__registered_regions__", &PySIEffect::d_regions_marked_for_registration)
                .def_readwrite("__registered_regions_kwargs__", &PySIEffect::d_regions_marked_for_registration_kwargs)
                .def_readwrite("cap_emit", &PySIEffect::d_cap_collision_emit)
                .def_readwrite("cap_recv", &PySIEffect::d_cap_collision_recv)
                .def_readwrite("cap_link_emit", &PySIEffect::d_cap_link_emit)
                .def_readwrite("cap_link_recv", &PySIEffect::d_cap_link_recv)
                .def_readwrite("color", &PySIEffect::d_color)
                .def_readwrite("border_color", &PySIEffect::d_border_color)
                .def_readwrite("border_width", &PySIEffect::d_border_width)
                .def_readwrite("x", &PySIEffect::d_x)
                .def_readwrite("y", &PySIEffect::d_y)
                .def_readwrite("transform_x", &PySIEffect::d_transform_x)
                .def_readwrite("transform_y", &PySIEffect::d_transform_y)
                .def_readwrite("width", &PySIEffect::d_width)
                .def_readwrite("height", &PySIEffect::d_height)
                .def_readwrite("visualization_width", &PySIEffect::d_visualization_width)
                .def_readwrite("visualization_height", &PySIEffect::d_visualization_height)
                .def_readwrite("angle_degrees", &PySIEffect::d_angle_deg)
                .def_readwrite("scale", &PySIEffect::d_scale)
                .def_readwrite("name", &PySIEffect::d_name)
                .def_readwrite("_uuid", &PySIEffect::d_uuid)
                .def_readwrite("source", &PySIEffect::d_source)
                .def_readwrite("qml_path", &PySIEffect::d_qml_path)
                .def_readwrite("region_type", &PySIEffect::d_effect_type)
                .def_readwrite("left_mouse_clicked", &PySIEffect::d_is_left_mouse_clicked)
                .def_readwrite("right_mouse_clicked", &PySIEffect::d_is_right_mouse_clicked)
                .def_readwrite("middle_mouse_clicked", &PySIEffect::d_is_middle_mouse_clicked)
                .def_readwrite("double_clicked", &PySIEffect::d_is_double_clicked)
                .def_readwrite("link_relations", &PySIEffect::d_link_relations)
                .def_readwrite("has_data_changed", &PySIEffect::d_data_changed)
                .def_readwrite("mouse_wheel_angle_px", &PySIEffect::mouse_wheel_angle_px)
                .def_readwrite("mouse_wheel_angle_degrees", &PySIEffect::mouse_wheel_angle_degrees)
                .def_readwrite("with_border", &PySIEffect::d_with_border)
                .def_readwrite("visible", &PySIEffect::d_visible)
                .def_readwrite("is_resampling_enabled", &PySIEffect::d_is_resampling_enabled)
                .def_readwrite("evaluate_enveloped", &PySIEffect::d_evaluate_enveloped)
                ;

        bp::enum_<uint32_t>("DataType")
                .value("INT", SI_DATA_TYPE_INT)
                .value("FLOAT", SI_DATA_TYPE_FLOAT)
                .value("BOOL", SI_DATA_TYPE_BOOL)
                .value("STRING", SI_DATA_TYPE_STRING)
                .value("IMAGE_AS_BYTES", SI_DATA_TYPE_IMAGE_AS_BYTES)
                .value("VIDEO", SI_DATA_TYPE_VIDEO)
                .value("LIST", SI_DATA_TYPE_LIST)
                .value("DICT", SI_DATA_TYPE_DICT)

                .export_values()
                ;

        bp::enum_<uint32_t>("EffectType")
                .value("SI_CANVAS", SI_TYPE_CANVAS)
                .value("SI_CURSOR", SI_TYPE_CURSOR)
                .value("SI_MOUSE_CURSOR", SI_TYPE_MOUSE_CURSOR)
                .value("SI_EXTERNAL_APPLICATION_CONTAINER", SI_TYPE_EXTERNAL_APPLICATION_CONTAINER)
                .value("SI_DIRECTORY", SI_TYPE_DIRECTORY)
                .value("SI_TEXT_FILE", SI_TYPE_TEXT_FILE)
                .value("SI_IMAGE_FILE", SI_TYPE_IMAGE_FILE)
                .value("SI_UNKNOWN_FILE", SI_TYPE_UNKNOWN_FILE)
                .value("SI_DELETION", SI_TYPE_DELETION)
                .value("SI_BUTTON", SI_TYPE_BUTTON)
                .value("SI_NOTIFICATION", SI_TYPE_NOTIFICATION)
                .value("SI_CUSTOM", SI_TYPE_CUSTOM)
                .value("SI_CUSTOM_NON_DRAWABLE", SI_TYPE_CUSTOM_NON_DRAWABLE)
                .value("SI_ENTRY", SI_TYPE_ENTRY)
                .value("SI_PREVIEW", SI_TYPE_PREVIEW)
                .value("SI_PALETTE", SI_TYPE_PALETTE)
                .value("SI_SELECTOR", SI_TYPE_SELECTOR)

                .export_values()
                ;

        {
            bp::scope effect_name = bp::class_<bp::object, boost::noncopyable>("EffectName", bp::init<>());

            effect_name.attr("SI_STD_NAME_CANVAS") = SI_NAME_EFFECT_CANVAS;
            effect_name.attr("SI_STD_NAME_DIRECTORY") = SI_NAME_EFFECT_DIRECTORY;
            effect_name.attr("SI_STD_NAME_TEXTFILE") = SI_NAME_EFFECT_TEXTFILE;
            effect_name.attr("SI_STD_NAME_IMAGEFILE") = SI_NAME_EFFECT_IMAGEFILE;
            effect_name.attr("SI_STD_NAME_UNKNOWNFILE") = SI_NAME_EFFECT_UNKNOWNFILE;
            effect_name.attr("SI_STD_NAME_BUTTON") = SI_NAME_EFFECT_BUTTON;
            effect_name.attr("SI_STD_NAME_TAG") = SI_NAME_EFFECT_TAG;
            effect_name.attr("SI_STD_NAME_DELETION") = SI_NAME_EFFECT_DELETION;
            effect_name.attr("SI_STD_NAME_OPEN_ENTRY") = SI_NAME_EFFECT_OPEN_ENTRY;
            effect_name.attr("SI_STD_NAME_MOUSE_CURSOR") = SI_NAME_EFFECT_MOUSE_CURSOR;
            effect_name.attr("SI_STD_NAME_SIMPLE_NOTIFICATION") = SI_NAME_EFFECT_SIMPLE_NOTIFICATION;
            effect_name.attr("SI_STD_NAME_ENTRY") = SI_NAME_EFFECT_ENTRY;
            effect_name.attr("SI_STD_NAME_CONTAINER") = SI_NAME_EFFECT_CONTAINER;
            effect_name.attr("SI_STD_NAME_PLACEHOLDER") = SI_NAME_EFFECT_PLACEHOLDER;
            effect_name.attr("SI_STD_NAME_PREVIEW") = SI_NAME_EFFECT_PREVIEW;
            effect_name.attr("SI_STD_NAME_PALETTE") = SI_NAME_EFFECT_PALETTE;
            effect_name.attr("SI_STD_NAME_SELECTOR") = SI_NAME_EFFECT_SELECTOR;
        }

        {
            bp::scope collision_event = bp::class_<bp::object, boost::noncopyable>("CollisionEvent", bp::init<>());

            collision_event.attr("ON_ENTER") = SI_COLLISION_EVENT_ON_ENTER;
            collision_event.attr("ON_CONTINUOUS") = SI_COLLISION_EVENT_ON_CONTINUOUS;
            collision_event.attr("ON_LEAVE") = SI_COLLISION_EVENT_ON_LEAVE;
        }

        {
            bp::scope collision_capability = bp::class_<bp::object, boost::noncopyable>("CollisionCapability", bp::init<>());

            collision_capability.attr("MOVE") = SI_CAPABILITY_COLLISION_MOVE;
            collision_capability.attr("BTN") = SI_CAPABILITY_COLLISION_BUTTON;
            collision_capability.attr("OPEN_ENTRY") = SI_CAPABILITY_COLLISION_OPEN_ENTRY;
            collision_capability.attr("PARENT") = SI_CAPABILITY_COLLISION_PARENT;
            collision_capability.attr("SKETCH") = SI_CAPABILITY_COLLISION_SKETCH;
            collision_capability.attr("CLICK") = SI_CAPABILITY_COLLISION_CLICK;
            collision_capability.attr("DELETION") = SI_CAPABILITY_COLLISION_DELETION;
            collision_capability.attr("PREVIEW") = SI_CAPABILITY_COLLISION_PREVIEW;
            collision_capability.attr("ASSIGN") = SI_CAPABILITY_COLLISION_ASSIGN;
            collision_capability.attr("HOVER") = SI_CAPABILITY_COLLISION_HOVER;
        }

        {
            bp::scope linking_capability = bp::class_<bp::object, boost::noncopyable>("LinkingCapability", bp::init<>());

            linking_capability.attr("POSITION") = SI_CAPABILITY_LINK_POSITION;
            linking_capability.attr("ROTATION") = SI_CAPABILITY_LINK_ROTATION;
            linking_capability.attr("SCALE") = SI_CAPABILITY_LINK_SCALE;
            linking_capability.attr("COLOR") = SI_CAPABILITY_LINK_COLOR;
            linking_capability.attr("GEOMETRY") = SI_CAPABILITY_LINK_GEOMETRY;
        }

            bp::class_<PySIStartup, boost::noncopyable>("Startup", bp::no_init)
                .def("available_plugins_by_name", &PySIEffect::__available_plugins_by_name__).staticmethod("available_plugins_by_name")
                .def("create_region_by_type", &PySIStartup::create_region_by_type).staticmethod("create_region_by_type")
                .def("create_region_by_name", &PySIStartup::create_region_by_name).staticmethod("create_region_by_name")
                .def("create_region_by_class", &PySIStartup::create_region_by_class).staticmethod("create_region_by_class")
                .def("context_dimensions", &PySIStartup::context_dimensions).staticmethod("context_dimensions")
                .def("logger_quench_messages_from_class", &PySIStartup::logger_quench_messages_from_class).staticmethod("logger_quench_messages_from_class")
                .def("logger_unquench_messages_from_class", &PySIStartup::logger_unquench_messages_from_class).staticmethod("logger_unquench_messages_from_class")
                .def("logger_log", &PySIStartup::logger_log).staticmethod("logger_log")
                .def("logger_set_log_output", &PySIStartup::logger_set_log_output).staticmethod("logger_set_log_output")
                .def("enable", &PySIStartup::enable).staticmethod("enable")
                .def("disable", &PySIStartup::disable).staticmethod("disable")
                .def("exclude_plugins", &PySIStartup::exclude_plugins).staticmethod("exclude_plugins")
                .def("set_tangible_ip_address_and_port", &PySIStartup::set_tangible_ip_address_and_port).staticmethod("set_tangible_ip_address_and_port")
                .def("set_pen_color", &PySIStartup::set_pen_color).staticmethod("set_pen_color")
                .def("set_file_system_root_folder", &PySIStartup::set_file_system_root_folder).staticmethod("set_file_system_root_folder")
                .def("file_system_root_folder", &PySIStartup::file_system_root_folder).staticmethod("file_system_root_folder")
                .def("set_file_system_desktop_folder", &PySIStartup::set_file_system_desktop_folder).staticmethod("set_file_system_desktop_folder")
                .def("file_system_desktop_folder", &PySIStartup::file_system_desktop_folder).staticmethod("file_system_desktop_folder")

                .enable_pickling()
                ;

        { // scope opened for everything which shall be part of logger_scope

            bp::scope logger_scope = bp::class_<PySIStartup>("Logger", bp::no_init);

            logger_scope.attr("SI_LOG_SHOW_ALL") = (int32_t) LOG_SHOW_ALL;
            logger_scope.attr("SI_LOG_SHOW_NONE") = (int32_t) LOG_SHOW_NONE;
            logger_scope.attr("SI_LOG_SHOW_INFO") = (int32_t) LOG_SHOW_INFO;
            logger_scope.attr("SI_LOG_SHOW_WARN") = (int32_t) LOG_SHOW_WARN;
            logger_scope.attr("SI_LOG_SHOW_ERROR") = (int32_t) LOG_SHOW_ERROR;
            logger_scope.attr("SI_LOG_SHOW_DEBUG") = (int32_t) LOG_SHOW_DEBUG;
            logger_scope.attr("SI_LOG_NONE") = (int32_t) LOG_NONE;
            logger_scope.attr("SI_LOG_FILE") = (int32_t) LOG_FILE;

        } // scope ended for everything which shall be part of startup_scope

        { // scope opened for everything which shall be part of startup_scope

            bp::scope startup_scope = bp::class_<PySIStartup>("Configuration", bp::no_init);

            startup_scope.attr("SI_CRASH_DUMP") = (int32_t) SI_CRASH_DUMP_FLAG;
            startup_scope.attr("SI_ANTI_ALIASING_OFF") = (int32_t) SI_ANTI_ALIASING_OFF;
            startup_scope.attr("SI_ANTI_ALIASING_4x") = (int32_t) SI_ANTI_ALIASING_4x;
            startup_scope.attr("SI_ANTI_ALIASING_8x") = (int32_t) SI_ANTI_ALIASING_8x;
            startup_scope.attr("SI_ANTI_ALIASING_16x") = (int32_t) SI_ANTI_ALIASING_16x;
            startup_scope.attr("PEN_CLOLOR_BLACK") = (int32_t) SI_BLACK_DRAWING_COLOR;
            startup_scope.attr("PEN_CLOLOR_WHITE") = (int32_t) SI_WHITE_DRAWING_COLOR;

        } // scope ended for everything which shall be part of startup_scope
    } // scope ended for everything which shall be part of effect_scope
}
