

#ifndef SITEST_LINK_HPP
#define SITEST_LINK_HPP

#include <string>
#include <sigrun/SIObject.hpp>
#include <QHash>
#include <QList>
#include <QMetaObject>
#include <QObject>

#include <sigrun/region/Region.hpp>
#include "../input/ExternalObject.hpp"

class ILink: public SIObject
{ SIGRUN
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
    [[nodiscard]] virtual const std::shared_ptr<ExternalObject>& external_sender_a() const = 0;
    [[nodiscard]] virtual const std::string& attribute_a() const = 0;
    [[nodiscard]] virtual const std::string& attribute_b() const = 0;

    virtual const bool is_external() const = 0;

    virtual void add_child(std::shared_ptr<ILink>& link) = 0;
    [[nodiscard]] virtual std::vector<std::shared_ptr<ILink>>& children() = 0;
};

class UnidirectionalLink: public ILink
{
public:
    UnidirectionalLink(const std::shared_ptr<Region>& ra, const std::shared_ptr<Region>& rb, const std::string& aa, const std::string& ab);
    UnidirectionalLink(const std::shared_ptr<ExternalObject>& eo, const std::shared_ptr<Region>& ra, const std::string& aa, const std::string& ab);
    ~UnidirectionalLink();

    [[nodiscard]] const LINK_TYPE& type() const override;
    [[nodiscard]] const std::shared_ptr<Region>& sender_a() const override;
    [[nodiscard]] const std::shared_ptr<Region>& sender_b() const override;
    [[nodiscard]] const std::shared_ptr<Region>& receiver_a() const override;
    [[nodiscard]] const std::shared_ptr<Region>& receiver_b() const override;
    [[nodiscard]] const std::shared_ptr<ExternalObject>& external_sender_a() const override;
    [[nodiscard]] const std::string& attribute_a() const override;
    [[nodiscard]] const std::string& attribute_b() const override;
    const bool is_external() const override;
    virtual void add_child(std::shared_ptr<ILink>& link) override;
    [[nodiscard]] std::vector<std::shared_ptr<ILink>>& children() override;

private:
    LINK_TYPE d_link_type;

    std::shared_ptr<Region> d_sender_a;
    std::shared_ptr<Region> d_sender_b;
    std::shared_ptr<Region> d_receiver_a;
    std::shared_ptr<Region> d_receiver_b;
    std::string d_attribute_a;
    std::string d_attribute_b;

    bool d_is_external;
    std::shared_ptr<ExternalObject> d_external_sender_a;


    std::vector<std::shared_ptr<ILink>> d_children;
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
    [[nodiscard]] const std::shared_ptr<ExternalObject>& external_sender_a() const override;
    [[nodiscard]] const std::string& attribute_a() const override;
    [[nodiscard]] const std::string& attribute_b() const override;
    virtual void add_child(std::shared_ptr<ILink>& link) override;
    [[nodiscard]] std::vector<std::shared_ptr<ILink>>& children() override;
    const bool is_external() const override;

private:
    LINK_TYPE d_link_type;

    std::shared_ptr<Region> d_sender_a;
    std::shared_ptr<Region> d_sender_b;
    std::shared_ptr<Region> d_receiver_a;
    std::shared_ptr<Region> d_receiver_b;
    std::string d_attribute_a;
    std::string d_attribute_b;
    std::shared_ptr<ExternalObject> d_external_sender_a;
    bool d_is_external = false;

    std::vector<std::shared_ptr<ILink>> d_children;
};

#endif //SITEST_LINK_HPP
