

#include "ExternalObject.hpp"
#include <sigrun/log/Log.hpp>
#include <QWidget>

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

void ExternalObject::LINK_SLOT(const std::string& uuid, const std::string& source_cap, const bp::tuple& args)
{
    switch(d_type)
    {
        case APPLICATION:
        {
            if(source_cap == "__position__")
            {
                const uint32_t&  x = (uint32_t) bp::extract<float>(args[0]);
                const uint32_t&  y = (uint32_t) bp::extract<float>(args[1]);

                if(embedded_object.external_application.window)
                    embedded_object.external_application.window->move(20 + x,  20 + y);
                else
                    ERROR("NO EMBEDDED OBJECT");
            }
            else if(source_cap == "__scale__")
            {
                INFO("SCALE NOT IMPLEMENTED");
            }
            else
            {
                ERROR("THIS CAPABILITY IS NOT SUPPORTED BY SIGRUN");
            }
        }
        break;

        default:
            WARN("UNSUPPORTED Type");
    }
}
