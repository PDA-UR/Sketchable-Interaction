

#include "ExternalObject.hpp"
#include <sigrun/log/Log.hpp>
#include <QVariant>
#include <QEvent>
#include <sigrun/util/UUID.hpp>

ExternalObject::ExternalObject(const ExternalObjectType& type):
    d_type(type),
    d_uuid(_UUID_)
{

}

ExternalObject::~ExternalObject()
{

}

const ExternalObject::ExternalObjectType &ExternalObject::type() const
{
    return d_type;
}

const std::string &ExternalObject::uuid() const
{
    return d_uuid;
}
