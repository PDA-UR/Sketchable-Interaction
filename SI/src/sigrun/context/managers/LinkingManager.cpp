

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>
#include <sigrun/context/Context.hpp>

LinkingManager::LinkingManager()
{SIGRUN
    qRegisterMetaType<std::string>("std::string");
}

LinkingManager::~LinkingManager()
{
    INFO("Destroying LinkingManager...");

    INFO("Destroying LinkingManager...");
}

void LinkingManager::add_link(const std::shared_ptr<Region>& ra, const std::shared_ptr<Region>& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type)
{
    if(is_capability_supported(aa) && is_capability_supported(ab))
    {
        if(type == ILink::LINK_TYPE::UD)
        {
            INFO("Establishing Unidirectional Link...");
            std::shared_ptr<UnidirectionalLink> l = std::make_shared<UnidirectionalLink>(ra->uuid(), rb->uuid(), aa, ab);
            d_links.insert({l, true});
            dynamicConnection(ra.get(), capability_signal_function(aa).c_str(), rb.get(), capability_slot_function(ab).c_str());

            INFO("Unidirectional Link established!");
        }
        else if(type == ILink::LINK_TYPE::BD)
        {
            std::shared_ptr<BidirectionalLink> l = std::make_shared<BidirectionalLink>(ra->uuid(), rb->uuid(), aa, ab);
            d_links.insert({l, true});
            dynamicConnection(ra.get(), capability_signal_function(aa).c_str(), rb.get(), capability_slot_function(ab).c_str());
            dynamicConnection(rb.get(), capability_signal_function(ab).c_str(), ra.get(), capability_slot_function(aa).c_str());
        }

    }
}

void LinkingManager::remove_link(const std::shared_ptr<Region> &ra, const std::shared_ptr<Region> &rb, const std::string &aa,
                            const std::string &ab, const ILink::LINK_TYPE type)
{
    if(is_linked(ra->uuid(), rb->uuid(), aa, ab, type))
    {
        if (type == ILink::LINK_TYPE::UD)
        {
            UnidirectionalLink udl(ra->uuid(), rb->uuid(), aa, ab);

            for (auto it = d_links.begin(); it != d_links.end();)
            {
                auto &key = it->first;

                if (key->attribute_a() == udl.attribute_a() &&
                    key->attribute_b() == udl.attribute_b() &&
                    key->sender_a() == udl.sender_a() &&
                    key->receiver_b() == udl.receiver_b() &&
                    key->sender_b() == udl.sender_b() &&
                    key->receiver_a() == udl.receiver_a())
                {
                    it = d_links.erase(it);

                    break;
                }
                else
                    ++it;
            }
            dynamicDisconnection(ra.get(), capability_signal_function(aa).c_str(), rb.get(), capability_slot_function(ab).c_str());

        }
        else if(type == ILink::LINK_TYPE::BD)
        {
            BidirectionalLink bdl(ra->uuid(), rb->uuid(), aa, ab);

            for (auto it = d_links.begin(); it != d_links.end();)
            {
                auto &key = it->first;

                if (key->attribute_a() == bdl.attribute_a() &&
                    key->attribute_b() == bdl.attribute_b() &&
                    key->sender_a() == bdl.sender_a() &&
                    key->receiver_b() == bdl.receiver_b() &&
                    key->sender_b() == bdl.sender_b() &&
                    key->receiver_a() == bdl.receiver_a())
                {
                    it = d_links.erase(it);

                    break;
                }
                else
                    ++it;
            }

            dynamicDisconnection(ra.get(), capability_signal_function(aa).c_str(), rb.get(), capability_slot_function(ab).c_str());
            dynamicDisconnection(rb.get(), capability_signal_function(ab).c_str(), ra.get(), capability_slot_function(aa).c_str());

        }
    }
}

const std::shared_ptr<ILink> LinkingManager::link(const std::string &ra, const std::string &rb, const std::string &aa, const std::string &ab,
                     const ILink::LINK_TYPE &type)
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

bool LinkingManager::is_linked(const std::string &ra, const std::string &rb, const std::string &aa, const std::string &ab,
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
        if(key->attribute_a() == l->attribute_a() &&
           key->attribute_b() == l->attribute_b() &&
           key->sender_a() == l->sender_a() &&
           key->receiver_b() == l->receiver_b() &&
           key->sender_b() == l->sender_b() &&
           key->receiver_a() == l->receiver_a())
        {
            return true;
        }
    }

    return false;
}

std::unordered_map<std::shared_ptr<ILink>, bool> LinkingManager::links() const
{
    return d_links;
}

void LinkingManager::add_capability_functions(const std::string& name, const std::string &signal, const std::string &slot)
{
    d_capability_functions.insert({name, std::vector<std::string> {signal, slot}});
}

bool LinkingManager::is_capability_supported(const std::string &capability)
{
    return d_capability_functions.find(capability) != d_capability_functions.end();
}

const std::string &LinkingManager::capability_signal_function(const std::string &capability)
{
    return d_capability_functions[capability][0];
}

const std::string &LinkingManager::capability_slot_function(const std::string &capability)
{
    return d_capability_functions[capability][1];
}

void LinkingManager::emit_link_event(QObject *source, const std::string &signal, const std::string& event_uuid, QGenericArgument arg1, QGenericArgument arg2)
{
    QMetaObject::invokeMethod(source, signal.c_str(), Qt::DirectConnection, arg1, arg2, Q_ARG(const std::string&, event_uuid));
}
