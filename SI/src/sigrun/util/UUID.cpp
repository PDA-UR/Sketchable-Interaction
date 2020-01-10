

#include "UUID.hpp"
#include <uuid/uuid.h>

std::string UUID::uuid()
{
    uuid_t uuid;
    char uuid_str[37];

    uuid_generate_time_safe(uuid);
    uuid_unparse_lower(uuid, uuid_str);

    return std::string(uuid_str);
}
