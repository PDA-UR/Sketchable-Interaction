

#ifndef SI_LAYER_HPP
#define SI_LAYER_HPP


#include <string>

class Layer
{
public:
    Layer(int id);
    ~Layer();

    int id() const;

protected:
private:
    int d_id;
};


#endif //SI_LAYER_HPP
