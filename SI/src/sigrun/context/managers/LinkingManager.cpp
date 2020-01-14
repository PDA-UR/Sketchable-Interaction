

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>

LinkingManager::LinkingManager()
{SIGRUN
    qRegisterMetaType<std::string>("std::string");
//    qRegisterMetaType<boost::python::object>("boost::python::object");
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
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is already present...");

        UnidirectionalLink udl(ra, rb, aa, ab);

        if(is_linked(&udl))
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
                std::shared_ptr<UnidirectionalLink> l = std::make_shared<UnidirectionalLink>(ra, rb, aa, ab);
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
                bool success = connect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

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
        }

        return false;
    }
    else if(type == ILink::LINK_TYPE::BD)
    {
        INFO("Checking if requested linking relationship (" + aa + "|" + ab + "as " + "unidirectional link) is already present...");

        BidirectionalLink bdl(ra, rb, aa, ab);

        if(is_linked(&bdl))
        {
            INFO("Requested linking relationship is already present!");

            return true;
        }

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

void LinkingManager::remove_link(const std::shared_ptr<Region> &ra, const std::string& aa, const std::shared_ptr<Region> &rb, const std::string &ab, const ILink::LINK_TYPE& type)
{
    if(is_linked(ra, aa, rb, ab, type))
    {
        if (type == ILink::LINK_TYPE::UD)
        {
            UnidirectionalLink udl(ra, rb, aa, ab);

            for (auto it = d_links.begin(); it != d_links.end();)
            {
                auto &key = it->first;

                if (key->attribute_a() == udl.attribute_a() &&
                    key->attribute_b() == udl.attribute_b() &&
                    key->sender_a()->uuid() == udl.sender_a()->uuid() &&
                    key->receiver_b()->uuid() == udl.receiver_b()->uuid() &&
                    key->sender_b()->uuid() == udl.sender_b()->uuid() &&
                    key->receiver_a()->uuid() == udl.receiver_a()->uuid())
                {
                    disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);

                    it = d_links.erase(it);

                    break;
                }
                else
                    ++it;
            }
        }
        else if(type == ILink::LINK_TYPE::BD)
        {
            BidirectionalLink l(ra, rb, aa, ab);

            for (auto it = d_links.begin(); it != d_links.end();)
            {
                auto &key = it->first;

                if ((key->attribute_a() == l.attribute_a() &&
                     key->attribute_b() == l.attribute_b() &&
                     key->sender_a()->uuid() == l.sender_a()->uuid() &&
                     key->receiver_b()->uuid() == l.receiver_b()->uuid() &&
                     key->sender_b()->uuid() == l.sender_b()->uuid() &&
                     key->receiver_a()->uuid() == l.receiver_a()->uuid()) ||
                    (key->attribute_a() == l.attribute_b() &&
                     key->attribute_b() == l.attribute_a() &&
                     key->sender_a()->uuid() == l.sender_b()->uuid() &&
                     key->receiver_b()->uuid() == l.receiver_a()->uuid() &&
                     key->sender_b()->uuid() == l.sender_a()->uuid() &&
                     key->receiver_a() ->uuid()== l.receiver_b()->uuid()))
                {

                    // are all connect killed then?
                    // e.g. (A -> B: pos, pos   and    A -> B: scale, pos)
                    // does the 2nd one still work if the first one is disconnected?
                    disconnect(ra.get(), &Region::LINK_SIGNAL, rb.get(), &Region::LINK_SLOT);
                    disconnect(rb.get(), &Region::LINK_SIGNAL, ra.get(), &Region::LINK_SLOT);

                    it = d_links.erase(it);

                    break;
                }
                else
                    ++it;
            }
        }
    }
}

const std::shared_ptr<ILink> LinkingManager::link(const std::shared_ptr<Region>& ra, const std::string& aa, const std::shared_ptr<Region>& rb, const std::string &ab, const ILink::LINK_TYPE &type)
{
    if(type == ILink::LINK_TYPE::UD)
    {
        UnidirectionalLink udl(ra, rb, aa, ab);

        if(is_linked(&udl))
            return std::make_shared<UnidirectionalLink>(udl);
    }
    else if(type == ILink::LINK_TYPE::BD)
    {
        BidirectionalLink bdl(ra, rb, aa, ab);

        if(is_linked(&bdl))
            return std::make_shared<BidirectionalLink>(bdl);
    }

    return nullptr;
}

bool LinkingManager::is_linked(const std::shared_ptr<Region>& ra, const std::string &aa, const std::shared_ptr<Region>& rb, const std::string &ab,
                          const ILink::LINK_TYPE &type)
{
    if(type == ILink::LINK_TYPE::UD)
    {
        UnidirectionalLink udl(ra, rb, aa, ab);
        return is_linked(&udl);
    }
    else if(type == ILink::LINK_TYPE::BD)
    {
        BidirectionalLink bdl(ra, rb, aa, ab);
        return is_linked(&bdl);
    }

    return false;
}

bool LinkingManager::is_linked(const ILink* l)
{
    for(auto& [key, value]: d_links)
    {
        if(l->type() == ILink::LINK_TYPE::BD)
        {
            if((key->attribute_a() == l->attribute_a() &&
               key->attribute_b() == l->attribute_b() &&
               key->sender_a()->uuid() == l->sender_a()->uuid() &&
               key->receiver_b()->uuid() == l->receiver_b()->uuid() &&
               key->sender_b()->uuid() == l->sender_b()->uuid() &&
               key->receiver_a()->uuid() == l->receiver_a()->uuid()) ||
                    (key->attribute_a() == l->attribute_b() &&
                    key->attribute_b() == l->attribute_a() &&
                    key->sender_a()->uuid() == l->sender_b()->uuid() &&
                    key->receiver_b()->uuid() == l->receiver_a()->uuid() &&
                    key->sender_b()->uuid() == l->sender_a()->uuid() &&
                    key->receiver_a()->uuid() == l->receiver_b()->uuid()))
            {
                return true;
            }
        }
        else if(l->type() == ILink::LINK_TYPE::UD)
        {
            if(key->attribute_a() == l->attribute_a() &&
               key->attribute_b() == l->attribute_b() &&
               key->sender_a()->uuid() == l->sender_a()->uuid() &&
               key->receiver_b()->uuid() == l->receiver_b()->uuid() &&
               key->sender_b()->uuid() == l->sender_b()->uuid() &&
               key->receiver_a()->uuid() == l->receiver_a()->uuid())
            {
                return true;
            }
        }
    }

    return false;
}

std::unordered_map<std::shared_ptr<ILink>, bool> LinkingManager::links() const
{
    return d_links;
}

void LinkingManager::emit_link_event(const std::shared_ptr<Region> &sender, const std::string &sender_attribute,
                                     const std::string &receiver_attribute, const std::string &uuid)
{
    const bp::list& py_list = bp::extract<bp::list>(sender->effect().attr("cap_link_emit")[sender_attribute]());

    sender->register_link_event(uuid, receiver_attribute);
    Q_EMIT sender->LINK_SIGNAL(uuid, sender_attribute, receiver_attribute, py_list);
}
