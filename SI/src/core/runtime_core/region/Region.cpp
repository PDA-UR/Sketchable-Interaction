

#include "Region.hpp"

namespace SI
{
    Region::Region()
    {

    }

    Region::Region(int id) : d_id(id)
    {

    }

    Region::~Region()
    {

    }

    void Region::set_effect(bp::object effect)
    {
        d_effect = effect;
    }

    void Region::set_shape(const std::vector<glm::vec2> &shape)
    {
        d_shape = shape;
    }

    const std::string &Region::name() const
    {
        return d_name;
    }

    const int Region::id() const
    {
        return d_id;
    }

    const std::vector<glm::vec2> &Region::shape() const
    {
        return d_shape;
    }

    void Region::on_enter()
    {
        d_effect.attr("on_enter")();
    }

    void Region::on_continuous()
    {
        d_effect.attr("on_continuous")();
    }

    void Region::on_leave()
    {
        d_effect.attr("on_leave")();
    }
}