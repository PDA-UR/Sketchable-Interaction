
#include "PySIEffect.hpp"

#include <sigrun/context/Context.hpp>
#include <sigrun/region/RegionResampler.hpp>
#include <sigrun/util/Dollar1GestureRecognizer.hpp>
#include <boost/python/numpy.hpp>
#include <QDebug>
#include <QVideoFrame>
#include <QImage>
#include <pysi/pickling/PickleSuits.hpp>

namespace bp = boost::python;

PySIEffect::PySIEffect(const std::vector<glm::vec3>& contour, const std::string& uuid, const std::string& tex_path, const bp::dict& kwargs)
    : d_data_changed(false), d_uuid(uuid)
{
    d_regions_marked_for_registration.reserve(10);
    d_link_relations.reserve(100);
    d_contour.reserve(SI_CONTOUR_STEPCOUNT);
    d_aabb.reserve(4);

    std::vector<glm::vec3> temp;

    Recognizer r;
    r.recognize(temp, contour);

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

    RegionResampler::resample(d_contour, temp);
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
    Context::SIContext()->register_link_event_emission(_UUID_, sender, linking_action, args);
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
        default:
            break;
    }
}

std::vector<std::string>& PySIEffect::regions_for_registration()
{
    return d_regions_marked_for_registration;
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
            if (value.is_none())
                return;

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
            img.convertTo(QVideoFrame::imageFormatFromPixelFormat(QVideoFrame::Format_RGB32));
            d_data[QString(key.c_str())] = img;
            break;
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

void PySIEffect::set_shape(const std::vector<glm::vec3>& shape)
{
    if (shape.empty() || d_name.empty())
        return;

    d_contour.clear();
    std::vector<glm::vec3> temp, smoothed;

    if(d_name != "__ ConveyorBelt __")
    {
        Recognizer r;
        r.recognize(temp, shape);
    }
    else
    {
        temp = shape;
    }

    int32_t x_min = INT32_MAX;
    int32_t x_max = INT32_MIN;
    int32_t y_min = INT32_MAX;
    int32_t y_max = INT32_MIN;

    for(const auto& v: temp)
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

    RegionResampler::resample(d_contour, temp);

    d_recompute_mask = true;
}

void PySIEffect::__create_region__(const std::vector<glm::vec3>& contour, const std::string& name, bool as_selector, bp::dict& kwargs)
{
    Context::SIContext()->register_new_region_via_name(contour, name, as_selector, kwargs);
}

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
    Context::SIContext()->set_effect(sender, effect_name, effect_display_name, kwargs);
}
