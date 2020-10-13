
#include "E.hpp"

#include <string>
#include <fstream>
#include <streambuf>
#include <iostream>
#include <pugixml.hpp>
#include <zconf.h>

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
    doc.load_file((directory + "/plugins/env/values/values.xml").c_str());

    for(auto node: doc.children("environment"))
    {
        for(auto n: node.children())
        {
            std::string identifier(n.attributes().begin()->value());
            std::string type(n.attributes().begin()->next_attribute().value());
            std::string value = "";

            if(type == "str")
                value = "\"" + std::string(n.begin()->value()) + "\"";
            else if(type == "color")
                value = "PySI.Color" + std::string(n.begin()->value());
            else // bool, int, float
                value = std::string(n.begin()->value());

            constants += std::string("\t\t" + identifier + " = " + value + "\n");
        }
    }
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