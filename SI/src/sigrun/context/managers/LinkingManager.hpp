

#ifndef SITEST_LINKINGMANAGER_HPP
#define SITEST_LINKINGMANAGER_HPP

#include <sigrun/SIObject.hpp>
#include <QObject>
#include <sigrun/region/Region.hpp>
#include "../Link.hpp"
#include <QMetaMethod>
#include <sigrun/util/UUID.hpp>
#include <boost/python.hpp>

Q_DECLARE_METATYPE (std::string)
//Q_DECLARE_METATYPE (boost::python::object)

class LinkingManager: public QObject, public SIObject
{ Q_OBJECT
public:
    LinkingManager();
    ~LinkingManager();

    bool add_link(const std::shared_ptr<Region>& ra,  const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    void remove_link(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    bool is_linked(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);

    void emit_link_event(const std::shared_ptr<Region>& sender, const std::string& sender_attribute, const std::string& receiver_attribute, const std::string& uuid);

    const std::shared_ptr<ILink> link(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    std::unordered_map<std::shared_ptr<ILink>, bool> links() const;

private:
    bool is_linked(const ILink* link);
    std::unordered_map<std::shared_ptr<ILink>, bool> d_links;

//    QMetaMethod fetchIndexOfMethod(QObject* obj, const char* name)
//    {
//        const QMetaObject* meta_object = obj->metaObject();
//        QByteArray normalized_name = QMetaObject::normalizedSignature(name);
//        int index = meta_object->indexOfMethod(normalized_name.constData());
//        return meta_object->method(index);
//    }
//
//// A QObject::connect wrapper
//    QMetaObject::Connection dynamicConnection(QObject* source,
//                                              const char* signal_name,
//                                              QObject* dest,
//                                              const char* slot_name)
//    {
//        return QObject::connect(source, fetchIndexOfMethod(source, signal_name),
//                                dest, fetchIndexOfMethod(dest, slot_name));
//    }
//
//    bool dynamicDisconnection(QObject* source,
//                              const char* signal_name,
//                              QObject* dest,
//                              const char* slot_name)
//    {
//        return QObject::disconnect(source, fetchIndexOfMethod(source, signal_name),
//                                   dest, fetchIndexOfMethod(dest, slot_name));
//    }

};


#endif //SITEST_LINKINGMANAGER_HPP
