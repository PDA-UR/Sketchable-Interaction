

#include "ExternalObject.hpp"
#include <sigrun/log/Log.hpp>
#include <QVariant>
#include <QEvent>

ExternalObject::ExternalObject(const ExternalObjectType& type):
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
