

#ifndef SI_CONTEXT_HPP
#define SI_CONTEXT_HPP

#include <boost/python.hpp>
#include <vector>

namespace bp = boost::python;

class Context
{
public:
    Context(int width, int height, const std::vector<bp::object>& plugins);
    ~Context();

    void begin();
    void end();

    int width() const;
    int height() const;

    void set_width(int width);
    void set_height(int height);

    const std::vector<bp::object>& plugins() const;
    void add_plugin(const bp::object& plugin);

private:
    int d_width, d_height;
    std::vector<bp::object> d_plugins;


protected:
};


#endif //SI_CONTEXT_HPP
