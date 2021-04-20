

#include "SIGRunLinkTest.hpp"
#include <sigrun/context/managers/helpers/linking/Link.hpp>
#include <sigrun/context/managers/helpers/input/ExternalObject.hpp>
#include <sigrun/region/Region.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>

TEST_F(SIGRunLinkTest, region_region_unidirectional_construction)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    ASSERT_NO_FATAL_FAILURE(UnidirectionalLink udl(a, b, "test", "test"));
}

TEST_F(SIGRunLinkTest, external_object_region_unidirectional_construction)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    std::shared_ptr<ExternalObject> eoa = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::APPLICATION);
    std::shared_ptr<ExternalObject> eof = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::FINGER);
    std::shared_ptr<ExternalObject> eot = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::TANGIBLE);

    ASSERT_NO_FATAL_FAILURE(UnidirectionalLink udl(eom, a, "test", "test"));
    ASSERT_NO_FATAL_FAILURE(UnidirectionalLink udl(eoa, a, "test", "test"));
    ASSERT_NO_FATAL_FAILURE(UnidirectionalLink udl(eof, a, "test", "test"));
    ASSERT_NO_FATAL_FAILURE(UnidirectionalLink udl(eot, a, "test", "test"));
}

TEST_F(SIGRunLinkTest, type)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.type(), ILink::LINK_TYPE::UD);

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.type(), ILink::LINK_TYPE::UD);
}

TEST_F(SIGRunLinkTest, sender_a)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.sender_a()->uuid(), a->uuid());

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.sender_a(), nullptr);
}

TEST_F(SIGRunLinkTest, sender_b)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.sender_b()->uuid(), b->uuid());

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.sender_b(), nullptr);
}

TEST_F(SIGRunLinkTest, receiver_a)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.receiver_a()->uuid(), a->uuid());

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.receiver_a(), nullptr);
}

TEST_F(SIGRunLinkTest, receiver_b)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.receiver_b()->uuid(), b->uuid());

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.receiver_b()->uuid(), a->uuid());
}

TEST_F(SIGRunLinkTest, external_sender_a)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.external_sender_a(), nullptr);

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.external_sender_a()->uuid(), eom->uuid());
}

TEST_F(SIGRunLinkTest, attribute_a)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.attribute_a(), "test");

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.attribute_a(), "test");
}

TEST_F(SIGRunLinkTest, attribute_b)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_EQ(udl.attribute_b(), "test");

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_EQ(udl2.attribute_b(), "test");
}

TEST_F(SIGRunLinkTest, is_external)
{
    std::vector<glm::vec3> contour {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};

    std::string path = "tests/res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath));

    std::shared_ptr<Region> a = std::make_shared<Region>(contour, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour, *o, 1920, 1080);

    UnidirectionalLink udl(a, b, "test", "test");

    ASSERT_FALSE(udl.is_external());

    std::shared_ptr<ExternalObject> eom = std::make_shared<ExternalObject>(ExternalObject::ExternalObjectType::MOUSE);
    UnidirectionalLink udl2(eom, a, "test", "test");

    ASSERT_TRUE(udl2.is_external());
}

// Bidirectional Case is modelled by using two unidirectional links instead of one bidirectional link
// conceptually there is no difference between two unidirectional links and one bidirectional link
