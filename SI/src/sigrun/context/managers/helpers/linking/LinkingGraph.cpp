

#include "LinkingGraph.hpp"
#include <sigrun/util/UUID.hpp>
#include <execution>

LinkingGraph::LinkingGraph()
{

}

LinkingGraph::~LinkingGraph()
{

}

void LinkingGraph::add_link(const std::shared_ptr<Region> &a, const std::string &attr_a, const std::shared_ptr<Region> &b,
                            const std::string &attr_b, const ILink::LINK_TYPE &link_type)
{
    if(link_type == ILink::LINK_TYPE::UD)
    {
        std::shared_ptr<ILink> pudl = std::make_shared<UnidirectionalLink>(a, b, attr_a, attr_b);

        d_links.push_back(pudl);
    }
    else if(link_type == ILink::LINK_TYPE::BD)
    {
        std::shared_ptr<ILink> pbdl =  std::make_shared<BidirectionalLink>(a, b, attr_a, attr_b);

        d_links.push_back(pbdl);
    }
}

void LinkingGraph::add_link_to_external_object(const UnidirectionalLink &udl)
{
//    d_links.push_back(std::make_shared<UnidirectionalLink>(udl));
}

void LinkingGraph::remove_link(int index)
{

}

void LinkingGraph::remove_link(const std::shared_ptr<Region> &a, const std::string &attr_a, const std::shared_ptr<Region> &b,
                               const std::string &attr_b, const ILink::LINK_TYPE &link_type)
{


    if(link_type == ILink::LINK_TYPE::UD)
    {
        UnidirectionalLink udl(a, b, attr_a, attr_b);

        auto it = std::find_if(std::execution::par_unseq, d_links.begin(), d_links.end(),[&](auto& link)
        {
            return (link->attribute_a() == udl.attribute_a() &&
                    link->attribute_b() == udl.attribute_b() &&
                    link->sender_a()->uuid() == udl.sender_a()->uuid() &&
                    link->receiver_b()->uuid() == udl.receiver_b()->uuid() &&
                    link->sender_b()->uuid() == udl.sender_b()->uuid() &&
                    link->receiver_a()->uuid() == udl.receiver_a()->uuid());
        });

        d_links.erase(it);
    }
    else if(link_type == ILink::LINK_TYPE::BD)
    {
        BidirectionalLink l(a, b, attr_a, attr_b);

        auto it = std::find_if(std::execution::par_unseq, d_links.begin(), d_links.end(),[&](auto& link)
        {
            return ((link->attribute_a() == l.attribute_a() &&
                    link->attribute_b() == l.attribute_b() &&
                    link->sender_a()->uuid() == l.sender_a()->uuid() &&
                    link->receiver_b()->uuid() == l.receiver_b()->uuid() &&
                    link->sender_b()->uuid() == l.sender_b()->uuid() &&
                    link->receiver_a()->uuid() == l.receiver_a()->uuid()) ||
                    (link->attribute_a() == l.attribute_b() &&
                    link->attribute_b() == l.attribute_a() &&
                    link->sender_a()->uuid() == l.sender_b()->uuid() &&
                    link->receiver_b()->uuid() == l.receiver_a()->uuid() &&
                    link->sender_b()->uuid() == l.sender_a()->uuid() &&
                    link->receiver_a()->uuid() == l.receiver_b()->uuid()));
        });

        d_links.erase(it);
    }
}

void LinkingGraph::emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a)
{
    const std::string uuid = _UUID_;

    const bp::tuple &args = bp::extract<bp::tuple>(a->effect().attr_link_emit()[attr_a]());

    a->register_link_event({uuid, attr_a});

    Q_EMIT a->LINK_SIGNAL(uuid, attr_a, args);
}

bool LinkingGraph::is_linked(const std::shared_ptr<Region> &a, const std::string &attr_a, const std::shared_ptr<Region> &b,
                             const std::string &attr_b, const ILink::LINK_TYPE &link_type) const
{
    for(auto& link: d_links)
    {
        if(link_type == ILink::LINK_TYPE::UD)
        {
            std::shared_ptr<ILink> l = std::make_shared<UnidirectionalLink>(a, b, attr_a, attr_b);

            if (link->attribute_a() == l->attribute_a() &&
                link->attribute_b() == l->attribute_b() &&
                link->sender_a()->uuid() == l->sender_a()->uuid() &&
                link->receiver_b()->uuid() == l->receiver_b()->uuid() &&
                link->sender_b()->uuid() == l->sender_b()->uuid() &&
                link->receiver_a()->uuid() == l->receiver_a()->uuid())
            {
                return true;
            }
        }
        else if(link_type == ILink::LINK_TYPE::BD)
        {
            std::shared_ptr<ILink> l = std::make_shared<BidirectionalLink>(a, b, attr_a, attr_b);

            if ((link->attribute_a() == l->attribute_a() &&
                 link->attribute_b() == l->attribute_b() &&
                 link->sender_a()->uuid() == l->sender_a()->uuid() &&
                 link->receiver_b()->uuid() == l->receiver_b()->uuid() &&
                 link->sender_b()->uuid() == l->sender_b()->uuid() &&
                 link->receiver_a()->uuid() == l->receiver_a()->uuid()) ||
                (link->attribute_a() == l->attribute_b() &&
                 link->attribute_b() == l->attribute_a() &&
                 link->sender_a()->uuid() == l->sender_b()->uuid() &&
                 link->receiver_b()->uuid() == l->receiver_a()->uuid() &&
                 link->sender_b()->uuid() == l->sender_a()->uuid() &&
                 link->receiver_a()->uuid() == l->receiver_b()->uuid()))
            {
                return true;
            }
        }
    }

    return false;
}

const std::vector<std::shared_ptr<ILink>> &LinkingGraph::links() const
{
    return d_links;
}