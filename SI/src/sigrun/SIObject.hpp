

#ifndef SITEST_SIOBJECT_HPP
#define SITEST_SIOBJECT_HPP

#include <string>
#include <debug/Print.hpp>

/**
\brief macro for extracting the name of the class to be registered as an SIObject
\details
    The macro computes the name of a class which is to be registered as an SIObject.
    After retrieving the name of the class, the macro makes transforms the result string to uppercase.
\return a std::string containing the uppercase name of the class to be registered as an SIObject
*/
#define __CLASS_NAME__ (\
{\
    std::string s = std::string(__PRETTY_FUNCTION__).substr(0, strchr(__PRETTY_FUNCTION__, ':') - __PRETTY_FUNCTION__);\
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);\
    s;\
})


/**
\brief macro for registering another class as SIObject
\details
    The macro is a shortcut for registering other classes which are derived from SIObject as such a SIObject.
    Syntax:
        class A: public SIObject
        {SIOBJECT
            ...
        };
*/

#define SIOBJECT(origin) (\
{\
    d_meta_type = __CLASS_NAME__;\
    d_origin = origin;\
});

/**
\class SIObject
\brief A meta class from which other classes are derived from to register them as SIObject meta types
\details
    This class enables registering other classes as SIObject meta types.
    This is currently achieved by storing std::strings containing the classes individual names.
    Currently, this meta typing is only used for Logging.

\see Log::Log
\see d_meta_type
*/
class SIObject
{
public:
    /**
    \brief default constructor
    */
    SIObject() = default;

    /**
    \brief default destructor
    */
    ~SIObject() = default;

    /**
    \brief function for retrieving meta type name
    \details
        The function for retrieving meta type name in a constant manner.
        Therefore, the instance calling this function will not mutate.
    \return d_meta_type a const std::string reference of the type name of the clas
    */
    const std::string& meta_type() const
    {
        return d_meta_type;
    }

    const std::string& origin() const
    {
        return d_origin;
    }

protected:
    /**
    \brief a std::string containing the name of the class to be registered as SIObject meta type
    */
    std::string d_meta_type;
    std::string d_origin;
};

#endif //SITEST_SIOBJECT_HPP
