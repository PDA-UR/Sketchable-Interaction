
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>
#include "PySIStartup.hpp"
#include <numeric>
#include <algorithm>
#include <execution>
#include <debug/Print.hpp>

PySIStartup::PySIStartup() = default;
PySIStartup::~PySIStartup() = default;

bp::tuple PySIStartup::context_dimensions()
{
    return bp::make_tuple(Context::SIContext()->width(), Context::SIContext()->height());
}

void PySIStartup::create_region_by_type(const bp::list &contour, int effect_type, bp::dict &kwargs)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

    for(uint32_t i = 0; i < bp::len(contour); ++i)
    {
        float x = bp::extract<float>(contour[i][0]);
        float y = bp::extract<float>(contour[i][1]);

        _contour.emplace_back(x, y, 1);
    }

    Context::SIContext()->register_new_region_via_type(_contour, effect_type, kwargs);
}

void PySIStartup::create_region_by_name(const bp::list &contour, const std::string &name, bp::dict &kwargs)
{
    std::vector<glm::vec3> _contour;
    _contour.reserve(bp::len(contour));

    for(uint32_t i = 0; i < bp::len(contour); ++i)
    {
        float x = bp::extract<float>(contour[i][0]);
        float y = bp::extract<float>(contour[i][1]);

        _contour.emplace_back(x, y, 1);
    }

    Context::SIContext()->register_new_region_via_name(_contour, name, false, kwargs);
}

void PySIStartup::create_region_by_class(const bp::list &contour, bp::object &clazz, bp::dict &kwargs)
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

void PySIStartup::logger_quench_messages_from_class(const std::string &class_name)
{
    std::string to_quench = class_name;

    std::transform(to_quench.begin(), to_quench.end(), to_quench.begin(), ::toupper);

    Log::quench(to_quench);
}



void PySIStartup::logger_unquench_messages_from_class(const std::string &class_name)
{
    std::string to_unquench = class_name;

    std::transform(to_unquench.begin(), to_unquench.end(), to_unquench.begin(), ::toupper);

    Log::unquench(to_unquench);
}

void PySIStartup::logger_log(bool flag)
{
    Log::__DEBUG__ = flag;
}

void PySIStartup::logger_set_log_output(int32_t flags)
{
    Log::WHERE = flags;
}

void PySIStartup::enable(int32_t flags)
{
    Context::SIContext()->enable(flags);
}

void PySIStartup::disable(int32_t flags)
{
    Context::SIContext()->disable(flags);
}