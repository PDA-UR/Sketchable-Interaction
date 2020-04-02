

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
        struct
        {

        } mouse;

        struct
        {
            QWidget* window;
            uint64_t pid;
            char* file_uuid;
        } external_application;
    } embedded_object;

    explicit ExternalObject(const ExternalObjectType& type);
    ~ExternalObject();

    const ExternalObjectType& type() const;

    Q_SIGNAL void LINK_SIGNAL(const std::string& uuid_event, const std::string& uuid_sender, const std::string& source_cap, const bp::tuple& args);
    Q_SLOT void LINK_SLOT(const std::string& uuid, const std::string& uuid_sender, const std::string& source_cap, const bp::tuple& args);

private:
    ExternalObjectType d_type;

    union embedded_object
    {
        int mouse;
        QWidget* external_application;
    };
};


#endif //SITEST_EXTERNALOBJECT_HPP
