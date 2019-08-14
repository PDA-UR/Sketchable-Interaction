

#ifndef SI_PLUGIN_HPP
#define SI_PLUGIN_HPP

#include <boost/python.hpp>
#include <ostream>

namespace SI
{
    namespace bp = boost::python;

    class Plugin
    {
    public:
        Plugin() = default;

        ~Plugin() = default;

        virtual int on_enter() = 0;

        virtual int on_continuous() = 0;

        virtual int on_leave() = 0;

        void set_pyplugin(const bp::object &plugin);

        bp::object d_pyplugin;
    };

    class SIPlugin : public Plugin
    {
    public:
        SIPlugin() = default;

        explicit SIPlugin(const bp::object &pyplugin);

        ~SIPlugin() = default;

        int on_enter() override;

        int on_continuous() override;

        int on_leave() override;

        friend std::ostream &operator<<(std::ostream &os, const SIPlugin &plugin);
    };
}

#endif //SI_PLUGIN_HPP
