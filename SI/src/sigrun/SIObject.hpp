

#ifndef SITEST_SIOBJECT_HPP
#define SITEST_SIOBJECT_HPP

#include <string>

/**
\brief macro for registering another class as SIObject
\details
    The macro is a shortcut for registering other classes which are derived from SIObject as such a SIObject.
    Syntax:
        class A: public SIObject
        {SIOBJECT("A")
            ...
        };
\param type a std::string containing the type name of a class to be registered as SIObject.
*/
#define SIOBJECT(type) (d_meta_type = type);

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

protected:
    /**
    \brief a std::string containing the name of the class to be registered as SIObject meta type
    */
    std::string d_meta_type;
};

#endif //SITEST_SIOBJECT_HPP
