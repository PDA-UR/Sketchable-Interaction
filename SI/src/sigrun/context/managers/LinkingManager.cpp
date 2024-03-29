

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>
#include <execution>
#include <algorithm>

LinkingManager::LinkingManager()
{
    qRegisterMetaType<std::string>("std::string");
    qRegisterMetaType<bp::object>("bp::object");
    qRegisterMetaType<bp::tuple>("bp::tuple");
}

LinkingManager::~LinkingManager()
{
    INFO("Destroying LinkingManager...");

    INFO("LinkingManager destroyed...");
}

bool LinkingManager::add_link(const std::shared_ptr<Region> &ra, const std::string& aa, const std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE &type)
{
    if(type == ILink::LINK_TYPE::UD)
    {
        INFO("Checking if requested linking relationship (" + ra->name() + ": " + aa + "|" + rb->name() + ": " + ab + "as " + "unidirectional link) is already present...");

        if(is_linked(ra, aa, rb, ab, type))
        {
            INFO("Requested linking relationship is already present!");
            return false;
        }

        INFO("Requested linking relationship is not already present!");
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is defined...");
        auto& dra = ra->effect()->attr_link_emit();
        auto& drb = rb->effect()->attr_link_recv();

        if (!dra.count(aa) || !drb.count(aa) || !drb[aa].count(ab))
            return false;

        INFO("Requested linking relationship is defined!");
        INFO("Establishing Unidirectional Link...");

        // hack: preemptive disconnect to quench multiple connects per Region pair
        // multiple connects occur, when a pair of regions is connected via multiple attributse
        // example:
        // A -> B: position, position;
        // A -> B: scale, scale;
        // with current add_link function logic, the connect() function would be called twice
        // this leads to executing the slot function twice for the same attribute pair
        //
        // this note is also present within the unidirectional linking case
        disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
        bool success_1 = connect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

        if(success_1)
        {
            d_links.push_back(std::make_shared<UnidirectionalLink>(ra, rb, aa, ab));

            INFO("Establishing Unidirectional Link Successfull!");
        }
        else
        {
            disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

            INFO("Establishing Unidirectional Link Failed!");
        }

        return success_1;

    }

    if(type == ILink::LINK_TYPE::BD)
    {
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is already present...");

        if(is_linked(ra, aa, rb, ab, ILink::LINK_TYPE::UD) && is_linked(rb, ab, ra, aa, ILink::LINK_TYPE::UD))
        {
            INFO("Requested linking relationship is already present!");

            d_links.push_back(std::make_shared<BidirectionalLink>(ra, rb, aa, ab));

            return true;
        }

        bool success = add_link(ra, aa, rb, ab, ILink::LINK_TYPE::UD);
        success = add_link(rb, ab, ra, aa, ILink::LINK_TYPE::UD);

        return success;
    }

    return false;
}

void LinkingManager::register_link_event_emission(const std::string& event_uuid, const std::string& sender_uuid, const std::string& sender_attribute, const bp::object& args)
{
    d_link_emission_queue.emplace(event_uuid, sender_uuid, sender_attribute, args);
}

void LinkingManager::perform_link_events()
{
    int32_t link_queue_size = d_link_emission_queue.size();

    for(int32_t i = 0; i < link_queue_size; ++i)
    {
        const auto& link_tuple = d_link_emission_queue.front();
        bool found = false;

        for(auto* region = Context::SIContext()->region_manager()->regions().data(); region < Context::SIContext()->region_manager()->regions().data() + Context::SIContext()->region_manager()->regions().size(); ++region)
        {
            if((*region)->uuid() == std::get<1>(link_tuple))
            {
                (*region)->register_link_event({std::get<0>(link_tuple), std::get<2>(link_tuple)});
                Q_EMIT (*region)->LINK_SIGNAL(std::get<0>(link_tuple), (*region)->uuid(), std::get<2>(link_tuple), std::get<3>(link_tuple));
                break;
            }
        }

        d_link_emission_queue.pop();
    }
}

void LinkingManager::add_link(std::shared_ptr<ExternalObject>& eo, std::shared_ptr<Region> &a, const std::string& ea, const std::string& aa)
{
    if(is_linked(eo, ea, a, aa))
        return;

    disconnect(eo.get(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT);
    if(connect(eo.get(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT))
    {
        d_links.push_back(std::make_shared<UnidirectionalLink>(Context::SIContext()->external_objects()[a->uuid()], a, ea, aa));

        INFO("Establishing unidirectional link to external object successfull!");
    }
    else
    {
        INFO("Establishing unidirectional link to external object failed!");
    }
}

void LinkingManager::remove_link(std::shared_ptr<ExternalObject>& eo, std::shared_ptr<Region> &a, const std::string &ea, const std::string &aa)
{
    if (!is_linked(eo, ea, a, aa))
        return;

    disconnect(eo.get(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT);

    for(int i = d_links.size() - 1; i >= 0; --i)
    {
        auto& link = d_links[i];

        if(!link->is_external())
            continue;

        if(link->attribute_a() == ea &&
           link->attribute_b() == aa &&
           link->external_sender_a()->uuid() == eo->uuid() &&
           link->receiver_b()->uuid() == a->uuid())
        {
            d_links.erase(d_links.begin() + i);
        }
    }
}

void LinkingManager::remove_links_by_indices(std::vector<uint32_t>& indices)
{
    for(int i: indices)
        remove_link(d_links[i]->sender_a(), d_links[i]->attribute_a(), d_links[i]->receiver_b(), d_links[i]->attribute_b(), ILink::LINK_TYPE::UD);
}

void LinkingManager::remove_link(const std::shared_ptr<Region> &ra, const std::string& aa, const std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE& type)
{
    if (!is_linked(ra, aa, rb, ab, type))
        return;

    if(type == ILink::LINK_TYPE ::UD)
    {
        INFO("Requested Deletion of UD Link between " + ra->name() + " and " + rb->name());
        disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

        for(int i = d_links.size() - 1; i >= 0; --i)
        {
            if(!d_links[i]->is_external())
            {
                if(d_links[i]->attribute_a() == aa &&
                   d_links[i]->attribute_b() == ab &&
                   d_links[i]->sender_a()->uuid() == ra->uuid() &&
                   d_links[i]->receiver_b()->uuid() == rb->uuid() &&
                   d_links[i]->sender_b()->uuid() == rb->uuid() &&
                   d_links[i]->receiver_a()->uuid() == ra->uuid())
                {
                    d_links.erase(d_links.begin() + i);
                }
            }
        }

        INFO("Requested Deletion of UD Link successful!");
    }

    if(type == ILink::LINK_TYPE ::BD)
    {
        INFO("Requested Deletion of BD Link between " + ra->name() + " and " + rb->name());

        disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
        disconnect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);

        for(int i = d_links.size() - 1; i >= 0; --i)
        {
            if(!d_links[i]->is_external())
            {
                if((d_links[i]->attribute_a() == aa &&
                        d_links[i]->attribute_b() == ab &&
                        d_links[i]->sender_a()->uuid() == ra->uuid() &&
                        d_links[i]->receiver_b()->uuid() == rb->uuid() &&
                        d_links[i]->sender_b()->uuid() == rb->uuid() &&
                        d_links[i]->receiver_a()->uuid() == ra->uuid()) ||
                    (d_links[i]->attribute_a() == ab &&
                            d_links[i]->attribute_b() == aa &&
                            d_links[i]->sender_a()->uuid() == rb->uuid() &&
                            d_links[i]->receiver_b()->uuid() == ra->uuid() &&
                            d_links[i]->sender_b()->uuid() == ra->uuid() &&
                            d_links[i]->receiver_a()->uuid() == rb->uuid()))
                {
                    d_links.erase(d_links.begin() + i);
                }
            }
        }

        INFO("Requested Deletion of BD Link successful!");
    }
}

bool LinkingManager::is_linked(const std::shared_ptr<Region>& ra, const std::string &aa, const std::shared_ptr<Region>& rb, const std::string &ab, const ILink::LINK_TYPE &type)
{
    return is_linked(ra->uuid(), aa, rb->uuid(), ab, type);
}

bool LinkingManager::is_linked(const std::string& ra_uuid, const std::string& aa, const std::string& rb_uuid, const std::string& ab, const ILink::LINK_TYPE& type)
{
    for(auto& link: d_links)
    {
        if(link->is_external())
            continue;

        if(type == ILink::LINK_TYPE::UD)
        {
            if(link->attribute_a() == aa &&
               link->attribute_b() == ab &&
               link->sender_a()->uuid() == ra_uuid &&
               link->receiver_b()->uuid() == rb_uuid &&
               link->sender_b()->uuid() == rb_uuid &&
               link->receiver_a()->uuid() == ra_uuid)
                return true;
        }
        else
        {
            if((link->attribute_a() == aa &&
                link->attribute_b() == ab &&
                link->sender_a()->uuid() == ra_uuid &&
                link->receiver_b()->uuid() == rb_uuid &&
                link->sender_b()->uuid() == rb_uuid &&
                link->receiver_a()->uuid() == ra_uuid) ||
               (link->attribute_a() == ab &&
                link->attribute_b() == aa &&
                link->sender_a()->uuid() == rb_uuid &&
                link->receiver_b()->uuid() == ra_uuid &&
                link->sender_b()->uuid() == ra_uuid &&
                link->receiver_a()->uuid() == rb_uuid))
                return true;
        }
    }

    return false;
}

bool LinkingManager::is_linked(const std::shared_ptr<ExternalObject> &eo, const std::string &ea, const std::shared_ptr<Region> &ra, const std::string &aa)
{
    return is_linked(eo->uuid(), ea, ra->uuid(), aa);
}

bool LinkingManager::is_linked(const std::string &eo_uuid, const std::string &ea, const std::string &ra_uuid, const std::string &aa)
{
    for(auto* link = d_links.data(); link < d_links.data() + d_links.size(); ++link)
    {
        if (!link->get()->is_external())
            continue;

        if(link->get()->attribute_a() == ea &&
            link->get()->attribute_b() == aa &&
            link->get()->external_sender_a()->uuid() == eo_uuid &&
            link->get()->receiver_b()->uuid() == ra_uuid)
        {
            return true;
        }
    }

    return false;
}

void LinkingManager::emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a)
{
    const std::string uuid_event = _UUID_;

    const bp::tuple &args = bp::extract<bp::tuple>(a->effect()->attr_link_emit()[attr_a]());

    Context::SIContext()->register_link_event_emission(uuid_event, a->uuid(), attr_a, args);
}

const std::vector<std::shared_ptr<ILink>>& LinkingManager::links() const
{
    return d_links;
}

const uint64_t LinkingManager::num_links() const
{
    return d_links.size();
}

void LinkingManager::update_linking_candidates(std::vector<LinkCandidate>& candidates, const std::string& source)
{
    if (candidates.empty())
    {
        remove_all_source_linking_relations(source);
        return;
    }

    remove_linking_relations(candidates, source);
    create_linking_relations(candidates, source);

}

void LinkingManager::remove_all_source_linking_relations(const std::string &source)
{
    if (!d_links_in_ctx.count(source))
        return;

    for(auto& i: d_links_in_ctx[source])
        remove_link(i->sender_a(), i->attribute_a(), i->receiver_b(), i->attribute_b(), ILink::LINK_TYPE::UD);

    d_links_in_ctx.erase(source);
}

void LinkingManager::remove_all_partaking_linking_relations(const std::string &source)
{
    remove_all_source_linking_relations(source);

    for(int i = d_links.size() - 1; i >= 0; --i)
    {
        if(!d_links[i]->is_external() && (d_links[i]->sender_a()->uuid() == source || d_links[i]->receiver_b()->uuid() == source))
        {
            d_links.erase(d_links.begin() + i);
        }
    }

//    d_links.erase(std::remove_if(d_links.begin(), d_links.end(), [&](auto& link)
//    {
//        if(link->is_external())
//            return false;
//
//        return link->sender_a()->uuid() == source || link->receiver_b()->uuid() == source;
//    }), d_links.end());
}

void LinkingManager::remove_linking_relations(const std::vector<LinkCandidate> &relations, const std::string &source)
{
    if (d_links_in_ctx.count(source))
    {
        for(int i = d_links_in_ctx[source].size() - 1; i >= 0; --i)
        {
            bool found = false;

            for(const LinkCandidate& x: relations)
            {
                found = d_links_in_ctx[source][i]->sender_a()->uuid() == x.sender &&
                        d_links_in_ctx[source][i]->attribute_a() == x.sender_attrib &&
                        d_links_in_ctx[source][i]->receiver_b()->uuid() == x.recv &&
                        d_links_in_ctx[source][i]->attribute_b() == x.recv_attrib;
            }

            if(!found)
            {
                remove_link(d_links_in_ctx[source][i]->sender_a(), d_links_in_ctx[source][i]->attribute_a(), d_links_in_ctx[source][i]->receiver_b(), d_links_in_ctx[source][i]->attribute_b(), ILink::LINK_TYPE::UD);
                d_links_in_ctx[source].erase(d_links_in_ctx[source].begin() + i);
            }
        }

//        d_links_in_ctx[source].erase(std::remove_if(d_links_in_ctx[source].begin(), d_links_in_ctx[source].end(), [&](auto& link) -> bool
//        {
//            if(std::find_if(relations.begin(), relations.end(), [&link](const LinkCandidate &x)
//            {
//                return link->sender_a()->uuid() == x.sender &&
//                       link->attribute_a() == x.sender_attrib &&
//                       link->receiver_b()->uuid() == x.recv &&
//                       link->attribute_b() == x.recv_attrib;
//            }) == relations.end())
//            {
//                remove_link(link->sender_a(), link->attribute_a(), link->receiver_b(), link->attribute_b(), ILink::LINK_TYPE::UD);
//                return true;
//            }
//
//            return false;
//        }), d_links_in_ctx[source].end());
    }
}

void LinkingManager::create_linking_relations(std::vector<LinkCandidate> &candidates, const std::string &source)
{
    if (!d_links_in_ctx.count(source))
    {
        d_links_in_ctx[source] = std::vector<std::shared_ptr<ILink>>();
        d_links_in_ctx[source].reserve(candidates.size());
    }

    for(auto& relation: candidates)
    {
        std::shared_ptr<Region> sender;
        std::shared_ptr<Region> receiver;

        for(auto& region: Context::SIContext()->region_manager()->regions())
        {
            if(region->uuid() == relation.sender)
            {
                sender = region;
            }

            if(region->uuid() == relation.recv)
            {
                receiver = region;
            }
        }

        if(sender.get() && receiver.get())
        {
            if(sender.get() != Context::SIContext()->region_manager()->regions().end()->get() && receiver.get() != Context::SIContext()->region_manager()->regions().end()->get())
            {
                if (add_link(sender, relation.sender_attrib, receiver, relation.recv_attrib, ILink::LINK_TYPE::UD))
                {
                    d_links_in_ctx[source].push_back(std::make_shared<UnidirectionalLink>(sender, receiver, relation.sender_attrib, relation.recv_attrib));

                    Q_EMIT (sender)->LINK_SIGNAL(_UUID_, (sender)->uuid(), relation.sender_attrib, (sender)->effect()->attr_link_emit()[relation.sender_attrib]());
                }
            }
        }

//        auto sender = std::find_if(Context::SIContext()->region_manager()->regions().begin(), Context::SIContext()->region_manager()->regions().end(), [&relation](auto& region)
//        {
//            return region->uuid() == relation.sender;
//        });
//
//        auto receiver = std::find_if(Context::SIContext()->region_manager()->regions().begin(), Context::SIContext()->region_manager()->regions().end(), [&relation](auto& region)
//        {
//            return region->uuid() == relation.recv;
//        });
//
//        if(sender != Context::SIContext()->region_manager()->regions().end() && receiver != Context::SIContext()->region_manager()->regions().end())
//        {
//            if (add_link(*sender, relation.sender_attrib, *receiver, relation.recv_attrib, ILink::LINK_TYPE::UD))
//            {
//                d_links_in_ctx[source].push_back(std::make_shared<UnidirectionalLink>(*sender, *receiver, relation.sender_attrib, relation.recv_attrib));
//
//                Q_EMIT (*sender)->LINK_SIGNAL(_UUID_, (*sender)->uuid(), relation.sender_attrib, (*sender)->effect()->attr_link_emit()[relation.sender_attrib]());
//            }
//        }
    }
}
