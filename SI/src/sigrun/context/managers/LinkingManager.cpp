

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>

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

void LinkingManager::remove_link(std::shared_ptr<Region> &ra, const std::string& aa, std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE& type)
{
    if(is_linked(ra, aa, rb, ab, type))
    {
        if(type == ILink::LINK_TYPE ::UD)
        {
            disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
            d_linking_graph->remove_link(ra, aa, rb, ab, type);
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
