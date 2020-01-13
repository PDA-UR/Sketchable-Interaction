
#include "PythonInvoker.hpp"

PythonInvoker::PythonInvoker()
= default;

PythonInvoker::~PythonInvoker()
= default;

int PythonInvoker::invoke_collision_event_function(bp::object &self, bp::object &other, const std::string &function_name)
{
    const bp::dict& d = bp::extract<bp::dict>(self.attr("cap_recv"));
    const bp::dict& o = bp::extract<bp::dict>(other.attr("cap_emit"));

    const bp::list& items = o.items();
    for(int i = 0; i < bp::len(items); ++i)
    {
        const bp::object& key = items[i][0];

        if (d.has_key(key))
        {
            try
            {
                const bp::object& t = o[key][function_name](self);

                if(t.is_none())
                    return bp::extract<int>(d[key][function_name]());
                else
                {
                    if(bp::extract<bp::tuple>(t).check())
                        return bp::extract<int>(d[key][function_name](*t));
                    else
                        return bp::extract<int>(d[key][function_name](t));
                }
            }
            HANDLE_PYTHON_ERROR
        }
    }

    return -1;
}

int PythonInvoker::invoke_linking_event_function(bp::object &self, const std::string &capability,  const bp::list &args)
{
    try
    {
        return bp::extract<int>(self.attr("cap_link")[capability][1](args));
    }
    HANDLE_PYTHON_ERROR

    return -1;
}

const bp::list PythonInvoker::retrieve_linking_event_args(const bp::object& self, const std::string& capability)
{
    try
    {
        return bp::extract<bp::list>(self.attr("cap_link")[capability][0]());
    }
    catch (const bp::error_already_set&)
    {
        handle_python_error();
    }

    return bp::list();
}

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
