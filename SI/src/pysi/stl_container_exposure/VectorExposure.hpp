

#ifndef SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
#define SITEST_PYTHONICVECTORINDEXINGSUITE_HPP

#include <boost/python.hpp>
#include <vector>
#include <debug/Print.hpp>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/core/demangle.hpp>
#include <glm/glm.hpp>

namespace bp = boost::python;

void IndexError()
{
    PyErr_SetString(PyExc_IndexError, "Index out of range");
}

template <typename T>
class VectorExposure
{
public:
    typedef typename T::value_type V;

    static V& get(T& x, int i)
    {
        if(i < 0)
            i += x.size();

        if(i >= 0 && i < x.size())
            return x[i];

        IndexError();
    }

    static void set(T& x, int i, V const& v)
    {
        if(i < 0)
            i += x.size();

        if(i >= 0 && i<x.size())
            x[i] = v;
        else
            IndexError();
    }

    static void del(T& x, int i)
    {
        if(i < 0)
            i += x.size();

        if(i >= 0 && i < x.size())
            x.erase(x.begin() + i);
        else IndexError();
    }

    static void add(T& x, V const& v)
    {
        x.push_back(v);
    }

    static bool in(T const& x, V const& v)
    {
        return std::find(x.begin(), x.end(), v) != x.end();
    }

    static int index(T const& x, V const& v)
    {
        int i = 0;

        for(typename T::const_iterator it = x.begin(); it!=x.end(); ++it, ++i)
            if(*it == v)
                return i;

        return -1;
    }
};

class VectorExposureVec3
{
public:
    static boost::shared_ptr<std::vector<glm::vec3>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<glm::vec3>>();

        if(bp::len(list) > 0)
        {
            if(bp::extract<bp::list>(list[0]).check())
                apply_lists_of_lists(self, list);
            else
                apply_list(self, list);
        }

        return self;
    }

    static void add(std::vector<glm::vec3>& self, const bp::list& list)
    {
        if(bp::len(list) > 0)
        {
            float x = bp::extract<float>(list[0]);
            float y = bp::extract<float>(list[1]);

            self.emplace_back(x, y, 1);
        }
    }

    static void set(std::vector<glm::vec3>& self, int i, const bp::list& list)
    {
        if(i < 0)
            i += self.size();

        if(i >= 0 && i < self.size())
        {
            float x = bp::extract<float>(list[0]);
            float y = bp::extract<float>(list[1]);

            self[i] = glm::vec3(x, y, 1);
        }
        else
            IndexError();
    }

    static const std::string repr(std::vector<glm::vec3>& self)
    {
        std::string ret = "List of Points: [";

        for(int i = 0; i < self.size(); ++i)
        {
            ret += ("[" + std::to_string(self[i].x) + ", " + std::to_string(self[i].y) + "]");

            if(i != self.size() - 1)
                ret += ", ";
        }

        ret += "]";

        return ret;
    }

private:
    static void apply_lists_of_lists(boost::shared_ptr<std::vector<glm::vec3>>& self, const bp::list& list)
    {
        for(int i = 0; i < bp::len(list); ++i)
        {
            const bp::list& l = bp::extract<bp::list>(list[i]);
            apply_list(self, l);
        }
    }

    static void apply_list(boost::shared_ptr<std::vector<glm::vec3>>& self, const bp::list& list)
    {
        float x = bp::extract<float>(list[0]);
        float y = bp::extract<float>(list[1]);

        self->emplace_back(x, y, 1);
    }
};

class VectorExposureString
{
public:
    static boost::shared_ptr<std::vector<std::string>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<std::string>>();

        for(int i = 0; i < bp::len(list); ++i)
        {
            const std::string& s = bp::extract<std::string>(list[i]);
            self->emplace_back(s);
        }

        return self;
    }

    static void add(std::vector<std::string>& self, const std::string& s)
    {
        self.push_back(s);
    }

    static void set(std::vector<std::string>& self, int i, const std::string& s)
    {
        if(i < 0)
            i += self.size();

        if(i >= 0 && i < self.size())
            self[i] = s;
        else
            IndexError();
    }

    static const std::string repr(std::vector<std::string>& self)
    {
        std::string ret = "List of strings: [";

        for(int i = 0; i < self.size(); ++i)
        {
            ret += self[i];

            if(i != self.size() - 1)
                ret += ", ";
        }

        ret += "]";

        return ret;
    }
};

class VectorExposureLinkRelation
{
public:
    static boost::shared_ptr<std::vector<LinkRelation>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<LinkRelation>>();


        if(bp::len(list) > 0)
        {
            if(bp::extract<bp::list>(list[0]).check())
                apply_lists_of_lists(self, list);
            else
                apply_list(self, list);
        }

        return self;
    }

    // weird stuff happens with strings; scrambles data; unclear why
    static void add(std::vector<LinkRelation>& self, const bp::list& list)
    {
        if(bp::len(list) > 0)
        {
            const char* sender = bp::extract<char*>(list[0]);
            const char* sender_attrib = bp::extract<char*>(list[1]);
            const char* recv = bp::extract<char*>(list[2]);
            const char* recv_attrib = bp::extract<char*>(list[3]);

            self.emplace_back(sender, sender_attrib, recv, recv_attrib);
        }
    }

    static void set(std::vector<LinkRelation>& self, int i, const bp::list& list)
    {
        if(i < 0)
            i += self.size();

        if(i >= 0 && i < self.size())
        {
            if(bp::len(list) > 0)
            {
                const char* sender = bp::extract<char*>(list[0]);
                const char* sender_attrib = bp::extract<char*>(list[1]);
                const char* recv = bp::extract<char*>(list[2]);
                const char* recv_attrib = bp::extract<char*>(list[3]);

                self[i] = LinkRelation(sender, sender_attrib, recv, recv_attrib);
            }
        }
        else
            IndexError();
    }

    static const std::string repr(std::vector<LinkRelation>& self)
    {
        std::string ret = "List of LinkRelations: [";

        for(int i = 0; i < self.size(); ++i)
        {
            ret += ("[" + self[i].sender + ", " + self[i].sender_attrib + ", " + self[i].recv + ", " + self[i].recv_attrib + "]");

            if(i != self.size() - 1)
                ret += ", ";
        }

        ret += "]";

        return ret;
    }

private:
    static void apply_lists_of_lists(boost::shared_ptr<std::vector<LinkRelation>>& self, const bp::list& list)
    {
        for(int i = 0; i < bp::len(list); ++i)
        {
            const bp::list& l = bp::extract<bp::list>(list[i]);
            apply_list(self, l);
        }
    }

    static void apply_list(boost::shared_ptr<std::vector<LinkRelation>>& self, const bp::list& list)
    {
        const std::string& sender = bp::extract<std::string>(list[0]);
        const std::string& sender_attrib = bp::extract<std::string>(list[1]);
        const std::string& recv = bp::extract<std::string>(list[2]);
        const std::string& recv_attrib = bp::extract<std::string>(list[3]);

        self->emplace_back(sender, sender_attrib, recv, recv_attrib);
    }
};

template <typename VectorExposureType, typename VectorType>
bp::class_<VectorType> create_vector(const char* name)
{
    return bp::class_<VectorType>(name, bp::no_init)
            .def("__init__", bp::make_constructor(&VectorExposureType::init, bp::default_call_policies(), (bp::arg("list")=bp::list())))
            .def("__len__", &VectorType::size)
            .def("clear", &VectorType::clear)
            .def("append", &VectorExposure<VectorType>::add)
            .def("append", &VectorExposureType::add)
            .def("__getitem__", &VectorExposure<VectorType>::get, bp::return_value_policy<bp::reference_existing_object>())
            .def("__setitem__", &VectorExposure<VectorType>::set)
            .def("__setitem__", &VectorExposureType::set)
            .def("__delitem__", &VectorExposure<VectorType>::del)
            .def("__iter__", bp::iterator<VectorType>())
            .def("__contains__", &VectorExposure<VectorType>::in)
            .def("index", &VectorExposure<VectorType>::index)
            .def("__repr__", &VectorExposureType::repr)
            ;
}

#endif //SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
