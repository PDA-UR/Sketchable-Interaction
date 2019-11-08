

#ifndef SITEST_METATYPE_HPP
#define SITEST_METATYPE_HPP

#include <string>

#define META_TYPE(type) (d_meta_type = type);

class SIMetaType
{
public:
    SIMetaType() = default;
    ~SIMetaType() = default;

    const std::string& meta_type() const
    {
        return d_meta_type;
    }

protected:
    std::string d_meta_type;
};

#endif //SITEST_METATYPE_HPP
