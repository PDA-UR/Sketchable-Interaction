
#ifndef SITEST_PYTHONINVOKER_HPP
#define SITEST_PYTHONINVOKER_HPP

#include <boost/python.hpp>
#include <sigrun/log/Log.hpp>

namespace bp = boost::python;

#define HANDLE_PYTHON_CALL(...) { \
    try\
    {\
        __VA_ARGS__\
    }\
    catch (const bp::error_already_set&)\
    {\
        PyObject *exc,*val,*tb;\
        PyErr_Fetch(&exc, &val, &tb);\
        PyErr_NormalizeException(&exc, &val, &tb);\
        bp::handle<> hexc(exc), hval(bp::allow_null(val)), htb(bp::allow_null(tb));\
        bp::object formatted_list, formatted;\
        bp::object traceback(bp::import("traceback"));\
        if (!tb) {\
        bp::object format_exception_only(traceback.attr("format_exception_only"));\
        formatted_list = format_exception_only(hexc, hval);\
        } else {\
        bp::object format_exception(traceback.attr("format_exception"));\
        formatted_list = format_exception(hexc, hval, htb);\
        }\
        std::string error = bp::extract<std::string>(bp::str("\n").join(formatted_list));\
        ERROR(error);\
        bp::handle_exception();\
        PyErr_Clear();\
    }\
}

#endif //SITEST_PYTHONINVOKER_HPP
