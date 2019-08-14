

#ifndef SI_SIPLUGINTEST_HPP
#define SI_SIPLUGINTEST_HPP

#include <boost/python.hpp>

namespace bp = boost::python;


#include <gtest/gtest.h>

class SIPluginTest : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        const std::string rel_path = "tests/res";

        SI::Scripting script;
        std::vector<std::string> files, classes;

        SI::PluginCollector::collect("/" + rel_path, files);

        for(auto& path : files)
        {
            std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
            std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
            std::string rpath = rel_path + "/" + base_filename;

            script.load_class_names(classes, path);

            for(auto& ref : classes)
            {
                d_py_object =  script.si_plugin(module_name, rpath, ref);
                break;
            }
        }
    }

    virtual void TearDown()
    {

    }

    boost::python::object d_py_object;
};

#endif //SI_SIPLUGINTEST_HPP
