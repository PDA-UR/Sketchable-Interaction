

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

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.linking_graph()->links().empty());
    ASSERT_FALSE(lm.linking_graph()->links()[0]->children().empty());
    ASSERT_FALSE(lm.linking_graph()->links()[0]->children()[0]->children().empty());

    ASSERT_TRUE(lm.linking_graph()->links().size() == 3);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[2]->sender_a()->uuid() == c->uuid());

    ASSERT_TRUE(lm.linking_graph()->links()[0]->children().size() == 1);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->sender_a()->uuid() == b->uuid());

    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->children().size() == 1);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->children()[0]->sender_a()->uuid() == c->uuid());
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

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_TRUE(lm.is_linked(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.is_linked(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(c, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(b, "__position__", a, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(a, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
}

TEST_F(SIGRunLinkingManagerTest, link_event_execution_uni)
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

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.linking_graph()->links().empty());
    ASSERT_FALSE(lm.linking_graph()->links()[0]->children().empty());
    ASSERT_FALSE(lm.linking_graph()->links()[0]->children()[0]->children().empty());

    ASSERT_TRUE(lm.linking_graph()->links().size() == 3);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[2]->sender_a()->uuid() == c->uuid());

    ASSERT_TRUE(lm.linking_graph()->links()[0]->children().size() == 1);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->sender_a()->uuid() == b->uuid());

    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->children().size() == 1);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->children()[0]->sender_a()->uuid() == c->uuid());

    lm.linking_graph()->emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 1);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);
}

TEST_F(SIGRunLinkingManagerTest, link_removal_uni)
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

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.linking_graph()->links().empty());
    ASSERT_FALSE(lm.linking_graph()->links()[0]->children().empty());
    ASSERT_FALSE(lm.linking_graph()->links()[0]->children()[0]->children().empty());

    ASSERT_TRUE(lm.linking_graph()->links().size() == 3);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[2]->sender_a()->uuid() == c->uuid());

    ASSERT_TRUE(lm.linking_graph()->links()[0]->children().size() == 1);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->sender_a()->uuid() == b->uuid());

    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->children().size() == 1);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children()[0]->children()[0]->sender_a()->uuid() == c->uuid());

    lm.linking_graph()->emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 1);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);

    lm.remove_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD);

    ASSERT_TRUE(lm.linking_graph()->links().size() == 2);

    ASSERT_FALSE(lm.linking_graph()->links().empty());
    ASSERT_TRUE(lm.linking_graph()->links()[0]->children().empty());
    ASSERT_TRUE(lm.linking_graph()->links()[1]->sender_a()->uuid() == c->uuid());
    ASSERT_FALSE(lm.linking_graph()->links()[1]->children().empty());
    lm.linking_graph()->emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 2);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);

    lm.linking_graph()->emit_link_event(c, "__position__");

    ASSERT_EQ(spy1.count(), 3);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 2);
}

