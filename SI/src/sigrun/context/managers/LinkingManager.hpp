

#ifndef SITEST_LINKINGMANAGER_HPP
#define SITEST_LINKINGMANAGER_HPP

#include <sigrun/SIObject.hpp>
#include <QObject>
#include <sigrun/region/Region.hpp>
#include <QMetaMethod>
#include <sigrun/util/UUID.hpp>
#include <boost/python.hpp>
#include "sigrun/context/managers/helpers/linking/Link.hpp"

Q_DECLARE_METATYPE (std::string)
Q_DECLARE_METATYPE (boost::python::object)
Q_DECLARE_METATYPE (bp::tuple)

class LinkingManager: public QObject, public SIObject
{ Q_OBJECT SIGRUN
public:
    LinkingManager();
    ~LinkingManager();

    bool add_link(const std::shared_ptr<Region>& ra,  const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    void add_link(std::shared_ptr<ExternalObject>& eo, std::shared_ptr<Region> &a, const std::string& ea, const std::string& aa);

    void remove_link(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    void remove_link(std::shared_ptr<ExternalObject>& eo, std::shared_ptr<Region> &a, const std::string& ea, const std::string& aa);


    bool is_linked(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string& ab, const ILink::LINK_TYPE& type);
    bool is_linked(const std::string& ra_uuid, const std::string& aa, const std::string& rb_uuid, const std::string& ab, const ILink::LINK_TYPE& type);
    bool is_linked(const std::shared_ptr<ExternalObject>& eo, const std::string& ea, const std::shared_ptr<Region>& ra, const std::string& aa);
    bool is_linked(const std::string& eo_uuid, const std::string& ea, const std::string& rb_uuid, const std::string& ab);

    void emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a);

    void remove_links_by_indices(std::vector<uint32_t>& indices);


    const std::vector<std::shared_ptr<ILink>>& links() const;

    const uint64_t num_links() const;

    void update_linking_candidates(std::vector<LinkCandidate>& relations, const std::string& source);
    void remove_all_partaking_linking_relations(const std::string& source);

private:
    std::vector<std::shared_ptr<ILink>> d_links;
    std::unordered_map<std::string, std::vector<std::shared_ptr<ILink>>> d_links_in_ctx;

    void remove_all_source_linking_relations(const std::string& source);
    void remove_linking_relations(const std::vector<LinkCandidate>& candidates, const std::string& source);
    void create_linking_relations(std::vector<LinkCandidate>& candidates, const std::string& source);
};


#endif //SITEST_LINKINGMANAGER_HPP
