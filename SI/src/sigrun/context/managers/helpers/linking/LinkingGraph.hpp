

#ifndef SITEST_LINKINGGRAPH_HPP
#define SITEST_LINKINGGRAPH_HPP

#include <memory>
#include <vector>
#include <string>
#include <sigrun/region/Region.hpp>
#include "Link.hpp"

class LinkingGraph
{
public:
    LinkingGraph();
    ~LinkingGraph();

    void add_link(const std::shared_ptr<Region>& a, const std::string& attr_a, const std::shared_ptr<Region>& b, const std::string& attr_b, const ILink::LINK_TYPE& link_type);
    void add_link_to_external_object(const UnidirectionalLink& udl);
    void remove_link(const std::shared_ptr<Region>& a, const std::string& attr_a, const std::shared_ptr<Region>& b, const std::string& attr_b, const ILink::LINK_TYPE& link_type);
    void remove_link(int i);
    void emit_link_event(std::shared_ptr<Region>& a, const std::string& attr_a);
    [[nodiscard]] bool is_linked(const std::shared_ptr<Region>& a, const std::string& attr_a, const std::shared_ptr<Region>& b, const std::string& attr_b, const ILink::LINK_TYPE& link_type) const;

    [[nodiscard]] const std::vector<std::shared_ptr<ILink>>& links() const;

private:
    std::vector<std::shared_ptr<ILink>> d_links;
};


#endif //SITEST_LINKINGGRAPH_HPP
