
#ifndef SITEST_TEMPLATES_H
#define SITEST_TEMPLATES_H

#include <unistd.h>
#include <string>
#include <boost/algorithm/string.hpp>

#define SI_PLUGIN(name, path) \
    std::string(std::string("from libPySI import PySI\n") + \
    "from plugins.standard_environment_library.SIEffect import SIEffect\n"\
    "\n"\
    "class " + std::string(name) + "(SIEffect):\n"\
        "\tregiontype = PySI.EffectType.SI_CUSTOM\n"\
        "\tregionname = PySI.EffectName.SI_STD_NAME_PLACEHOLDER\n"\
        "\tregion_display_name = \"" + std::string(name) + "\"\n"\
        "\n"\
        "\tdef __init__(self, shape=PySI.PointVector(), uuid=\"\", kwargs={}):\n"\
            "\t\tsuper(" + std::string(name) + ", self).__init__(shape, uuid, \"res/" + name + ".png\", " + std::string(name) + ".regiontype, " + std::string(name) + ".regionname, kwargs)\n"\
            "\t\tself.qml_path = \"" + std::string(path) + "/" + std::string(name) + ".qml\"\n"\
            "\t\tself.color = PySI.Color(55, 55, 55, 255)  # change to your color")

#define SIQML \
    std::string(std::string("import QtQuick 2.7\n") + \
    "\n"\
    "Item {\n"\
        "\tfunction updateData(data) {\n"\
            "\t\timage.width = data.img_width;\n"\
            "\t\timage.height = data.img_height;\n"\
            "\t\timage.source = data.img_path;\n"\
        "\n"\
            "\t\timage.anchors.leftMargin = data.widget_width / 2 - image.width / 2;\n"\
            "\t\timage.anchors.topMargin = data.widget_height / 2 - image.height / 2;\n"\
        "\t}\n"\
        "\n"\
        "\tid: container\n"\
            "\t\tvisible: true\n"\
        "\n"\
        "\tImage {\n"\
            "\t\tid: texture\n"\
            "\t\tanchors.left: parent.left\n"\
            "\t\tanchors.top: parent.top\n"\
        "\n"\
            "\t\tvisible: true\n"\
        "\t}\n"\
    "}\n")
#endif //SITEST_TEMPLATES_H
