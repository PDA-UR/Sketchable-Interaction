

#include "Layer.hpp"

namespace SI
{
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

    std::ostream &operator<<(std::ostream &stream, const Layer &layer)
    {
        return stream << std::string("Layer with id: ") << layer.id();
    }

    std::ostream &operator<<(std::ostream &stream, const Layer *layer)
    {
        return stream << std::string("Layer with id: ") << layer->id();
    }
}