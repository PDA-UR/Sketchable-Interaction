

#ifndef SITEST_EXTERNALOBJECT_HPP
#define SITEST_EXTERNALOBJECT_HPP

#include <string>
#include <vector>
#include <QObject>
#include <sigrun/SIObject.hpp>
#include <boost/python.hpp>

namespace bp = boost::python;

class ExternalObject: public QObject, public SIObject
{Q_OBJECT SIGRUN
public:
    enum ExternalObjectType
    {
        MOUSE = 0,
        FINGER = 1,
        TANGIBLE = 2,
        APPLICATION = 3
    };

    union
    {
        int mouse;
        QWidget* external_application;
    } EmbeddedObject;

    explicit ExternalObject(const ExternalObjectType& type);
    ~ExternalObject();

    const ExternalObjectType& type() const;

    Q_SIGNAL void LINK_SIGNAL(const std::string& uuid, const std::string& source_cap, const bp::tuple& args);
    Q_SLOT void LINK_SLOT(const std::string& uuid, const std::string& source_cap, const bp::tuple& args);

private:
    ExternalObjectType d_type;

    union embedded_object
    {
        int mouse;
        QWidget* external_application;
    };
};


#endif //SITEST_EXTERNALOBJECT_HPP
