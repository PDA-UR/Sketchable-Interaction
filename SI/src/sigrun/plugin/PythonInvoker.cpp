
#include "PythonInvoker.hpp"

PythonInvoker::PythonInvoker()
= default;

PythonInvoker::~PythonInvoker()
= default;

void PythonInvoker::handle_python_error()
{
    PyObject *exc,*val,*tb;
    PyErr_Fetch(&exc, &val, &tb);
    PyErr_NormalizeException(&exc, &val, &tb);

    bp::handle<> hexc(exc), hval(bp::allow_null(val)), htb(bp::allow_null(tb));

    bp::object formatted_list, formatted;
    bp::object traceback(bp::import("traceback"));

    if (!tb) {
        bp::object format_exception_only(traceback.attr("format_exception_only"));
        formatted_list = format_exception_only(hexc, hval);
    } else {
        bp::object format_exception(traceback.attr("format_exception"));
        formatted_list = format_exception(hexc, hval, htb);
    }

    std::string error = bp::extract<std::string>(bp::str("\n").join(formatted_list));

    Print::print(error);

    bp::handle_exception();
    PyErr_Clear();
}
