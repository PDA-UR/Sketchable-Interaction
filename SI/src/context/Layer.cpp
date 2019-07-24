

#include "Layer.hpp"


Layer::Layer()
{

}

Layer::~Layer()
{

}

const int Layer::id() const
{
    return d_id;
}

const std::string &Layer::name() const
{
    return d_name;
}
