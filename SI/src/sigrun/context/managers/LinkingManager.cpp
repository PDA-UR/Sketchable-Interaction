

#include "LinkingManager.hpp"
#include <sigrun/log/Log.hpp>

LinkingManager::LinkingManager()
{SIGRUN

}

LinkingManager::~LinkingManager()
{
    INFO("Destroying LinkingManager...");
    INFO("Erasing Linking Relationships...");
    for(auto& [key, value]: d_links)
        delete key;

    d_links.erase(d_links.begin(), d_links.end());
    INFO("Linking Relationships!");
    INFO("Destroying LinkingManager...");
}

void LinkingManager::add_link(const std::string& ra, const std::string& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type)
{
    if(type == ILink::LINK_TYPE::UD)
    {
        d_links.insert({new UnidirectionalLink(ra, rb, aa, ab), true});
    }
    else if(type == ILink::LINK_TYPE::BD)
    {
//        d_links.insert({});
    }
}

const ILink* LinkingManager::link(const std::string &ra, const std::string &rb, const std::string &aa, const std::string &ab,
                     const ILink::LINK_TYPE &type)
{
    if(type == ILink::LINK_TYPE::UD)
    {
        UnidirectionalLink udl(ra, rb, aa, ab);

        if(is_linked(&udl))
            return new UnidirectionalLink(udl); // de-alloc somewhere else
    }
    else if(type == ILink::LINK_TYPE::BD)
    {

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

    }

    return false;
}

bool LinkingManager::is_linked(const ILink* udl)
{
    for(auto& [key, value]: d_links)
    {
        if(key->attribute_a() == udl->attribute_a() &&
           key->attribute_a() == udl->attribute_b() &&
           key->sender_a() == udl->sender_a() &&
           key->receiver_b() == udl->receiver_b())
        {
            return true;
        }
    }

    return false;
}

std::unordered_map<ILink*, bool> LinkingManager::links() const
{
    return d_links;
}
