

#include "Link.hpp"

UnidirectionalLink::UnidirectionalLink(const std::shared_ptr<Region> &ra, const std::shared_ptr<Region> &rb, const std::string &aa,
                                       const std::string &ab):
    d_sender_a(ra),
    d_sender_b(rb),
    d_receiver_a(ra),
    d_receiver_b(rb),
    d_attribute_a(aa),
    d_attribute_b(ab),
    is_external(false),
    d_link_type(LINK_TYPE::UD)
{

}

UnidirectionalLink::UnidirectionalLink(const ExternalObject::ExternalObjectType &type, const std::shared_ptr<Region> &ra,
                                       const std::string &aa, const std::string &ab):
    d_sender_a(ra),
    d_sender_b(nullptr),
    d_receiver_a(ra),
    d_receiver_b(nullptr),
    d_attribute_a(aa),
    d_attribute_b(ab),
    is_external(true),
    external_sender_a(type),
    d_link_type(LINK_TYPE::UD)
{

}

UnidirectionalLink::~UnidirectionalLink()
{

}

const ILink::LINK_TYPE& UnidirectionalLink::type() const
{
    return d_link_type;
}

const std::shared_ptr<Region>& UnidirectionalLink::sender_a() const
{
    return d_sender_a;
}

const std::shared_ptr<Region>& UnidirectionalLink::sender_b() const
{
    return d_sender_b;
}

const std::shared_ptr<Region>& UnidirectionalLink::receiver_a() const
{
    return d_receiver_a;
}

const std::shared_ptr<Region>& UnidirectionalLink::receiver_b() const
{
    return d_receiver_b;
}

const std::string& UnidirectionalLink::attribute_a() const
{
    return d_attribute_a;
}

const std::string& UnidirectionalLink::attribute_b() const
{
    return d_attribute_b;
}

BidirectionalLink::BidirectionalLink(const std::shared_ptr<Region> &ra, const std::shared_ptr<Region> &rb, const std::string &aa,
                                     const std::string &ab):
    d_sender_a(ra),
    d_sender_b(rb),
    d_receiver_a(ra),
    d_receiver_b(rb),
    d_attribute_a(aa),
    d_attribute_b(ab),
    d_link_type(LINK_TYPE::BD)
{

}

BidirectionalLink::~BidirectionalLink()
{

}

const ILink::LINK_TYPE &BidirectionalLink::type() const
{
    return d_link_type;
}

const std::shared_ptr<Region> &BidirectionalLink::sender_a() const
{
    return d_sender_a;
}

const std::shared_ptr<Region> &BidirectionalLink::sender_b() const
{
    return d_sender_b;
}

const std::shared_ptr<Region> &BidirectionalLink::receiver_a() const
{
    return d_receiver_a;
}

const std::shared_ptr<Region> &BidirectionalLink::receiver_b() const
{
    return d_receiver_b;
}

const std::string &BidirectionalLink::attribute_a() const
{
    return d_attribute_a;
}

const std::string &BidirectionalLink::attribute_b() const
{
    return d_attribute_b;
}

std::vector<std::shared_ptr<ILink>>& UnidirectionalLink::children()
{
    return d_children;
}

std::vector<std::shared_ptr<ILink>>& BidirectionalLink::children()
{
    return d_children;
}

void UnidirectionalLink::add_child(std::shared_ptr<ILink> &link)
{
    d_children.push_back(link);
}

void BidirectionalLink::add_child(std::shared_ptr<ILink> &link)
{
    d_children.push_back(link);
}
