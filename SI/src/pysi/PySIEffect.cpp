
#include "PySIEffect.hpp"

#include <sigrun/context/Context.hpp>
#include <sigrun/region/RegionResampler.hpp>
#include <sigrun/util/Dollar1GestureRecognizer.hpp>
#include <boost/python/numpy.hpp>
#include <QDebug>
#include <QVideoFrame>
#include <QImage>
#include <QVariant>
#include <QList>
#include <QtGui>
#include <pysi/pickling/PickleSuits.hpp>
#include <glm/gtx/string_cast.hpp>

namespace bp = boost::python;

PySIEffect::PySIEffect(const std::vector<glm::vec3>& contour, const std::string& uuid, const std::string& tex_path, const bp::dict& kwargs)
    : d_data_changed(false), d_uuid(uuid)
{
    d_regions_marked_for_registration.reserve(10);
    d_link_relations.reserve(100);
    d_contour.reserve(SI_CONTOUR_STEPCOUNT);
    d_aabb.reserve(4);

    std::vector<glm::vec3> temp;

    if((kwargs.has_key("__name__") && kwargs["__name__"] == "__ Painter __"))
    {
        temp = contour;
    }
    else
    {
        Recognizer r;
        r.recognize(temp, contour);
    }


    int32_t x_min = INT32_MAX;
    int32_t x_max = INT32_MIN;
    int32_t y_min = INT32_MAX;
    int32_t y_max = INT32_MIN;

    for(auto& v: temp)
    {
        x_max = v.x > x_max ? v.x : x_max;
        y_max = v.y > y_max ? v.y : y_max;
        x_min = v.x < x_min ? v.x : x_min;
        y_min = v.y < y_min ? v.y : y_min;
    }

    glm::vec3 tlc(x_min, y_min, 1), blc(x_min, y_max, 1), brc(x_max, y_max, 1), trc(x_max, y_min, 1);

    d_aabb = std::vector<glm::vec3>
    {
        tlc, blc, brc, trc
    };

    if(kwargs.has_key("__name__") and kwargs["__name__"] == "__ Painter __")
    {
        d_contour = temp;
    }
    else
    {
        RegionResampler::resample(d_contour, temp);
    }
}

void PySIEffect::set_data(const QMap<QString, QVariant> &data)
{
    for(auto& key: data.keys())
        d_data[key] = data[key];
}

bool PySIEffect::is_border_present()
{
    return d_with_border;
}

bool PySIEffect::visible()
{
    return d_visible;
}

bp::object PySIEffect::__data__(const std::string& key, const uint32_t type)
{
    switch (type)
    {
        case SI_DATA_TYPE_INT:
            return bp::object(d_data[QString(key.c_str())].value<int>());
        case SI_DATA_TYPE_FLOAT:
            return bp::object(d_data[QString(key.c_str())].value<float>());
        case SI_DATA_TYPE_STRING:
            return bp::object(d_data[QString(key.c_str())].value<QString>().toStdString());
        case SI_DATA_TYPE_BOOL:
            return bp::object(d_data[QString(key.c_str())].value<bool>());
        case SI_DATA_TYPE_DICT:
        {
            Print::print("SI_DATA_TYPE_DICT CONVERSION UNIMPLEMENTED");
            bp::dict d;
            return d;
        }
    }

    return bp::object();
}

bp::list PySIEffect::__logger_messages__()
{
    bp::list l;
    const auto& msgs = Log::messages();

    for(auto& msg: msgs)
        l.append(msg);

    return l;
}


void PySIEffect::__signal_deletion__()
{
    d_flagged_for_deletion = true;
}

void PySIEffect::__signal_deletion_by_uuid__(const std::string& uuid)
{
    const auto& regions = Context::SIContext()->region_manager()->regions();

    std::find_if(regions.begin(), regions.end(), [&uuid](auto& r)
    {
       if(uuid == r->uuid())
           r->effect()->d_flagged_for_deletion = true;

       return uuid == r->uuid();
    });
}

bool PySIEffect::is_flagged_for_deletion()
{
    return d_flagged_for_deletion;
}

void PySIEffect::__embed_file_standard_appliation_into_context__(const std::string& uuid, const std::string& path)
{
    Context::SIContext()->external_application_manager()->launch_standard_application(uuid, path);
}

void PySIEffect::__destroy_embedded_file_standard_appliation_in_context__(const std::string& uuid)
{
    Context::SIContext()->external_application_manager()->terminate_application(uuid);
}

void PySIEffect::__emit_linking_action__(const std::string& sender, const std::string& linking_action, const bp::object& args)
{
    bp::tuple t = bp::extract<bp::tuple>(args);
    Context::SIContext()->register_link_event_emission(_UUID_, sender, linking_action, args);
}

void PySIEffect::__move_hard__(float x, float y)
{
    d_x = x;
    d_y = y;

    auto& regions = Context::SIContext()->region_manager()->regions();

    auto it = std::find_if(regions.begin(), regions.end(), [&](auto &region)
    {
        return region->uuid() == uuid();
    });

    if(it != regions.end())
    {
        it->get()->move_and_rotate();
    }
}


const int32_t PySIEffect::x() const
{
    return d_x;
}

const int32_t PySIEffect::y() const
{
    return d_y;
}

const glm::vec4& PySIEffect::color() const
{
    return d_color;
}

std::unordered_map<std::string, bp::object>& PySIEffect::attr_link_emit()
{
    return d_cap_link_emit;
}

std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& PySIEffect::attr_link_recv()
{
    return d_cap_link_recv;
}

std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& PySIEffect::cap_collision_emit()
{
    return d_cap_collision_emit;
}

std::unordered_map<std::string, std::unordered_map<std::string, bp::object>>& PySIEffect::cap_collision_recv()
{
    return d_cap_collision_recv;
}

const std::string& PySIEffect::name() const
{
    return d_name;
}

const std::string& PySIEffect::qml_path() const
{
    return d_qml_path;
}

const std::string& PySIEffect::source() const
{
    return d_source;
}

const uint32_t PySIEffect::effect_type() const
{
    return d_effect_type;
}

bool PySIEffect::has_mouse_pressed_capability(uint32_t btn)
{
    switch (btn)
    {
        case SI_LEFT_MOUSE_BUTTON: // left mouse button
            return d_is_left_mouse_clicked;
        case SI_RIGHT_MOUSE_BUTTON: // right mouse button
            return d_is_right_mouse_clicked;
        case SI_MIDDLE_MOUSE_BUTTON: // middle mouse button
            return d_is_middle_mouse_clicked;
        default:
            return false;
    }
}

void PySIEffect::set_mouse_pressed_capability(uint32_t btn, bool active)
{
    switch (btn)
    {
        case SI_LEFT_MOUSE_BUTTON: // left mouse button
            d_is_left_mouse_clicked = active;
            break;
        case SI_RIGHT_MOUSE_BUTTON: // right mouse button
            d_is_right_mouse_clicked = active;
            break;
        case SI_MIDDLE_MOUSE_BUTTON: // middle mouse button
            d_is_middle_mouse_clicked = active;
            break;
        case SI_DOUBLE_CLICK:
            d_is_double_clicked = active;
        default:
            break;
    }
}

std::vector<std::string>& PySIEffect::regions_for_registration()
{
    return d_regions_marked_for_registration;
}

bp::list& PySIEffect::regions_for_registration_kwargs()
{
    return d_regions_marked_for_registration_kwargs;
}

const std::string& PySIEffect::uuid() const
{
    return d_uuid;
}

std::unordered_map<std::string, std::vector<glm::vec3>>& PySIEffect::partial_region_contours()
{
    return d_partial_regions;
}

const int32_t PySIEffect::width() const
{
    return d_width;
}

const int32_t PySIEffect::height() const
{
    return d_height;
}

const int32_t PySIEffect::visualization_width() const
{
    return d_visualization_width;
}

const int32_t PySIEffect::visualization_height() const
{
    return d_visualization_height;
}

const float PySIEffect::scale() const
{
    return d_scale;
}

const float PySIEffect::angle_degrees() const
{
    return d_angle_deg;
}

std::vector<LinkCandidate>& PySIEffect::link_relations()
{
    return d_link_relations;
}

std::vector<glm::vec3> &PySIEffect::contour()
{
    return d_contour;
}

void PySIEffect::set_aabb(const std::vector<glm::vec3> &aabb)
{
    d_aabb.clear();
    d_aabb.resize(4);

    for(int i = 0; i < aabb.size(); i++)
        d_aabb[i] = aabb[i];
}

std::vector<glm::vec3> &PySIEffect::aabb()
{
    return d_aabb;
}

void PySIEffect::__set_data__(const std::string &key, const bp::object &value, const uint32_t type, const bp::dict& kwargs)
{
    d_data_changed = false;

    QVariant qv;

    switch (type)
    {
        case SI_DATA_TYPE_INT:
            d_data[QString(key.c_str())] = QVariant(bp::extract<int>(value));
            d_data_changed = true;
            break;

        case SI_DATA_TYPE_FLOAT:
            d_data[QString(key.c_str())] = QVariant(bp::extract<float>(value));
            d_data_changed = true;
            break;

        case SI_DATA_TYPE_STRING:
            d_data[QString(key.c_str())] = QVariant(QString(bp::extract<char *>(value)));
            d_data_changed = true;
            break;

        case SI_DATA_TYPE_BOOL:
            d_data[QString(key.c_str())] = QVariant(bp::extract<bool>(value));
            d_data_changed = true;
            break;

        case SI_DATA_TYPE_IMAGE_AS_BYTES:
        {
            int img_width = bp::extract<int>(kwargs["width"]);
            int img_height = bp::extract<int>(kwargs["height"]);

            QImage img(img_width, img_height, QImage::Format::Format_RGBA8888);

            d_data_changed = true;

            if (value.is_none())
            {
                d_data[QString(key.c_str())] = QVariant(QImage());
                break;
            }

            const bp::list &bytes = bp::list(value);

            int len = bp::len(bytes);
            uint8_t buf[len];

            for (int i = 0; i < len; ++i)
                buf[i] = (uint8_t) bp::extract<int>(value[i]);

            img.fromData(buf, len, "PNG");

            d_data[QString(key.c_str())] = QVariant(img);
            break;
        }

        case SI_DATA_TYPE_VIDEO:
        {
            if (value.is_none())
                break;

            d_data_changed = true;

            int img_width = bp::extract<int>(kwargs["width"]);
            int img_height = bp::extract<int>(kwargs["height"]);

            const bp::list &bytes = bp::list(value);

            int len = bp::len(bytes);
            uint8_t buf[len];

            for (int i = 0; i < len; ++i)
                buf[i] = (uint8_t) bp::extract<int>(value[i]);

            QImage img(img_width, img_height, QImage::Format::Format_RGB888);
            img.fromData(buf, len);

            d_data[QString(key.c_str())] = img.convertToFormat(QVideoFrame::imageFormatFromPixelFormat(QVideoFrame::Format_RGB32));
            break;
        }

        case SI_DATA_TYPE_LIST:
        {
            if(value.is_none())
                break;

            d_data_changed = true;

            QList<QString> list;

            for (int i = 0; i < bp::len(value); ++i)
            {
                std::string s = bp::extract<std::string>(value[i]);

                list.push_back(s.c_str());
            }

            d_data[QString(key.c_str())] = QVariant(list);

            break;
        }
    }
}

const QMap<QString, QVariant> &PySIEffect::data()
{
    d_data_changed = false;

    return d_data;
}

const bool PySIEffect::has_data_changed() const
{
    return d_data_changed;
}

std::vector<glm::vec3> PySIEffect::get_shape()
{
    return d_contour;
}

const std::vector<glm::vec3>& PySIEffect::original_shape()
{
    return d_original_contour;
}

void PySIEffect::set_shape(const std::vector<glm::vec3>& shape)
{
    if (shape.empty() || d_name.empty())
        return;

    d_contour.clear();
    std::vector<glm::vec3> temp, smoothed;

    if(d_name != "__ ConveyorBelt __" && d_name != "__ Painter __")
    {
        Recognizer r;
        r.recognize(temp, shape);
    }
    else
    {
        temp = shape;
    }

    d_original_contour.clear();
    d_original_contour.resize(temp.size());

    for(int i = 0; i < temp.size(); ++i)
        d_original_contour[i] = temp[i];

    int x_min = INT32_MAX;
    int x_max = INT32_MIN;
    int y_min = INT32_MAX;
    int y_max = INT32_MIN;

    for(const auto& v: d_original_contour)
    {
        x_max = v.x > x_max ? std::round(v.x) : x_max;
        y_max = v.y > y_max ? std::round(v.y) : y_max;
        x_min = v.x < x_min ? std::round(v.x) : x_min;
        y_min = v.y < y_min ? std::round(v.y) : y_min;
    }

    glm::vec3 tlc(x_min, y_min, 1), blc(x_min, y_max, 1), brc(x_max, y_max, 1), trc(x_max, y_min, 1);

    d_aabb = std::vector<glm::vec3>
    {
        tlc, blc, brc, trc
    };

    float aabb_x = d_aabb[0].x;
    float aabb_y = d_aabb[0].y;
    float aabb_w = d_aabb[3].x - d_aabb[0].x;
    float aabb_h = d_aabb[1].y - d_aabb[0].y;

    float move_x = 0, move_y = 0;

    if(aabb_x < 0)
    {
        move_x = -(aabb_x);
    }

    if(aabb_y < 0)
    {
        move_y = -(aabb_y);
    }

    if(aabb_x + aabb_w > Context::SIContext()->width())
    {
        move_x = (aabb_w + aabb_x) - Context::SIContext()->width();
    }

    if(aabb_y + aabb_h > Context::SIContext()->height())
    {
        move_y = (aabb_h + aabb_y) - Context::SIContext()->height();
    }

    for (int i = 0; i < temp.size(); ++i)
    {
        temp[i].x += move_x;
        temp[i].y += move_y;
    }

    if (d_name != "__ Painter __" && temp.size() != SI_CONTOUR_STEPCOUNT)
    {
        RegionResampler::resample(d_contour, temp);
    }
    else
    {
        d_contour = temp;
    }

    d_recompute_mask = true;
}

// by name
void PySIEffect::__create_region__(const std::vector<glm::vec3>& contour, const std::string& name, bool as_selector, bp::dict& kwargs)
{
    Context::SIContext()->register_new_region_via_name(contour, name, as_selector, kwargs);
}

// by name
void PySIEffect::__create_region__(const bp::list& contour, const std::string& name, bool as_selector, bp::dict& kwargs)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

    HANDLE_PYTHON_CALL(PY_ERROR, "Passed list is not trivially convertible to SIGRun's internal datastructures. Try [[x1, y1], [x2, y2], ..., [xn, yn]].",
        for(uint32_t i = 0; i < bp::len(contour); ++i)
        {
            float x = bp::extract<float>(contour[i][0]);
            float y = bp::extract<float>(contour[i][1]);

            _contour.emplace_back(x, y, 1);
        }
    )

    if(!_contour.empty())
        __create_region__(_contour, name, as_selector, kwargs);
}

// by type
void PySIEffect::__create_region__(const bp::list& contour, int effect_type, bp::dict& kwargs)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

    HANDLE_PYTHON_CALL(PY_ERROR, "Passed list is not trivially convertible to SIGRun's internal datastructures. Try [[x1, y1], [x2, y2], ..., [xn, yn]].",
       for(uint32_t i = 0; i < bp::len(contour); ++i)
       {
           float x = bp::extract<float>(contour[i][0]);
           float y = bp::extract<float>(contour[i][1]);

           _contour.emplace_back(x, y, 1);
       }
    )

    if(!_contour.empty())
        Context::SIContext()->register_new_region_via_type(_contour, effect_type, kwargs);
}

// by object
void PySIEffect::__create_region__(const bp::object &object, const bp::dict &qml)
{
    Context::SIContext()->register_new_region_from_object(object, qml);
}

// by class
void PySIEffect::__create_region__(const bp::list &contour, bp::object &clazz, bp::dict &kwargs)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

    for(uint32_t i = 0; i < bp::len(contour); ++i)
    {
        float x = bp::extract<float>(contour[i][0]);
        float y = bp::extract<float>(contour[i][1]);

        _contour.emplace_back(x, y, 1);
    }

    Context::SIContext()->register_region_via_class_object(_contour, clazz, kwargs);
}

bp::list PySIEffect::__current_regions__()
{
    bp::list ret;

    auto& regions = Context::SIContext()->region_manager()->regions();

    for(auto& r: regions)
        ret.append(r->raw_effect());

    return ret;
}

void PySIEffect::__add_multiple_regions__(const bp::list& contours, const std::string& effect_name, bp::dict& kwargs)
{
    for (int i = 0; i < bp::len(contours); i++)
    {
        const std::vector<glm::vec3>& contour = bp::extract<std::vector<glm::vec3>>(contours[i]);
        Print::print(kwargs);
        Context::SIContext()->register_new_region_via_name(contour, effect_name, false, kwargs);
    }
}

void PySIEffect::__update_transform__(int32_t delta_x, int32_t delta_y)
{
    d_transform_x += delta_x;
    d_transform_x += delta_y;
}

std::vector<std::string> PySIEffect::__available_plugins_by_name__()
{
    return Context::SIContext()->available_plugins_names();
}

bp::tuple PySIEffect::__context_dimensions__()
{
    return bp::make_tuple(Context::SIContext()->width(), Context::SIContext()->height());
}

void PySIEffect::__assign_effect__(const std::string& sender, const std::string& effect_name, const std::string& effect_display_name, bp::dict& kwargs)
{
    Context::SIContext()->set_effect(sender, effect_name, kwargs);
}

bp::dict PySIEffect::__qml_data_keys_and_types__()
{
    bp::dict ret;

    for(auto& k0: d_data.keys())
    {
        std::string k = k0.toStdString();

        if(std::string(d_data[k0].typeName()) == "int")
        {
            ret[k] = SI_DATA_TYPE_INT;
            continue;
        }

        if(std::string(d_data[k0].typeName()) == "QString")
        {
            ret[k] = SI_DATA_TYPE_STRING;
            continue;
        }

        if(std::string(d_data[k0].typeName()) == "float")
        {
            ret[k] = SI_DATA_TYPE_FLOAT;
            continue;
        }

        if(std::string(d_data[k0].typeName()) == "bool")
        {
            ret[k] = SI_DATA_TYPE_BOOL;
            continue;
        }
    }

    return ret;
}

bp::list PySIEffect::__excluded_plugins__()
{
    auto& plugins = Context::SIContext()->excluded_plugins();

    bp::list l;

    for(auto& s: plugins)
        l.append(s);

    return l;
}

bp::list PySIEffect::__conditional_variables__()
{
    auto& vars = Context::SIContext()->conditional_variables();

    bp::list l;

    for(auto& s: vars)
        l.append(s);

    return l;
}

void PySIEffect::__set_drawing_additions__(const bp::list &drawing_additions)
{
    d_drawing_additions.clear();

    for(int i = 0; i < bp::len(drawing_additions); ++i)
    {
        std::vector<std::vector<glm::vec3>> shapes;
        for(int k = 0; k < bp::len(drawing_additions[i]); ++k)
        {
            std::vector<glm::vec3> shape_part;

            for(int l = 0; l < bp::len(drawing_additions[i][k]); ++l)
            {
                int x = bp::extract<float>(drawing_additions[i][k][l][0]);
                int y = bp::extract<float>(drawing_additions[i][k][l][1]);

                shape_part.emplace_back(x, y, 1);
            }

            shapes.push_back(shape_part);
        }

        d_drawing_additions.push_back(shapes);
    }

    d_data_changed = true;
}

bp::list PySIEffect::__drawing_additions__()
{
    bp::list ret;

    for(std::vector<std::vector<glm::vec3>>& shape: d_drawing_additions)
    {
        bp::list shapes;
        for(std::vector<glm::vec3>& shape_part: shape)
        {
            bp::list shape_parts;
            for(glm::vec3& p: shape_part)
            {
                bp::list point;

                point.append(p.x);
                point.append(p.y);

                shape_parts.append(point);
            }

            shapes.append(shape_parts);
        }

        ret.append(shapes);
    }

    return ret;
}

std::vector<std::vector<std::vector<glm::vec3>>> &PySIEffect::drawing_additions()
{
    return d_drawing_additions;
}

std::vector<std::vector<std::string>> PySIEffect::get_collisions()
{
    return d_collisions;
}

void PySIEffect::set_collisions(const std::vector<std::vector<std::string>> &collisions)
{
    d_collisions.clear();
    d_collisions = collisions;
}

bool PySIEffect::evaluate_enveloped() const
{
    return d_evaluate_enveloped;
}

bool PySIEffect::is_enveloped() const
{
    return d_is_enveloped;
}

bp::dict PySIEffect::__selected_effects_by_cursor_id__ ()
{
    const auto& selected_effects_by_id = Context::SIContext()->selected_effects_by_cursor_id();

    bp::dict ret;
    for(auto& [k, v]: selected_effects_by_id)
        ret[k] = v.attr(v.attr(SI_INTERNAL_NAME)).attr("regionname");

    return ret;
}

void PySIEffect::__notify__(const bp::object &msg, const int type)
{
    if(Context::SIContext()->physical_environment())
    {
        if(type == SI_DATA_TYPE_STRING)
        {
            const std::string message = bp::extract<std::string>(msg);
            Context::SIContext()->physical_environment()->send(message);
        }
    }
}

void PySIEffect::__set_cursor_stroke_width_by_cursorid__(const std::string &cursor_id, int width)
{
    Context::SIContext()->update_cursor_stroke_width_by_cursor_id(cursor_id, width);
}

void PySIEffect::__set_cursor_stroke_color_by_cursorid__(const std::string &cursor_id, const glm::vec4& color)
{
    Context::SIContext()->update_cursor_stroke_color_by_cursor_id(cursor_id, color);
}

void PySIEffect::__current_tangible_selection__(const std::string &effect_to_assign, const std::string &effect_display_name, const std::string &effect_texture, bp::dict &kwargs)
{
    Context::SIContext()->tangible_manager()->set_current_pen_selection(effect_to_assign, effect_display_name, effect_texture, kwargs);
}

void PySIEffect::__on_destroy__() {
}

void PySIEffect::__click_mouse__(float x, float y) {
    Context::SIContext()->click_mouse(x, y);
}

void PySIEffect::__dbl_click_mouse__(float x, float y) {
    Context::SIContext()->dbl_click_mouse(x, y);
}