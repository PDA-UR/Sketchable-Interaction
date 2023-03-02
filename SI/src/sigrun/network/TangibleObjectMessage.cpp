

#include "TangibleObjectMessage.hpp"
#include <sigrun/context/Context.hpp>


TangibleObjectMessage::~TangibleObjectMessage() = default;

TangibleObjectMessage::TangibleObjectMessage(int32_t id, const std::vector<glm::vec3> &shape, const std::string &plugin_identifier,
                               float x, float y, const glm::vec4& color, bool is_click, bool is_drag, bool is_dbl_click, bool is_touch, bool is_alive,
                               const std::vector<int>& links, int tracker_dimension_x, int tracker_dimension_y):
                               d_id(id),
                               d_shape(shape),
                               d_plugin_identifier(plugin_identifier),
                               d_x(x),
                               d_y(y),
                               d_color(color),
                               d_is_click(is_click),
                               d_is_drag(is_drag),
                               d_is_dbl_click(is_dbl_click),
                               d_is_touch(is_touch),
                               d_is_alive(is_alive),
                               d_links(links),
                               d_tracker_dimensions(glm::vec2(tracker_dimension_x, tracker_dimension_y))
{}

void TangibleObjectMessage::send()
{
    Context::SIContext()->tangible_manager()->receive(this);
}

const int TangibleObjectMessage::id() const
{
    return d_id;
}

const std::vector<glm::vec3> &TangibleObjectMessage::shape() const
{
    return d_shape;
}

const std::string &TangibleObjectMessage::plugin_identifier() const
{
    return d_plugin_identifier;
}

const float TangibleObjectMessage::x() const
{
    return d_x;
}

const float TangibleObjectMessage::y() const
{
    return d_y;
}

const glm::vec4 &TangibleObjectMessage::color() const
{
    return d_color;
}

const bool TangibleObjectMessage::is_click() const
{
    return d_is_click;
}

const bool TangibleObjectMessage::is_drag() const
{
    return d_is_drag;
}

const bool TangibleObjectMessage::is_dbl_click() const
{
    return d_is_dbl_click;
}

const bool TangibleObjectMessage::is_touch() const
{
    return d_is_touch;
}

const bool TangibleObjectMessage::is_alive() const
{
    return d_is_alive;
}

const std::vector<int> &TangibleObjectMessage::links() const
{
    return d_links;
}

const glm::vec2 &TangibleObjectMessage::tracker_dimensions() const
{
    return d_tracker_dimensions;
}

const int TangibleObjectMessage::tracker_dimension_x() const
{
    return d_tracker_dimensions.x;
}

const int TangibleObjectMessage::tracker_dimension_y() const
{
    return d_tracker_dimensions.y;
}

const bool TangibleObjectMessage::has_links() const
{
    return !d_links.empty();
}
