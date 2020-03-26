

#include "ExternalObject.hpp"

ExternalObject::ExternalObject(ExternalObjectType type):
    d_type(type)
{

}

ExternalObject::~ExternalObject()
{

}

const ExternalObject::ExternalObjectType &ExternalObject::type() const
{
    return d_type;
}
