

#ifndef SI_SIGRUN_EGenerator
#define SI_SIGRUN_EGenerator

#include <sigrun/SIObject.hpp>
#include <pugixml.hpp>
#include <vector>

class E
{
public:
    static void generate();

private:
    static void parse(std::string& constants);
    static void print(std::string& constants);

    static std::string fetch_id(const pugi::xml_node &node);
    static std::string fetch_color(const pugi::xml_node &node);
    static std::string fetch_capability(const pugi::xml_node &node);

    static std::string color_from_hex(const std::string& hex);

    static bool is_hex(const std::string& hex);

};

#endif // SI_SIGRUN_EGenerator