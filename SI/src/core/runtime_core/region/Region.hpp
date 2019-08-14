

#ifndef SI_REGION_HPP
#define SI_REGION_HPP

#include <boost/python.hpp>
#include "core/math/si_math.hpp"

namespace bp = boost::python;

namespace SI
{
    class Region
    {
    public:
        Region();

        explicit Region(int id);

        ~Region();

        void set_effect(bp::object effect);

        void set_shape(const std::vector<glm::vec2> &shape);

        const std::string &name() const;

        const int id() const;

        const std::vector<glm::vec2> &shape() const;

        void on_enter();

        void on_continuous();

        void on_leave();

    private:
        std::vector<glm::vec2> d_shape;
        bp::object d_effect;
        std::string d_name;
        int d_id;
    };
}

#endif //SI_REGION_HPP
