

#include "Layer.hpp"
#include <iostream>


Layer::Layer(const int id) : d_id(id)
{

}

Layer::~Layer() = default;

int Layer::id() const
{
    return d_id;
}

std::ostream& operator <<(std::ostream &stream, const Layer &layer)
{
    return stream << std::string("Layer with id: ") << layer.id();
}

std::ostream& operator <<(std::ostream &stream, const Layer* layer)
{
    return stream << std::string("Layer with id: ") << layer->id();
}
