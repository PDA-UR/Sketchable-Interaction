

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

class LinkingGraph
{
public:
    LinkingGraph();
    ~LinkingGraph();

    void add_link(std::shared_ptr<Region>& a, const std::string& attr_a, std::shared_ptr<Region>& b, const std::string& attr_b, const ILink::LINK_TYPE& link_type);

    void remove_link(std::shared_ptr<Region>& a, const std::string& attr_a, std::shared_ptr<Region>& b, const std::string& attr_b, const ILink::LINK_TYPE& link_type);
    void cascade_remove_link(ILink* link, std::vector<std::shared_ptr<ILink>>& children);

    void emit_link_event(std::shared_ptr<Region>& a, const std::string& attr_a);
    void cascade_link_events(const std::vector<std::shared_ptr<ILink>>& children, const std::string& uuid);

    bool is_linked(const std::shared_ptr<Region>& a, const std::string& attr_a, const std::shared_ptr<Region>& b, const std::string& attr_b, const ILink::LINK_TYPE& link_type) const;

    const std::vector<std::shared_ptr<ILink>>& links() const;

private:
    std::vector<std::shared_ptr<ILink>> d_links;
};

class LinkingManager: public QObject, public SIObject
{ Q_OBJECT
public:
    LinkingManager();
    ~LinkingManager();

    bool add_link(std::shared_ptr<Region>& ra,  const std::string& aa, std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    void remove_link(std::shared_ptr<Region>& ra, const std::string& aa, std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    bool is_linked(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);


    const std::shared_ptr<ILink> link(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    const std::vector<std::shared_ptr<ILink>>& links() const;

    const std::unique_ptr<LinkingGraph>& linking_graph() const;

private:
    std::unordered_map<std::shared_ptr<ILink>, bool> d_links;
    std::unique_ptr<LinkingGraph> d_linking_graph;

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
