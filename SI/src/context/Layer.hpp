

#ifndef SI_LAYER_HPP
#define SI_LAYER_HPP


#include <string>

class Layer
{
public:
    Layer();
    ~Layer();

    const int id() const;
    const std::string& name() const;

protected:
private:
    std::string d_name;
    int d_id;
};


#endif //SI_LAYER_HPP
