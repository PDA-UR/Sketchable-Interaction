

#ifndef SITEST_LINKINGMANAGER_HPP
#define SITEST_LINKINGMANAGER_HPP

#include <sigrun/SIObject.hpp>
#include <QObject>
#include <sigrun/region/Region.hpp>
#include "../Link.hpp"
#include <QMetaMethod>
#include <sigrun/util/UUID.hpp>

Q_DECLARE_METATYPE (std::string);

class LinkingManager: public QObject, public SIObject
{ Q_OBJECT
public:
    LinkingManager();
    ~LinkingManager();

    void add_link(const std::shared_ptr<Region>& ra, const std::shared_ptr<Region>& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type);
    void remove_link(const std::shared_ptr<Region>& ra, const std::shared_ptr<Region>& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE type);

    bool is_linked(const std::string& ra, const std::string& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type);

    const std::shared_ptr<ILink> link(const std::string& ra, const std::string& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type);
    std::unordered_map<std::shared_ptr<ILink>, bool> links() const;

    void add_capability_functions(const std::string& name, const std::string& signal, const std::string& slot);

    bool is_capability_supported(const std::string& capability);

    const std::string& capability_signal_function(const std::string& capability);
    const std::string& capability_slot_function(const std::string& capability);

    void emit_link_event(QObject* source, const std::string& signal, const std::string& event_uuid, QGenericArgument arg1, QGenericArgument arg2);

private:
    bool is_linked(const ILink* udl);

    QMetaMethod fetchIndexOfMethod(QObject* obj, const char* name)
    {
        const QMetaObject* meta_object = obj->metaObject();
        QByteArray normalized_name = QMetaObject::normalizedSignature(name);
        int index = meta_object->indexOfMethod(normalized_name.constData());
        return meta_object->method(index);
    }

// A QObject::connect wrapper
    QMetaObject::Connection dynamicConnection(QObject* source,
                                              const char* signal_name,
                                              QObject* dest,
                                              const char* slot_name)
    {
        return QObject::connect(source, fetchIndexOfMethod(source, signal_name),
                                dest, fetchIndexOfMethod(dest, slot_name));
    }

    bool dynamicDisconnection(QObject* source,
                              const char* signal_name,
                              QObject* dest,
                              const char* slot_name)
    {
        return QObject::disconnect(source, fetchIndexOfMethod(source, signal_name),
                                   dest, fetchIndexOfMethod(dest, slot_name));
    }

    std::unordered_map<std::shared_ptr<ILink>, bool> d_links;
    std::unordered_map<std::string, std::vector<std::string>> d_capability_functions;
};


#endif //SITEST_LINKINGMANAGER_HPP
