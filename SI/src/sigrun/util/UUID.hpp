

#ifndef SITEST_UUID_HPP
#define SITEST_UUID_HPP

#include <string>
#include <uuid/uuid.h>

class UUID
{
public:
    static std::string uuid()
    {
        uuid_t uuid;
        char uuid_str[37];

        uuid_generate_time_safe(uuid);
        uuid_unparse_lower(uuid, uuid_str);

        return std::string(uuid_str);
    }
};


#endif //SITEST_UUID_HPP
