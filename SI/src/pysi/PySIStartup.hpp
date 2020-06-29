

#ifndef SITEST_PYSISTARTUP_HPP
#define SITEST_PYSISTARTUP_HPP

#include <sigrun/SITypes.hpp>
#include <sigrun/SIObject.hpp>
#include <sigrun/log/Log.hpp>
#include <glm/glm.hpp>
#include <sigrun/util/Util.hpp>
#include<boost/python.hpp>
#include <memory>

class PySIStartup: public SIObject
{ PYSI
public:
    PySIStartup();
    ~PySIStartup();

    static bp::tuple context_dimensions();
    static void create_region_by_type(const bp::list& shape, int effect_type, bp::dict& kwargs);
    static void create_region_by_name(const bp::list &contour, const std::string& name, bp::dict &kwargs);
    static void create_region_by_class(const bp::list &contour, bp::object& clazz, bp::dict &kwargs);

    static void logger_quench_messages_from_class(const std::string& class_name);
    static void logger_unquench_messages_from_class(const std::string& class_name);
    static void logger_log(bool flag);
    static void logger_set_log_output(int32_t flags);
    static void enable(int32_t flags);
    static void disable(int32_t flags);
};

#endif //SITEST_PYSISTARTUP_HPP
