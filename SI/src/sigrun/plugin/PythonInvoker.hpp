
#ifndef SITEST_PYTHONINVOKER_HPP
#define SITEST_PYTHONINVOKER_HPP

#include <boost/python.hpp>
#include "debug/Print.hpp"

namespace bp = boost::python;

class PythonInvoker
{
public:
    PythonInvoker();
    ~PythonInvoker();

    template<typename T>
    T invoke_extract_attribute(const bp::object &self, const std::string& attribute_name)
    {
        try
        {
            return bp::extract<T>(self.attr(attribute_name.c_str()));
        }
        catch (const bp::error_already_set &)
        {
            handle_python_error();
        }

        return T();
    }

    template<typename T>
    void invoke_set_attribute(bp::object &self, std::string &attribute_name, T& value, bool is_pointer=false)
    {
//        try
//        {
//            if(is_pointer)
//                self.attr(attribute_name.c_str()) = bp::object(bp::ptr(&value));
//            else
//                self.attr(attribute_name.c_str()) = bp::object(1);
//        }
//        catch (const bp::error_already_set&)
//        {
//            handle_python_error();
//        }
    }

    template<typename T>
    T invoke_function(bp::object &self, const std::string &function_name, bp::object &other)
    {
        try
        {
            bp::object obj = self.attr(function_name.c_str())(other);

            return bp::extract<T>(obj);
        }
        catch (const bp::error_already_set&)
        {
            handle_python_error();
        }

        return T();
    }

    int invoke_collision_event_function(bp::object& self, bp::object& other, const std::string& function_name);

private:
    void handle_python_error();

protected:
};


#endif //SITEST_PYTHONINVOKER_HPP
