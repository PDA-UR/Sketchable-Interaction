

#ifndef SITEST_SIOBJECT_HPP
#define SITEST_SIOBJECT_HPP

#include <string>


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

#define SIOBJECT(o) \
private:\
    const std::string __s__() \
    {\
        std::string s = std::string(__PRETTY_FUNCTION__).substr(0, strchr(__PRETTY_FUNCTION__, ':') - __PRETTY_FUNCTION__);\
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);\
        return s;\
    }\
    std::string d_meta_type = __s__();\
    std::string d_origin = #o;\
public:\
    const std::string& meta_type() const override\
    {\
        return d_meta_type;\
    }\
    const std::string& origin() const override\
    {\
        return d_origin;\
    }\

#define SIGRUN SIOBJECT(SIGRUN);
#define SIREN SIOBJECT(SIREN);
#define PYSI SIOBJECT(PYSI);
#define SITEST SIOBJECT(SITEST);

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
    \brief function for retrieving meta type name
    \details
        The function for retrieving meta type name in a constant manner.
        Therefore, the instance calling this function will not mutate.
    \return d_meta_type a const std::string reference of the type name of the class
    */
    virtual const std::string& meta_type() const = 0;

    /**
    \brief function for retrieving origin
    \details
        The function for retrieving origin in a constant manner.
        Therefore, the instance calling this function will not mutate.
    \return origin a const std::string reference of the origin of the class
    */
    virtual const std::string& origin() const = 0;
};

#endif //SITEST_SIOBJECT_HPP
