

#include "Layer.hpp"


Layer::Layer(const int id) : d_id(id)
{

}

Layer::~Layer()
{

}

int Layer::id() const
{
    return d_id;
}
