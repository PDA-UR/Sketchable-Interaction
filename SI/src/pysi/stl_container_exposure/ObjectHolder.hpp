

#ifndef SITEST_OBJECTHOLDER_HPP
#define SITEST_OBJECTHOLDER_HPP

#include <boost/python.hpp>

namespace bp = boost::python;

template <typename T>
class ObjectHolder
{
public:

    typedef T element_type;

    ObjectHolder(element_type* ptr)
            : ptr_(ptr),
            object_()
    {}

    element_type* get() const
    {
        if (!object_.is_none())
        {
            return bp::extract<element_type*>(object_)();
        }
        return ptr_ ? ptr_.get() : nullptr;
    }

    void reset(bp::object object)
    {
        // Verify the object holds the expected element.
        boost::python::extract<element_type*> extractor(object_);
        if (!extractor.check()) return;

        object_ = object;
        ptr_.reset();
    }

private:
    std::shared_ptr<element_type> ptr_;
    bp::object object_;
};

/// @brief Helper function used to extract the pointed to object from
///        an object_holder.  Boost.Python will use this through ADL.
template <typename T>
T* get_pointer(const ObjectHolder<T>& holder)
{
    return holder.get();
}


#endif //SITEST_OBJECTHOLDER_HPP
