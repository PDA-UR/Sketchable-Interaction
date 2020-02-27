

#ifndef SITEST_LINKINGMANAGER_HPP
#define SITEST_LINKINGMANAGER_HPP

#include <sigrun/SIObject.hpp>
#include <QObject>
#include <sigrun/region/Region.hpp>
#include <QMetaMethod>
#include <sigrun/util/UUID.hpp>
#include <boost/python.hpp>
#include "sigrun/context/managers/helpers/linking/Link.hpp"
#include "sigrun/context/managers/helpers/linking/LinkingGraph.hpp"

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
    void emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a);

    void remove_links_by_indices(const std::vector<int>& indices);

    void add_link_to_object(std::shared_ptr<Region>& a, const ExternalObject::ExternalObjectType& type);

    const std::vector<std::shared_ptr<ILink>>& links() const;

    const std::unique_ptr<LinkingGraph>& linking_graph() const;
    const int num_links() const;

private:
    std::unordered_map<std::shared_ptr<ILink>, bool> d_links;
    std::unique_ptr<LinkingGraph> d_linking_graph;
};


#endif //SITEST_LINKINGMANAGER_HPP
