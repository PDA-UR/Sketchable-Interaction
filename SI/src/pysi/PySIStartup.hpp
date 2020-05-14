

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
    static void create_region_by_id(const bp::list& shape, int effect_type, bp::dict& kwargs);
};

#endif //SITEST_PYSISTARTUP_HPP
