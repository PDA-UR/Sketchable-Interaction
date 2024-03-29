

#ifndef SITEST_PYTHONICVECTORINDEXINGSUITE_HPP
#define SITEST_PYTHONICVECTORINDEXINGSUITE_HPP

#include <boost/python.hpp>
#include <vector>
#include <algorithm>
#include <numeric>
#include <execution>
#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>
#include <glm/glm.hpp>
#include <pysi/PySIEffect.hpp>

namespace bp = boost::python;

/**
 * @class VectorExposure
 * @brief VectorExposure class providing the interface for exposing STL vectors to the python3 bindings (PySI) in a pythonic way
 *
 * @tparam T the STL vector to be exposed
 */
template <typename T>
class VectorExposure
{
public:

    /**
     * @tparam V the type of variable the STL vector T contains
     */
    typedef typename T::value_type V;

    /**
     * @brief returns a reference to a value of type V at index i contained in STL vector T
     * @details
     *      Returns a reference to a value of type V at index i contained in STL vector T.
     *      Also allows pythonic accsess to values via negative indices
     *
     * @param[in] self the STL vector of type T with values of type V
     * @param[in] index the index of the value to be returned by reference
     *
     * @return a reference to the value in self at the given index
     */
    inline static V& get(T& self, uint32_t index)
    {
        if(index < 0)
            index += self.size();

        return self[index];
    }

    /**
     * @brief enables pythonic slicing of exposed STL vector T with values of type V

     * @param[in] self the STL vector of type T and values of type V to be sliced
     * @param[in] i the slicing parameters
     *
     * @return a deep copy to a STL vector of type T containing the elements of self which remained after slicing
     */
    static T get_slice(T const& self, const bp::slice& i)
    {
        T result;

        bp::slice::range<typename T::const_iterator> range = i.get_indices(self.begin(), self.end());

        for (; range.start != range.stop; std::advance(range.start, range.step))
            result.push_back(*range.start);

        result.push_back(*range.start);

        return result;
    }

    /**
     * @brief sets the value at the given index of self to the given value
     *
     * @param[in, out] self the vector of type T with values of type V which value at given index is to be changed
     * @param[in] index the index of the value to be changed
     * @param[in] value the new value to be set at the given index in self
     */
    inline static void set(T& self, uint32_t index, V const& value)
    {
        if(index < 0)
            index += self.size();

        if(index >= 0 && index < self.size())
            self[index] = value;
    }

    /**
     * @brief removes the value at the given index, therefore reducing the vectors size by one
     *
     * @param[in, out] self the STL vector of type T with values of type V to have a value deleted
     * @param[in] index the index of the value to be deleted in self
     */
    inline static void del(T& self, uint32_t index)
    {
        if(index < 0)
            index += self.size();

        if(index >= 0 && index < self.size())
            self.erase(self.begin() + index);
    }

    /**
     * @brief add a value of type V to the back of a STL vector of type T
     *
     * @param[in, out] self the STL vector to receive the value
     * @param[in] value the value to be pushed to the back of self
     */
    inline static void add(T& self, V const& value)
    {
        self.push_back(value);
    }

    /**
     * @brief check if a STL vector of type T with values of type V contains a value of type V
     *
     * @param[in] self the STL vector to be checked whether it contains the given value
     * @param[in] value the value to be checked whether it is contained in self
     *
     * @return true if self contains the value and false else
     */
    constexpr static bool in(T const& self, V const& value)
    {
        return VEC_CONTAINS(self, value);
    }

    /**
     * @brief returns the index of a value of type in the STL vector of type T with values of type V
     * @details
     *      Return the index of a value in the STL vector or -1 if the value is not present in the vector
     * @param[in] self the target vector
     * @param[in] value the value which index is to be returned
     *
     * @return the index of the value in self or -1 if the value is not contained by self
     */
    constexpr static int index(T const& self, V const& value)
    {
        int i = 0;

        for(typename T::const_iterator it = self.begin(); it != self.end(); ++it, ++i)
            if(*it == value)
                return i;

        return -1;
    }
};

/**
 * @class VectorExposureVec3
 * @brief Special wrapper class for VectorExposure handling vectors of glm::vec3
 */
class VectorExposureVec3
{
public:

    /**
     * @brief exposed constructor to create a vector of glm::vec3 based on a python list
     *
     * @param[in] list a list containing either further lists of three floats each representing coordinates.
     *             Or containing three floats representing one point.
     *
     * @return a reference to a new std::vector<glm::vec3> exposed to python containing the values of list
     */
    static boost::shared_ptr<std::vector<glm::vec3>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<glm::vec3>>();
        self->reserve(bp::len(list));

        if(bp::len(list) > 0)
        {
            self->reserve(bp::len(list));

            if(bp::extract<bp::list>(list[0]).check())
                apply_lists_of_lists(self, list);
            else
                apply_list(self, list);
        }

        return self;
    }

    /**
     * @brief adds a new point to the given vector
     *
     * @param[in, out] self a vector of points to receive a new point
     * @param list[in] a python list containing a points coordinates to be added to self
     */
    inline static void add(std::vector<glm::vec3>& self, const bp::list& list)
    {
        if(bp::len(list) > 0)
        {
            float x = bp::extract<float>(list[0]);
            float y = bp::extract<float>(list[1]);

            self.emplace_back(x, y, 1);
        }
    }

    /**
    * @brief sets the value at the given index of self to the given value
    *
    * @param[in, out] self the vector which value at given index is to be changed
    * @param[in] index the index of the value to be changed
    * @param[in] list the list containing point coordinates to be applied to the point at the given index
    */
    inline static void set(std::vector<glm::vec3>& self, uint32_t index, const bp::list& list)
    {
        if(index < 0)
            index += self.size();

        if(index >= 0 && index < self.size())
        {
            float x = bp::extract<float>(list[0]);
            float y = bp::extract<float>(list[1]);

            self[index] = glm::vec3(x, y, 1);
        }
    }

    /**
     * @brief returns the vectors representation as a string (__repr__ in python)
     *
     * @param[in] self the vector which data is to be presented in a readable way
     *
     * @return the std::string containing the vector's representation
     */
    static const std::string repr(std::vector<glm::vec3>& self)
    {
        return std::transform_reduce(std::execution::par, self.begin(), self.end(), std::string("["), [](const std::string& a, const std::string& b)
        {
            return a + ", " + b;

        }, [](const glm::vec3& p)
        {
            return "[" + std::to_string(p.x) + ", " + std::to_string(p.y) + "]";
        }) + "]";
    }

private:
    inline static void apply_lists_of_lists(boost::shared_ptr<std::vector<glm::vec3>>& self, const bp::list& list)
    {
        for(uint32_t i = 0; i < bp::len(list); ++i)
        {
            const bp::list& l = bp::extract<bp::list>(list[i]);
            apply_list(self, l);
        }
    }

    inline static void apply_list(boost::shared_ptr<std::vector<glm::vec3>>& self, const bp::list& list)
    {
        float x = bp::extract<float>(list[0]);
        float y = bp::extract<float>(list[1]);

        self->emplace_back(x, y, 1);
    }
};

/**
 * @class VectorExposureString
 * @brief Special wrapper class for VectorExposure handling vectors of std::string
 */
class VectorExposureString
{
public:

    /**
     * @brief exposed constructor to create a vector of std::string based on a python list
     *
     * @param[in] list a list containing strings.
     *
     * @return a reference to a new std::vector<std::string> exposed to python containing the values of list
     */
    static boost::shared_ptr<std::vector<std::string>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<std::string>>();
        self->reserve(bp::len(list));

        for(uint32_t i = 0; i < bp::len(list); ++i)
        {
            const std::string& s = bp::extract<std::string>(list[i]);
            self->emplace_back(s);
        }

        return self;
    }

    /**
     * @brief adds a new std::string to the given vector
     *
     * @param[in, out] self a vector of strings to receive a new string
     * @param[in] s a std::string to be added to self
     */
    inline static void add(std::vector<std::string>& self, const std::string& s)
    {
        self.push_back(s);
    }

    /**
    * @brief sets the value at the given index of self to the given value
    *
    * @param[in, out] self the vector which value at given index is to be changed
    * @param[in] index the index of the value in self to be changed
    * @param[in] s the std::string to be applied at the given index
    */
    inline static void set(std::vector<std::string>& self, uint32_t index, const std::string& s)
    {
        if(index < 0)
            index += self.size();

        if(index >= 0 && index < self.size())
            self[index] = s;
    }

    /**
     * @brief returns the vectors representation as a string (__repr__ in python)
     *
     * @param[in] self the vector which data is to be presented in a readable way
     *
     * @return the std::string containing the vector's representation
     */
    static const std::string repr(std::vector<std::string>& self)
    {
        return std::reduce(std::execution::par, self.begin(), self.end(),std::string("["), [&](const std::string& a, const std::string& b)
        {
            return a + ", " + b;
        }) + "]";
    }
};

/**
 * @class VectorExposureStringVector
 * @brief Special wrapper class for VectorExposureStringVector handling vectors of vectors of std::string
 */
class VectorExposureStringVector
{
public:
    /**
     * @brief exposed constructor to create a vector of vectors of std::string based on a python list
     *
     * @param[in] list a list of lists containing strings.
     *
     * @return a reference to a new std::vector<std::vector<std::string>> exposed to python containing the values of list
     */
    static boost::shared_ptr<std::vector<std::vector<std::string>>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<std::vector<std::string>>>();
        self->reserve(bp::len(list));

        for(uint32_t i = 0; i < bp::len(list); ++i)
        {
            const std::vector<std::string>& s = bp::extract<std::vector<std::string>>(list[i]);
            self->emplace_back(s);
        }

        return self;
    }

    /**
     * @brief adds a new std::vector<std::string> to the given vector
     *
     * @param[in, out] self a vector of vectors of strings to receive a new vector of strings
     * @param[in] s a std::vector<std::string> to be added to self
     */
    inline static void add(std::vector<std::vector<std::string>>& self, const std::vector<std::string>& s)
    {
        self.push_back(s);
    }

    /**
    * @brief sets the value at the given index of self to the given value
    *
    * @param[in, out] self the vector which value at given index is to be changed
    * @param[in] index the index of the value in self to be changed
    * @param[in] s the std::vector<std::string> to be applied at the given index
    */
    inline static void set(std::vector<std::vector<std::string>>& self, uint32_t index, const std::vector<std::string>& s)
    {
        if(index < 0)
            index += self.size();

        if(index >= 0 && index < self.size())
            self[index] = s;
    }

    /**
     * @brief returns the vectors representation as a string (__repr__ in python)
     *
     * @param[in] self the vector which data is to be presented in a readable way
     *
     * @return the std::string containing the vector's representation
     */
    static const std::string repr(std::vector<std::vector<std::string>>& self)
    {
        std::string ret = "[";

        for (int i = 0; i < self.size(); ++i)
        {
            ret += "[";

            for(int k = 0; k < self[i].size(); ++k)
                ret += self[i][k] + ", ";

            ret += "], ";
        }

        ret += "]";

        return ret;
    }
};

/**
 * @class VectorExposureLinkRelation
 * @brief Special wrapper class for VectorExposure handling vectors of LinkRelation
 */
class VectorExposureLinkRelation
{
public:

    /**
     * @brief exposed constructor to create a vector of LinkRelation based on a python list
     *
     * @param[in] list a list containing instances of LinkRelation or one LinkRelation in form of four strings.
     *
     * @return a reference to a new std::vector<LinkRelation>> exposed to python containing the values of list
     *
     * @see LinkRelation
     */
    static boost::shared_ptr<std::vector<LinkCandidate>> init(const bp::list& list=bp::list())
    {
        auto self = boost::make_shared<std::vector<LinkCandidate>>();
        self->reserve(bp::len(list));

        if(bp::len(list) > 0)
        {
            if(bp::extract<bp::list>(list[0]).check())
                apply_lists_of_lists(self, list);
            else
                apply_list(self, list);
        }

        return self;
    }

    /**
     * @brief adds a new LinkRelation to the given vector
     *
     * @param[in, out] self a vector of LinkRelation to receive a new LinkRelation
     * @param[in] list a python list containing LinkRelation to be added to self
     */
    inline static void add(std::vector<LinkCandidate>& self, const bp::list& list)
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

    /**
    * @brief sets the value at the given index of self to the given value
    *
    * @param[in, out] self the vector which value at given index is to be changed
    * @param[in] index the index of the value to be changed
    * @param[in] list the list containing LinkRelation to be applied at the given index
    */
    inline static void set(std::vector<LinkCandidate>& self, uint32_t index, const bp::list& list)
    {
        if(index < 0)
            index += self.size();

        if(index >= 0 && index < self.size())
        {
            if(bp::len(list) > 0)
            {
                const char* sender = bp::extract<char*>(list[0]);
                const char* sender_attrib = bp::extract<char*>(list[1]);
                const char* recv = bp::extract<char*>(list[2]);
                const char* recv_attrib = bp::extract<char*>(list[3]);

                self[index] = LinkCandidate(sender, sender_attrib, recv, recv_attrib);
            }
        }
    }

    /**
     * @brief returns the vectors representation as a string (__repr__ in python)
     *
     * @param[in] self the vector which data is to be presented in a readable way
     *
     * @return the std::string containing the vector's representation
     */
    static const std::string repr(std::vector<LinkCandidate>& self)
    {
        return std::transform_reduce(std::execution::par, self.begin(), self.end(), std::string("["), [&](const std::string& a, const std::string& b)
        {
           return a + ", " + b;
        }, [&](const LinkCandidate& lr)
        {
            return "[" + lr.sender + ", " + lr.sender_attrib + ", " + lr.recv + ", " + lr.recv_attrib + "]";
        }) + "]";
    }

private:
    inline static void apply_lists_of_lists(boost::shared_ptr<std::vector<LinkCandidate>>& self, const bp::list& list)
    {
        for(uint32_t i = 0; i < bp::len(list); ++i)
        {
            const bp::list& l = bp::extract<bp::list>(list[i]);
            apply_list(self, l);
        }
    }

    inline static void apply_list(boost::shared_ptr<std::vector<LinkCandidate>>& self, const bp::list& list)
    {
        const char* sender = bp::extract<char*>(list[0]);
        const char* sender_attrib = bp::extract<char*>(list[1]);
        const char* recv = bp::extract<char*>(list[2]);
        const char* recv_attrib = bp::extract<char*>(list[3]);

        self->emplace_back(sender, sender_attrib, recv, recv_attrib);
    }
};

/**
 * \brief creates a python binding to the STL vector of type VectorType
 *
 * @tparam VectorExposureType the type of the helper class for construction
 * @tparam VectorType the vector type to be exposed
 * @param name the name with which the vector can be referenced in python
 *
 * @return the datastructure handling the python binding of the vector
 */
template <typename VectorExposureType, typename VectorType, typename PickleSuite>
bp::class_<VectorType> create_vector(const char* name)
{
    return bp::class_<VectorType>(name, bp::no_init)
            .def_pickle(PickleSuite())

            .def("__init__", bp::make_constructor(&VectorExposureType::init, bp::default_call_policies(), (bp::arg("list")=bp::list())))
            .def("__len__", &VectorType::size)
            .def("clear", &VectorType::clear)
            .def("append", &VectorExposure<VectorType>::add)
            .def("append", &VectorExposureType::add)
            .def("__getitem__", &VectorExposure<VectorType>::get, bp::return_value_policy<bp::reference_existing_object>())
            .def("__getitem__", &VectorExposure<VectorType>::get_slice)
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
