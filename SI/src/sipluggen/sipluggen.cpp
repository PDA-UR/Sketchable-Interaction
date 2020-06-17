
#include "templates.h"
#include <boost/filesystem.hpp>
#include <iostream>

namespace fs = boost::filesystem;

int main(int argc, char** argv)
{
    if(argc < 3)
    {
        if(argc == 2)
        {
            if(std::string(argv[1]) == "--help")
            {
                std::string msg = "sipluggen is used to generate a SI-plugin (python-file) and its associated qml file with a simple CLI-tool."
                                  "";

                std::cout << msg << std::endl;
                std::cout << "Usage: sipluggen (-std) [-p <desired path to plugin>, ...]!" << std::endl;

                return 0;
            }
        }

        std::cout << "Use sipluggen --help for more detailed information!" << std::endl;
        std::cout << "Usage: sipluggen (-std) [-p <desired path to plugin>, ...]!" << std::endl;
        return 1;
    }

    int32_t start_index = 1;
    std::string std_lib = "";

    if(!(argc & 1))
    {
        if(std::string(argv[1]) != "-std")
        {
            std::cout << "Usage: sipluggen (-std) [-p <desired path to plugin>, ...]" << std::endl;
            return 1;
        }

        std_lib = "standard_environment_library/";
        start_index = 2;
    }
    else
    {
        if(argc == 3)
        {
            if(std::string(argv[1]) != "-p")
            {
                std::cout << "Usage: sipluggen (-std) [-p <desired path to plugin>, ...]" << std::endl;
                return 1;
            }
        }
    }

    for(int32_t i = start_index; i < argc; i += 2)
    {
        std::string folder = fs::current_path().string() + "/plugins/" + std_lib;

        if(std::string(argv[i]) != "-p")
        {
            std::cout << "Usage: sipluggen (-std) [-p <desired path to plugin>, ...]" << std::endl;
            return 1;
        }

        folder += std::string(argv[i + 1]).substr(0, std::string(argv[i + 1]).find_last_of('/'));
        std::string target_file = std::string(argv[i + 1]).substr(std::string(argv[i + 1]).find_last_of('/') + 1, std::string::npos);

        if(target_file.find('.') != std::string::npos)
            if(target_file.substr(target_file.find_last_of('.'), std::string::npos) == ".py")
                target_file = target_file.substr(0, target_file.find_last_of('.'));

        fs::create_directories(folder);
        fs::create_directories(folder + "/res");

        fs::ofstream plugin_file (folder + "/" + target_file + ".py");
        std::string content = SI_PLUGIN(target_file, "plugins/" + std_lib + std::string(argv[i + 1]).substr(0, std::string(argv[i + 1]).find_last_of('/')));
        plugin_file.write(content.c_str(), content.size());
        plugin_file.close();

        fs::ofstream qml_file (folder + "/" + target_file + ".qml");
        content = SIQML;
        qml_file.write(content.c_str(), content.size());
        qml_file.close();
    }

    return 0;
}
