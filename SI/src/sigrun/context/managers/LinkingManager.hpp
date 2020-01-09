

#ifndef SITEST_LINKINGMANAGER_HPP
#define SITEST_LINKINGMANAGER_HPP

#include <sigrun/SIObject.hpp>
#include <QObject>
#include <sigrun/region/Region.hpp>
#include "../Link.hpp"

class LinkingManager: public QObject, public SIObject
{ Q_OBJECT
public:
    LinkingManager();
    ~LinkingManager();

    void add_link(const std::string& ra, const std::string& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type);


    bool is_linked(const std::string& ra, const std::string& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type);

    const ILink* link(const std::string& ra, const std::string& rb, const std::string& aa, const std::string& ab, const ILink::LINK_TYPE& type);
    std::unordered_map<ILink*, bool> links() const;

private:
    bool is_linked(const ILink* udl);

    std::unordered_map<ILink*, bool> d_links;
};


#endif //SITEST_LINKINGMANAGER_HPP
