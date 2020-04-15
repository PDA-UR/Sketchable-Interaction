

#include "Link.hpp"

UnidirectionalLink::UnidirectionalLink(const std::shared_ptr<Region> &ra, const std::shared_ptr<Region> &rb, const std::string &aa, const std::string &ab):
    d_sender_a(ra),
    d_sender_b(rb),
    d_receiver_a(ra),
    d_receiver_b(rb),
    d_attribute_a(aa),
    d_attribute_b(ab),
    d_is_external(false),
    d_link_type(LINK_TYPE::UD)
{

}

UnidirectionalLink::UnidirectionalLink(const std::shared_ptr<ExternalObject>& eo, const std::shared_ptr<Region> &ra, const std::string &ea, const std::string &aa):
    d_sender_a(nullptr),
    d_sender_b(nullptr),
    d_receiver_a(nullptr),
    d_receiver_b(ra),
    d_attribute_a(ea),
    d_attribute_b(aa),
    d_is_external(true),
    d_external_sender_a(eo),
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

const std::shared_ptr<ExternalObject>& UnidirectionalLink::external_sender_a() const
{
    return d_external_sender_a;
}

const std::string& UnidirectionalLink::attribute_a() const
{
    return d_attribute_a;
}

const std::string& UnidirectionalLink::attribute_b() const
{
    return d_attribute_b;
}

const bool UnidirectionalLink::is_external() const
{
    return d_is_external;
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

const std::shared_ptr<ExternalObject>& BidirectionalLink::external_sender_a() const
{
    return d_external_sender_a;
}

const bool BidirectionalLink::is_external() const
{
    return d_is_external;
}
