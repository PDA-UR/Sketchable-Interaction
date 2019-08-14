

#ifndef SI_LAYER_HPP
#define SI_LAYER_HPP


#include <string>
#include <iostream>

namespace SI
{
    class Layer
    {
    public:
        Layer(int id);

        ~Layer();

        int id() const;

        friend std::ostream &operator<<(std::ostream &stream, const Layer &layer);

        friend std::ostream &operator<<(std::ostream &stream, const Layer *layer);

    protected:
    private:
        int d_id;
    };
}

#endif //SI_LAYER_HPP
