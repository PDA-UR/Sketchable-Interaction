

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>


LinkingGraph::LinkingGraph()
{

}

LinkingGraph::~LinkingGraph()
{

}

void LinkingGraph::add_link(std::shared_ptr<Region> &a, const std::string &attr_a, std::shared_ptr<Region> &b,
                            const std::string &attr_b, const ILink::LINK_TYPE &link_type)
{
    std::shared_ptr<ILink> pudl;

    if(link_type == ILink::LINK_TYPE::UD)
    {
        pudl =  std::make_shared<UnidirectionalLink>(a, b, attr_a, attr_b);

        for (std::shared_ptr<ILink>& link: d_links)
        {
            if((link->receiver_b()->uuid() == pudl->sender_a()->uuid()) && (link->attribute_b() == pudl->attribute_a()))
                link->add_child(pudl);

            if((pudl->receiver_b()->uuid() == link->sender_a()->uuid()) && (pudl->attribute_b() == link->attribute_a()))
                pudl->add_child(link);
        }

        d_links.push_back(pudl);
    }
}

void LinkingGraph::remove_link(std::shared_ptr<Region> &a, const std::string &attr_a, std::shared_ptr<Region> &b,
                               const std::string &attr_b, const ILink::LINK_TYPE &link_type)
{
    if(link_type == ILink::LINK_TYPE::UD)
    {
        UnidirectionalLink udl(a, b, attr_a, attr_b);

        for (auto it = d_links.begin(); it != d_links.end();)
        {
            if (it->get()->attribute_a() == udl.attribute_a() &&
                it->get()->attribute_b() == udl.attribute_b() &&
                it->get()->sender_a()->uuid() == udl.sender_a()->uuid() &&
                it->get()->receiver_b()->uuid() == udl.receiver_b()->uuid() &&
                it->get()->sender_b()->uuid() == udl.sender_b()->uuid() &&
                it->get()->receiver_a()->uuid() == udl.receiver_a()->uuid())
            {
                QObject::disconnect(a.get(), &Region::LINK_SIGNAL, b.get(), &Region::LINK_SLOT);

                for(auto it2 = d_links.begin(); it2 != d_links.end(); ++it2)
                {
                    if(it == it2)
                        continue;

                    cascade_remove_link(it->get(), it2->get()->children());
                }

                it = d_links.erase(it);
                break;
            }
            else
                ++it;
        }
    }
}

void LinkingGraph::cascade_remove_link(ILink* link, std::vector<std::shared_ptr<ILink>>& children)
{
    // just for UD case
    for(auto it = children.begin(); it != children.end();)
    {
        if (link->attribute_a() == it->get()->attribute_a() &&
            link->attribute_b() == it->get()->attribute_b() &&
            link->sender_a()->uuid() == it->get()->sender_a()->uuid() &&
            link->receiver_b()->uuid() == it->get()->receiver_b()->uuid() &&
            link->sender_b()->uuid() == it->get()->sender_b()->uuid() &&
            link->receiver_a()->uuid() == it->get()->receiver_a()->uuid())
        {
            it = children.erase(it);
            cascade_remove_link(link, it->get()->children());
        }
        else
        {
            ++it;
        }
    }
}


void LinkingGraph::emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a)
{
    for (std::shared_ptr<ILink>& link: d_links)
    {
        if(link->sender_a()->uuid() == a->uuid() && link->attribute_a() == attr_a)
        {
            const bp::list& py_list = bp::extract<bp::list>(a->effect().attr("cap_link_emit")[attr_a]());
            const std::string uuid = UUID::uuid();

            a->register_link_event({uuid, link->attribute_a()});

            Q_EMIT a->LINK_SIGNAL(uuid, attr_a, link->attribute_b(), py_list);

            cascade_link_events(link->children(), uuid);
        }
    }
}

void LinkingGraph::cascade_link_events(const std::vector<std::shared_ptr<ILink>> &children, const std::string& uuid)
{
    for(auto& child: children)
    {
        std::tuple<std::string, std::string> event = {uuid, child->attribute_a()};
        const std::vector<std::tuple<std::string, std::string>>& link_events = child->sender_a()->link_events();

        if(std::find(link_events.begin(), link_events.end(), event) == link_events.end())
        {
            child->sender_a()->register_link_event(event);

            const bp::list& py_list = bp::extract<bp::list>(child->sender_a()->effect().attr("cap_link_emit")[child->attribute_a()]());

            Q_EMIT child->sender_a()->LINK_SIGNAL(uuid, child->attribute_a(), child->attribute_b(), py_list);

            cascade_link_events(child->children(), uuid);
        }
    }
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


///////////////////////////////////
///////////////////////////////////
///////////////////////////////////


LinkingManager::LinkingManager():
        d_linking_graph(std::make_unique<LinkingGraph>())
{SIGRUN
    qRegisterMetaType<std::string>("std::string");
//    qRegisterMetaType<boost::python::object>("boost::python::object");

}

LinkingManager::~LinkingManager()
{
    INFO("Destroying LinkingManager...");

    INFO("Destroying LinkingManager...");
}

bool LinkingManager::add_link(std::shared_ptr<Region> &ra, const std::string& aa, std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE &type)
{
    if(type == ILink::LINK_TYPE::UD)
    {
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is already present...");

        if(is_linked(ra, aa, rb, ab, type))
        {
            INFO("Requested linking relationship is already present!");

            return true;
        }

        INFO("Requested linking relationship is not already present!");
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is defined...");
        const bp::dict& dra = bp::extract<bp::dict>(ra->effect().attr("cap_link_emit"));
        const bp::dict& drb = bp::extract<bp::dict>(rb->effect().attr("cap_link_recv"));

        if(dra.has_key(aa) && drb.has_key(aa))
        {
            const bp::dict& inner_drb = bp::extract<bp::dict>(drb[aa]);

            if(inner_drb.has_key(ab))
            {
                INFO("Requested linking relationship is defined!");
                INFO("Establishing Unidirectional Link...");

                d_linking_graph->add_link(ra, aa, rb, ab, type);

                // hack: preemptive disconnect to quench multiple connects per Region pair
                // multiple connects occur, when a pair of regions is connected via multiple attributs
                // example:
                // A -> B: position, position;
                // A -> B: scale, scale;
                // with current add_link function logic, the connect() function would be called twice
                // this leads to executing the slot function twice for the same attribute pair
                //
                // this note is also present within the unidirectional linking case
                disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
                bool success = connect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

                if(success)
                {
                    INFO("Establishing Unidirectional Link Successfull!");
                }
                else
                {
                    d_linking_graph->remove_link(ra, aa, rb, ab, type);

                    INFO("Establishing Unidirectional Link Failed!");
                }

                return success;
            }
        }

        return false;
    }
    else if(type == ILink::LINK_TYPE::BD)
    {
//        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is already present...");
//
//        BidirectionalLink bdl(ra, rb, aa, ab);
//
//        if(is_linked(&bdl))
//        {
//            INFO("Requested linking relationship is already present!");
//
//            return true;
//        }

        INFO("Requested linking relationship is not already present!");
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "bidirectional link) is defined...");

        const bp::dict& drae = bp::extract<bp::dict>(ra->effect().attr("cap_link_emit"));
        const bp::dict& drbe = bp::extract<bp::dict>(ra->effect().attr("cap_link_emit"));
        const bp::dict& drar = bp::extract<bp::dict>(rb->effect().attr("cap_link_recv"));
        const bp::dict& drbr = bp::extract<bp::dict>(rb->effect().attr("cap_link_recv"));

        if(drae.has_key(aa) && drbr.has_key(aa) && drar.has_key(ab) && drbe.has_key(ab))
        {
            const bp::dict& inner_drbr = bp::extract<bp::dict>(drbr[aa]);
            const bp::dict& inner_drar = bp::extract<bp::dict>(drar[ab]);

            if(inner_drbr.has_key(ab) && inner_drar.has_key(aa))
            {
                INFO("Requested linking relationship is defined!");
                INFO("Establishing Bidirectional Link...");


                std::shared_ptr<BidirectionalLink> l = std::make_shared<BidirectionalLink>(ra, rb, aa, ab);
                d_links.insert({l, true});

                // hack: preemptive disconnect to quench multiple connects per Region pair
                // multiple connects occur, when a pair of regions is connected via multiple attributs
                // example:
                // A -> B: position, position;
                // A -> B: scale, scale;
                // with current add_link function logic, the connect() function would be called twice
                // this leads to executing the slot function twice for the same attribute pair
                //
                // this note is also present within the unidirectional linking case
                disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
                disconnect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);
                bool success = connect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
                success = connect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);

                if(success)
                {
                    INFO("Establishing Unidirectional Link Successfull!");
                }
                else
                {
                    d_links.erase(l);
                    INFO("Establishing Unidirectional Link Failed!");
                }

                return success;
            }

            return false;
        }

        return false;
    }

    return false;
}

void LinkingManager::remove_link(std::shared_ptr<Region> &ra, const std::string& aa, std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE& type)
{
    if(is_linked(ra, aa, rb, ab, type))
    {
        d_linking_graph->remove_link(ra, aa, rb, ab, type);

//        else if(type == ILink::LINK_TYPE::BD)
//        {
//            BidirectionalLink l(ra, rb, aa, ab);
//
//            for (auto it = d_links.begin(); it != d_links.end();)
//            {
//                auto &key = it->first;
//
//                if ((key->attribute_a() == l.attribute_a() &&
//                     key->attribute_b() == l.attribute_b() &&
//                     key->sender_a()->uuid() == l.sender_a()->uuid() &&
//                     key->receiver_b()->uuid() == l.receiver_b()->uuid() &&
//                     key->sender_b()->uuid() == l.sender_b()->uuid() &&
//                     key->receiver_a()->uuid() == l.receiver_a()->uuid()) ||
//                    (key->attribute_a() == l.attribute_b() &&
//                     key->attribute_b() == l.attribute_a() &&
//                     key->sender_a()->uuid() == l.sender_b()->uuid() &&
//                     key->receiver_b()->uuid() == l.receiver_a()->uuid() &&
//                     key->sender_b()->uuid() == l.sender_a()->uuid() &&
//                     key->receiver_a() ->uuid()== l.receiver_b()->uuid()))
//                {
//
//                    // are all connect killed then?
//                    // e.g. (A -> B: pos, pos   and    A -> B: scale, pos)
//                    // does the 2nd one still work if the first one is disconnected?
//                    disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
//                    disconnect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);
//
//                    it = d_links.erase(it);
//
//                    break;
//                }
//                else
//                    ++it;
//            }
    }
}

const std::shared_ptr<ILink> LinkingManager::link(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string &ab, const ILink::LINK_TYPE &type)
{
//    if(type == ILink::LINK_TYPE::UD)
//    {
//        UnidirectionalLink udl(ra, rb, aa, ab);
//
//        if(is_linked(&udl))
//            return std::make_shared<UnidirectionalLink>(udl);
//    }
//    else if(type == ILink::LINK_TYPE::BD)
//    {
//        BidirectionalLink bdl(ra, rb, aa, ab);
//
//        if(is_linked(&bdl))
//            return std::make_shared<BidirectionalLink>(bdl);
//    }

    return nullptr;
}

bool LinkingManager::is_linked(const std::shared_ptr<Region>& ra, const std::string &aa, const std::shared_ptr<Region>& rb, const std::string &ab,
                          const ILink::LINK_TYPE &type)
{
    return d_linking_graph->is_linked(ra, aa, rb, ab, type);
}

const std::vector<std::shared_ptr<ILink>>& LinkingManager::links() const
{
    return d_linking_graph->links();
}

const std::unique_ptr<LinkingGraph> &LinkingManager::linking_graph() const
{
    return d_linking_graph;
}
