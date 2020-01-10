

#include "Link.hpp"

UnidirectionalLink::UnidirectionalLink(const std::string &ra, const std::string &rb, const std::string &aa,
                                       const std::string &ab):
    d_sender_a(ra),
    d_sender_b(""),
    d_receiver_a(""),
    d_receiver_b(rb),
    d_attribute_a(aa),
    d_attribute_b(ab),
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

const std::string& UnidirectionalLink::sender_a() const
{
    return d_sender_a;
}

const std::string& UnidirectionalLink::sender_b() const
{
    return d_sender_b;
}

const std::string& UnidirectionalLink::receiver_a() const
{
    return d_receiver_a;
}

const std::string& UnidirectionalLink::receiver_b() const
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

const BidirectionalLink UnidirectionalLink::to_bidirectional()
{

}

BidirectionalLink::BidirectionalLink(const std::string &ra, const std::string &rb, const std::string &aa,
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

const std::string &BidirectionalLink::sender_a() const
{
    return d_sender_a;
}

const std::string &BidirectionalLink::sender_b() const
{
    return d_sender_b;
}

const std::string &BidirectionalLink::receiver_a() const
{
    return d_receiver_a;
}

const std::string &BidirectionalLink::receiver_b() const
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

const UnidirectionalLink BidirectionalLink::to_unidirectional()
{

}
