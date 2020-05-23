

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

        if(dra.count(aa) && drb.count(aa))
        {
            if(drb[aa].count(ab))
            {
                INFO("Requested linking relationship is defined!");
                INFO("Establishing Unidirectional Link...");

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
                bool success = connect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT, Qt::UniqueConnection);

                if(success)
                {
                    d_links.push_back(std::make_shared<UnidirectionalLink>(ra, rb, aa, ab));

                    INFO("Establishing Unidirectional Link Successfull!");
                }
                else
                {
                    disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

                    INFO("Establishing Unidirectional Link Failed!");
                }

                return success;
            }
        }

        return false;
    }
    else if(type == ILink::LINK_TYPE::BD)
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

void LinkingManager::add_link(std::shared_ptr<ExternalObject>& eo, std::shared_ptr<Region> &a, const std::string& ea, const std::string& aa)
{
    if(is_linked(eo, ea, a, aa))
        return;

    disconnect(eo.get(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT);
    if(connect(eo.get(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT, Qt::UniqueConnection))
    {
        d_links.push_back(std::make_shared<UnidirectionalLink>(Context::SIContext()->input_manager()->external_objects()[a->uuid()], a, ea, aa));

        INFO("Establishing unidirectional link to external object successfull!");
    }
    else
    {
        INFO("Establishing unidirectional link to external object failed!");
    }
}

void LinkingManager::remove_link(std::shared_ptr<ExternalObject>& eo, std::shared_ptr<Region> &a, const std::string &ea, const std::string &aa)
{
    if(is_linked(eo, ea, a, aa))
    {
        disconnect(eo.get(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT);

        d_links.erase(std::remove_if(d_links.begin(), d_links.end(), [&](auto &link)
        {
            if(!link->is_external())
                return false;

            return (link->attribute_a() == ea &&
                    link->attribute_b() == aa &&
                    link->external_sender_a()->uuid() == eo->uuid() &&
                    link->receiver_b()->uuid() == a->uuid());
        }), d_links.end());
    }
}

void LinkingManager::remove_links_by_indices(std::vector<uint32_t>& indices)
{
    std::for_each(indices.begin(), indices.end(), [&](uint32_t i)
    {
        remove_link(d_links[i]->sender_a(), d_links[i]->attribute_a(), d_links[i]->receiver_b(), d_links[i]->attribute_b(), ILink::LINK_TYPE::UD);
    });
}

void LinkingManager::remove_link(const std::shared_ptr<Region> &ra, const std::string& aa, const std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE& type)
{
    if(is_linked(ra, aa, rb, ab, type))
    {
        if(type == ILink::LINK_TYPE ::UD)
        {
            INFO("Requested Deletion of UD Link between " + ra->name() + " and " + rb->name());
            disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

            d_links.erase(std::remove_if(d_links.begin(), d_links.end(),[&](auto& link)
            {
                if(link->is_external())
                    return false;

                return (link->attribute_a() == aa &&
                        link->attribute_b() == ab &&
                        link->sender_a()->uuid() == ra->uuid() &&
                        link->receiver_b()->uuid() == rb->uuid() &&
                        link->sender_b()->uuid() == rb->uuid() &&
                        link->receiver_a()->uuid() == ra->uuid());
            }), d_links.end());

            INFO("Requested Deletion of UD Link successful!");
        }
        else if(type == ILink::LINK_TYPE ::BD)
        {
            INFO("Requested Deletion of BD Link between " + ra->name() + " and " + rb->name());

            disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
            disconnect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);

            d_links.erase(std::remove_if(d_links.begin(), d_links.end(),[&](auto& link)
            {
                return ((link->attribute_a() == aa &&
                         link->attribute_b() == ab &&
                         link->sender_a()->uuid() == ra->uuid() &&
                         link->receiver_b()->uuid() == rb->uuid() &&
                         link->sender_b()->uuid() == rb->uuid() &&
                         link->receiver_a()->uuid() == ra->uuid()) ||
                        (link->attribute_a() == ab &&
                         link->attribute_b() == aa &&
                         link->sender_a()->uuid() == rb->uuid() &&
                         link->receiver_b()->uuid() == ra->uuid() &&
                         link->sender_b()->uuid() == ra->uuid() &&
                         link->receiver_a()->uuid() == rb->uuid()));
            }), d_links.end());

            INFO("Requested Deletion of BD Link successful!");

        }
    }
}

bool LinkingManager::is_linked(const std::shared_ptr<Region>& ra, const std::string &aa, const std::shared_ptr<Region>& rb, const std::string &ab, const ILink::LINK_TYPE &type)
{
    return is_linked(ra->uuid(), aa, rb->uuid(), ab, type);
}

bool LinkingManager::is_linked(const std::string& ra_uuid, const std::string& aa, const std::string& rb_uuid, const std::string& ab, const ILink::LINK_TYPE& type)
{
    return std::transform_reduce(d_links.begin(), d_links.end(), false, [](bool a, bool b)
    {
        return a || b;
    }, [&](auto& link)
    {
        if(link->is_external())
            return false;

        return type == ILink::LINK_TYPE::UD ? (link->attribute_a() == aa &&
                                                 link->attribute_b() == ab &&
                                                 link->sender_a()->uuid() == ra_uuid &&
                                                 link->receiver_b()->uuid() == rb_uuid &&
                                                 link->sender_b()->uuid() == rb_uuid &&
                                                 link->receiver_a()->uuid() == ra_uuid)
                                                 :
                                                (link->attribute_a() == aa &&
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
                                                 link->receiver_a()->uuid() == rb_uuid);
    });
}

bool LinkingManager::is_linked(const std::shared_ptr<ExternalObject> &eo, const std::string &ea, const std::shared_ptr<Region> &ra, const std::string &aa)
{
    return is_linked(eo->uuid(), ea, ra->uuid(), aa);
}

bool LinkingManager::is_linked(const std::string &eo_uuid, const std::string &ea, const std::string &ra_uuid, const std::string &aa)
{
    return std::transform_reduce(d_links.begin(), d_links.end(), false, [](bool a, bool b)
    {
        return a || b;
    }, [&](auto& link)
    {
        if (!link->is_external())
            return false;

        return (link->attribute_a() == ea &&
                link->attribute_b() == aa &&
                link->external_sender_a()->uuid() == eo_uuid &&
                link->receiver_b()->uuid() == ra_uuid);
    });
}

void LinkingManager::emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a)
{
    const std::string uuid_event = _UUID_;

    const bp::tuple &args = bp::extract<bp::tuple>(a->effect()->attr_link_emit()[attr_a]());

    a->register_link_event({uuid_event, attr_a});

    Q_EMIT a->LINK_SIGNAL(uuid_event, a->uuid(), attr_a, args);
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
        remove_all_source_linking_relations(source);
    else
    {
        remove_linking_relations(candidates, source);
        create_linking_relations(candidates, source);
    }
}

void LinkingManager::remove_all_source_linking_relations(const std::string &source)
{
    if(MAP_HAS_KEY(d_links_in_ctx, source))
    {
        std::for_each(d_links_in_ctx[source].begin(), d_links_in_ctx[source].end(), [&](auto& i)
        {
            remove_link(i->sender_a(), i->attribute_a(), i->receiver_b(), i->attribute_b(), ILink::LINK_TYPE::UD);
        });

        d_links_in_ctx.erase(source);
    }
}

void LinkingManager::remove_all_partaking_linking_relations(const std::string &source)
{
    remove_all_source_linking_relations(source);

    d_links.erase(std::remove_if(d_links.begin(), d_links.end(), [&](auto& link)
    {
        if(link->is_external())
            return false;

        return link->sender_a()->uuid() == source || link->receiver_b()->uuid() == source;
    }), d_links.end());
}

void LinkingManager::remove_linking_relations(const std::vector<LinkCandidate> &relations, const std::string &source)
{
    if (MAP_HAS_KEY(d_links_in_ctx, source))
    {
        d_links_in_ctx[source].erase(std::remove_if(d_links_in_ctx[source].begin(), d_links_in_ctx[source].end(), [&](auto& link) -> bool
        {
            if(std::find_if(relations.begin(), relations.end(), [&link](const LinkCandidate &x)
            {
                return link->sender_a()->uuid() == x.sender &&
                       link->attribute_a() == x.sender_attrib &&
                       link->receiver_b()->uuid() == x.recv &&
                       link->attribute_b() == x.recv_attrib;
            }) == relations.end())
            {
                remove_link(link->sender_a(), link->attribute_a(), link->receiver_b(), link->attribute_b(), ILink::LINK_TYPE::UD);
                return true;
            }

            return false;
        }), d_links_in_ctx[source].end());
    }
}

void LinkingManager::create_linking_relations(std::vector<LinkCandidate> &candidates, const std::string &source)
{
    if (!MAP_HAS_KEY(d_links_in_ctx, source))
    {
        d_links_in_ctx[source] = std::vector<std::shared_ptr<ILink>>();
        d_links_in_ctx[source].reserve(candidates.size());
    }

    std::for_each(candidates.begin(), candidates.end(), [&](auto& relation)
    {
        auto sender = std::find_if(Context::SIContext()->region_manager()->regions().begin(), Context::SIContext()->region_manager()->regions().end(), [&relation](auto& region)
        {
            return region->uuid() == relation.sender;
        });

        auto receiver = std::find_if(Context::SIContext()->region_manager()->regions().begin(), Context::SIContext()->region_manager()->regions().end(), [&relation](auto& region)
        {
            return region->uuid() == relation.recv;
        });

        if(sender != Context::SIContext()->region_manager()->regions().end() && receiver != Context::SIContext()->region_manager()->regions().end())
        {
            if (add_link(*sender, relation.sender_attrib, *receiver, relation.recv_attrib, ILink::LINK_TYPE::UD))
            {
                d_links_in_ctx[source].push_back(std::make_shared<UnidirectionalLink>(*sender, *receiver, relation.sender_attrib, relation.recv_attrib));

                Q_EMIT (*sender)->LINK_SIGNAL(_UUID_, (*sender)->uuid(), relation.sender_attrib, bp::extract<bp::tuple>((*sender)->effect()->attr_link_emit()[relation.sender_attrib]()));
            }
        }
    });
}
