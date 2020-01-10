

#include "SIGRunLinkingManagerTest.hpp"

#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/Link.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <QtTest/QSignalSpy>
#include <sigrun/util/UUID.hpp>

TEST_F(SIGRunLinkingManagerTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(LinkingManager lm);
}

//
// UNIDIRECTIONAL CASE
//
TEST_F(SIGRunLinkingManagerTest, link_creation_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    ASSERT_EQ(lm.links().size(), 1);
}

TEST_F(SIGRunLinkingManagerTest, is_linked_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    ASSERT_EQ(lm.links().size(), 1);

    ASSERT_TRUE(lm.is_linked(a->uuid(), b->uuid(), "position", "position", ILink::LINK_TYPE::UD));
}

TEST_F(SIGRunLinkingManagerTest, link_retrieval_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    ASSERT_EQ(lm.links().size(), 1);

    const auto l = lm.link(a->uuid(), b->uuid(), "position", "position", ILink::LINK_TYPE::UD);
    ASSERT_NE(l, nullptr);
    ASSERT_EQ(l->sender_a(), a->uuid());
    ASSERT_EQ(l->receiver_b(), b->uuid());
    ASSERT_EQ(l->attribute_a(), "position");
    ASSERT_EQ(l->attribute_b(), "position");

    ASSERT_EQ(l->type(), ILink::LINK_TYPE::UD);
}

TEST_F(SIGRunLinkingManagerTest, add_capability_functions_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
}

TEST_F(SIGRunLinkingManagerTest, is_capability_supported_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "scale", "position", ILink::LINK_TYPE::UD));

    ASSERT_TRUE(lm.is_capability_supported("position"));
    ASSERT_FALSE(lm.is_capability_supported("scale"));
    ASSERT_EQ(lm.links().size(), 1);
}

TEST_F(SIGRunLinkingManagerTest, capability_signal_function_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.is_capability_supported("position"));
    ASSERT_EQ(lm.capability_signal_function("position"), "__position__(int,int,const std::string&)");
}

TEST_F(SIGRunLinkingManagerTest, capability_slot_function_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.is_capability_supported("position"));
    ASSERT_EQ(lm.capability_slot_function("position"), "__set_position__(int,int,const std::string&)");
}

TEST_F(SIGRunLinkingManagerTest, connection_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));

//    Q_EMIT a->__position__(1, 1, UUID::uuid());

    lm.emit_link_event(a.get(), "__position__", UUID::uuid(), Q_ARG(int, 1), Q_ARG(int, 1));

    ASSERT_EQ(spy.count(), 1);

    ASSERT_EQ(a->__x__, 0);
    ASSERT_EQ(a->__y__, 0);
    ASSERT_EQ(b->__x__, 1);
    ASSERT_EQ(b->__y__, 1);
}

TEST_F(SIGRunLinkingManagerTest, disconnection_uni)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    Q_EMIT a->__position__(1, 1, UUID::uuid());
    ASSERT_EQ(spy.count(), 1);
    ASSERT_EQ(a->__x__, 0);
    ASSERT_EQ(a->__y__, 0);
    ASSERT_EQ(b->__x__, 1);
    ASSERT_EQ(b->__y__, 1);

    EXPECT_NO_FATAL_FAILURE(lm.remove_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
    Q_EMIT a->__position__(1, 1, UUID::uuid());
    ASSERT_EQ(spy.count(), 2);
    ASSERT_EQ(a->__x__, 0);
    ASSERT_EQ(a->__y__, 0);
    ASSERT_EQ(b->__x__, 1);
    ASSERT_EQ(b->__y__, 1);
    ASSERT_EQ(lm.links().size(), 0);
}

//
// BIDIRECTIONAL CASE
//
TEST_F(SIGRunLinkingManagerTest, link_creation_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
//
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));
    ASSERT_EQ(lm.links().size(), 1);
}

TEST_F(SIGRunLinkingManagerTest, is_linked_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));
    ASSERT_EQ(lm.links().size(), 1);

    ASSERT_TRUE(lm.is_linked(a->uuid(), b->uuid(), "position", "position", ILink::LINK_TYPE::BD));
}

TEST_F(SIGRunLinkingManagerTest, link_retrieval_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));
    ASSERT_EQ(lm.links().size(), 1);

    const auto l = lm.link(a->uuid(), b->uuid(), "position", "position", ILink::LINK_TYPE::BD);
    ASSERT_NE(l, nullptr);
    ASSERT_EQ(l->sender_a(), a->uuid());
    ASSERT_EQ(l->receiver_b(), b->uuid());
    ASSERT_EQ(l->attribute_a(), "position");
    ASSERT_EQ(l->attribute_b(), "position");

    ASSERT_EQ(l->type(), ILink::LINK_TYPE::BD);
}

TEST_F(SIGRunLinkingManagerTest, add_capability_functions_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
}

TEST_F(SIGRunLinkingManagerTest, is_capability_supported_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));

    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "scale", "position", ILink::LINK_TYPE::BD));

    ASSERT_TRUE(lm.is_capability_supported("position"));
    ASSERT_FALSE(lm.is_capability_supported("scale"));
    ASSERT_EQ(lm.links().size(), 1);
}

TEST_F(SIGRunLinkingManagerTest, capability_signal_function_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.is_capability_supported("position"));
    ASSERT_EQ(lm.capability_signal_function("position"), "__position__(int,int,const std::string&)");
}

TEST_F(SIGRunLinkingManagerTest, capability_slot_function_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.is_capability_supported("position"));
    ASSERT_EQ(lm.capability_slot_function("position"), "__set_position__(int,int,const std::string&)");
}

TEST_F(SIGRunLinkingManagerTest, connection_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
    QSignalSpy spy1(a.get(), SIGNAL(__position__(int, int, const std::string&)));
    QSignalSpy spy2(b.get(), SIGNAL(__position__(int, int, const std::string&)));

    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::BD));

    Q_EMIT a->__position__(1, 1, UUID::uuid());

    ASSERT_EQ(spy1.count(), 2); // will be terminated at 2nd visit B instead of 2nd visit A (source)
    ASSERT_EQ(spy2.count(), 1);

    ASSERT_EQ(a->__x__, 1);
    ASSERT_EQ(a->__y__, 1);
    ASSERT_EQ(b->__x__, 1);
    ASSERT_EQ(b->__y__, 1);
}

TEST_F(SIGRunLinkingManagerTest, disconnection_bi)
{
    std::string path = "res/region";

    std::vector<std::string> files, classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    std::string base_filename = files[0].substr(files[0].find_last_of("/\\") + 1);
    std::string module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    std::string rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[0]);

    std::shared_ptr<bp::object> o = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    classes.clear();

    base_filename = files[1].substr(files[1].find_last_of("/\\") + 1);
    module_name = base_filename.substr(0, base_filename.find_last_of('.'));
    rpath = path + "/" + base_filename;

    script.load_class_names(classes, files[1]);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1 {glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1), glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2 {glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1), glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);

    LinkingManager lm;
//    QSignalSpy spy(a.get(), SIGNAL(__position__(int, int)));
//
//    EXPECT_NO_FATAL_FAILURE(lm.add_capability_functions("position", "__position__(int,int,const std::string&)", "__set_position__(int,int,const std::string&)"));
//
//    EXPECT_NO_FATAL_FAILURE(lm.add_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
//    Q_EMIT a->__position__(1, 1);
//    ASSERT_EQ(spy.count(), 1);
//    ASSERT_EQ(a->__x__, 0);
//    ASSERT_EQ(a->__y__, 0);
//    ASSERT_EQ(b->__x__, 1);
//    ASSERT_EQ(b->__y__, 1);
//
//    EXPECT_NO_FATAL_FAILURE(lm.remove_link(a, b, "position", "position", ILink::LINK_TYPE::UD));
//    Q_EMIT a->__position__(1, 1);
//    ASSERT_EQ(spy.count(), 2);
//    ASSERT_EQ(a->__x__, 0);
//    ASSERT_EQ(a->__y__, 0);
//    ASSERT_EQ(b->__x__, 1);
//    ASSERT_EQ(b->__y__, 1);
//    ASSERT_EQ(lm.links().size(), 0);
}