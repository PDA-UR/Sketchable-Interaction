
#include "E.hpp"

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <zconf.h>
#include <algorithm>
#include <sstream>

void E::generate()
{
    std::string constants = "";
    parse(constants);
    print(constants);
}

void E::parse(std::string& constants)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    pugi::xml_document doc;
    doc.load_file((directory + "/plugins/env/values.xml").c_str());

    std::vector<std::string> ids, colors, collision_capabilities, linking_capabilities;

    for(const auto& node: doc.children("environment"))
    {
        for(const auto& n: node.children())
        {
            if (std::string(n.name()) == "id")
                ids.push_back(fetch_id(n));

            if (std::string(n.name()) == "color")
                colors.push_back(fetch_color(n));

            if (std::string(n.name()) == "capability")
            {
                std::string type(n.attributes().begin()->next_attribute().value());

                if(type == "collision")
                    collision_capabilities.push_back(fetch_capability(n));

                if(type == "link")
                    linking_capabilities.push_back(fetch_capability(n));
            }
        }

        for(const auto& id: ids)
            constants += id;

        constants += "\tclass color:\n";

        for(const std::string& color: colors)
            constants += color;

        constants += "\tclass capability:\n";

        for(const std::string& capability: collision_capabilities)
            constants += capability;

        for(const std::string& capability: linking_capabilities)
            constants += capability;
    }
}

std::string E::fetch_id(const pugi::xml_node &n)
{
    const std::string value = n.begin()->value();

    // IS_FLOAT
    std::istringstream iss(value);
    float flt;
    iss >> std::noskipws >> flt;

    if(iss && iss.eof())
        return std::string("\t\t") + n.attributes().begin()->value() + " = " + value + "\n";

    // IS_INT
    if(std::find_if(value.begin(), value.end(), [](u_char c)
    {
        return !std::isdigit(c);
    }) == value.end())
    {
        return std::string("\t\t") + n.attributes().begin()->value() + " = " + value + "\n";
    }

    // IS_BOOL
    if(value == "true" || value == "True")
        return std::string("\t\t") + n.attributes().begin()->value() + " = True\n";

    if(value == "false" || value == "False")
        return std::string("\t\t") + n.attributes().begin()->value() + " = False\n";

    // IS_STR
    return std::string("\t\t") + n.attributes().begin()->value() + " = \"" + value + "\"\n";
}

std::string E::fetch_color(const pugi::xml_node &n)
{
    std::string identifier(n.attributes().begin()->value());
    std::string value = std::string(n.begin()->value());

    if(is_hex(value))
        return "\t\t" + identifier + " = " + color_from_hex(value) + "\n";

    return "\t\t" + identifier + " = " + std::string("PySI.Color(") + value + ")\n";
}

std::string E::fetch_capability(const pugi::xml_node &n)
{
    return std::string("\t\t") + n.attributes().begin()->value() + " = \"" + n.begin()->value() + "\"\n";
}

std::string E::color_from_hex(const std::string &hex)
{
    uint32_t value = std::stoul(hex, nullptr, 16);
    uint8_t *rgba = (uint8_t*) &value;

    int endianess = 1;
    int r, g, b, a;

    if(*(char *) &endianess == 1) // little
    {
        a = rgba[0];
        b = rgba[1];
        g = rgba[2];
        r = rgba[3];
    }
    else // big
    {
        r = rgba[0];
        g = rgba[1];
        b = rgba[2];
        a = rgba[3];
    }

    return (std::string("PySI.Color(") + std::to_string(r) + ", " + std::to_string(g) + ", " + std::to_string(b) + ", " + std::to_string(a) + ")");
}

bool E::is_hex(const std::string &hex)
{
    if(hex.size() != 8)
        return false;

    return hex.find_first_not_of("abcdefABCDEF1234567890") == std::string::npos;
}

void E::print(std::string& constants)
{
    std::string warning = "\"\"\"\nTHIS FILE IS AUTO-GENERATED!\nIt is not recommended to make manual changes to this file!\nThis file is generated according to env/values/values.xml\n\"\"\"\n\n\n";

    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    std::ofstream E;

    E.open(directory + "/plugins/E.py", std::ios::trunc);
    E << warning + "from libPySI import PySI\n\n\nclass E:\n\tclass id:\n" + constants;
    E.close();
}

int main()
{
    E::generate();
}