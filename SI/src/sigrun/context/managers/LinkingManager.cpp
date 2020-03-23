

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>

LinkingManager::LinkingManager():
        d_linking_graph(std::make_unique<LinkingGraph>())
{SIGRUN
    qRegisterMetaType<std::string>("std::string");
}

LinkingManager::~LinkingManager()
{
    INFO("Destroying LinkingManager...");

    INFO("Destroying LinkingManager...");
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
        auto& dra = ra->effect().attr_link_emit();
        auto& drb = rb->effect().attr_link_recv();

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
                    d_linking_graph->add_link(ra, aa, rb, ab, type);
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

            return true;
        }

        bool success = add_link(ra, aa, rb, ab, ILink::LINK_TYPE::UD);
        success = add_link(rb, ab, ra, aa, ILink::LINK_TYPE::UD);

        return success;
    }

    return false;
}

void LinkingManager::add_link_to_object(std::shared_ptr<Region> &a, const ExternalObject::ExternalObjectType &type)
{
    switch(type)
    {
        case ExternalObject::ExternalObjectType::MOUSE:
        {
            // needs is_linked, remove_link, etc. upgrades
            d_linking_graph->add_link_to_external_object(UnidirectionalLink(type, a, "__position__", "__position__"));
            bool success = connect(Context::SIContext()->input_manager()->mouse_object(), &ExternalObject::LINK_SIGNAL, a.get(), &Region::LINK_SLOT, Qt::UniqueConnection);
        }
    }
}

void LinkingManager::remove_links_by_indices(const std::vector<int>& indices)
{
    for(auto& i: indices)
    {
        auto& link = d_linking_graph->links()[i];

        remove_link(link->sender_a(), link->attribute_a(), link->receiver_b(), link->attribute_b(), ILink::LINK_TYPE::UD);
    }
}

void LinkingManager::remove_link(const std::shared_ptr<Region> &ra, const std::string& aa, const std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE& type)
{
    if(is_linked(ra, aa, rb, ab, type))
    {
        if(type == ILink::LINK_TYPE ::UD)
        {
            INFO("Requested Deletion of UD Link between " + ra->name() + " and " + rb->name());
            disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
            d_linking_graph->remove_link(ra, aa, rb, ab, type);
            INFO("Requested Deletion of UD Link successful!");
        }
        else if(type == ILink::LINK_TYPE ::BD)
        {
            disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
            disconnect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);
            d_linking_graph->remove_link(ra, aa, rb, ab, ILink::LINK_TYPE::UD);
            d_linking_graph->remove_link(rb, ab, ra, aa, ILink::LINK_TYPE::UD);
        }
    }
}

bool LinkingManager::is_linked(const std::shared_ptr<Region>& ra, const std::string &aa, const std::shared_ptr<Region>& rb, const std::string &ab,
                          const ILink::LINK_TYPE &type)
{
    return d_linking_graph->is_linked(ra, aa, rb, ab, type);
}

void LinkingManager::emit_link_event(std::shared_ptr<Region> &a, const std::string &attr_a)
{
    d_linking_graph->emit_link_event(a, attr_a);
}

const std::vector<std::shared_ptr<ILink>>& LinkingManager::links() const
{
    return d_linking_graph->links();
}

const std::unique_ptr<LinkingGraph> &LinkingManager::linking_graph() const
{
    return d_linking_graph;
}

const int LinkingManager::num_links() const
{
    int num_bd_links = 0;
    int num_ud_links = 0;
    return 0;
}
