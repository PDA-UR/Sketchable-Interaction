

#ifndef SITEST_LINK_HPP
#define SITEST_LINK_HPP

#include <string>
#include <sigrun/SIObject.hpp>
#include <QHash>
#include <QList>
#include <QMetaObject>
#include <QObject>

#include <sigrun/region/Region.hpp>

class ILink: public SIObject
{
public:
    enum LINK_TYPE
    {
        UD,
        BD
    };

    virtual ~ILink() {}

    [[nodiscard]] virtual const LINK_TYPE& type() const = 0;

    [[nodiscard]] virtual const std::shared_ptr<Region>& sender_a() const = 0;
    [[nodiscard]] virtual const std::shared_ptr<Region>& sender_b() const = 0;
    [[nodiscard]] virtual const std::shared_ptr<Region>& receiver_a() const = 0;
    [[nodiscard]] virtual const std::shared_ptr<Region>& receiver_b() const = 0;
    [[nodiscard]] virtual const std::string& attribute_a() const = 0;
    [[nodiscard]] virtual const std::string& attribute_b() const = 0;
};

class BidirectionalLink;
class UnidirectionalLink: public ILink
{
public:
    UnidirectionalLink(const std::shared_ptr<Region>& ra, const std::shared_ptr<Region>& rb, const std::string& aa, const std::string& ab);
    ~UnidirectionalLink();

    [[nodiscard]] const LINK_TYPE& type() const override;
    [[nodiscard]] const std::shared_ptr<Region>& sender_a() const override;
    [[nodiscard]] const std::shared_ptr<Region>& sender_b() const override;
    [[nodiscard]] const std::shared_ptr<Region>& receiver_a() const override;
    [[nodiscard]] const std::shared_ptr<Region>& receiver_b() const override;
    [[nodiscard]] const std::string& attribute_a() const override;
    [[nodiscard]] const std::string& attribute_b() const override;

    // might be useful
    const BidirectionalLink to_bidirectional();

private:
    LINK_TYPE d_link_type;

    std::shared_ptr<Region> d_sender_a;
    std::shared_ptr<Region> d_sender_b;
    std::shared_ptr<Region> d_receiver_a;
    std::shared_ptr<Region> d_receiver_b;
    std::string d_attribute_a;
    std::string d_attribute_b;
};

class BidirectionalLink: public ILink
{

public:
    BidirectionalLink(const std::shared_ptr<Region>& ra, const std::shared_ptr<Region>& rb, const std::string& aa, const std::string& ab);
    ~BidirectionalLink();

    [[nodiscard]] const LINK_TYPE& type() const override;
    [[nodiscard]] const std::shared_ptr<Region>& sender_a() const override;
    [[nodiscard]] const std::shared_ptr<Region>& sender_b() const override;
    [[nodiscard]] const std::shared_ptr<Region>& receiver_a() const override;
    [[nodiscard]] const std::shared_ptr<Region>& receiver_b() const override;
    [[nodiscard]] const std::string& attribute_a() const override;
    [[nodiscard]] const std::string& attribute_b() const override;

    // might be useful
    const UnidirectionalLink to_unidirectional();

private:
    LINK_TYPE d_link_type;

    std::shared_ptr<Region> d_sender_a;
    std::shared_ptr<Region> d_sender_b;
    std::shared_ptr<Region> d_receiver_a;
    std::shared_ptr<Region> d_receiver_b;
    std::string d_attribute_a;
    std::string d_attribute_b;
};

#endif //SITEST_LINK_HPP
