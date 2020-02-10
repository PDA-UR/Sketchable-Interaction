

#include "SIGRunLinkingManagerTest.hpp"

#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/managers/helpers/linking/Link.hpp>
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

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.linking_graph()->links().empty());

    ASSERT_TRUE(lm.linking_graph()->links().size() == 3);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[2]->sender_a()->uuid() == c->uuid());
}

TEST_F(SIGRunLinkingManagerTest, is_linked_uni)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);

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

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);
    std::shared_ptr<Region> d = std::make_shared<Region>(contour2, *t, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy4(d.get(), &Region::LINK_SIGNAL);

    a->set_name("A");
    b->set_name("B");
    c->set_name("C");
    d->set_name("D");

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", d, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.links().empty());

    ASSERT_TRUE(lm.links().size() == 4);
    ASSERT_TRUE(lm.links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.links()[2]->sender_a()->uuid() == c->uuid());
    ASSERT_TRUE(lm.links()[3]->sender_a()->uuid() == c->uuid());

    lm.emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 1);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);
    ASSERT_EQ(spy4.count(), 1);
}

TEST_F(SIGRunLinkingManagerTest, link_removal_uni)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    a->set_name("a");
    b->set_name("b");
    c->set_name("c");

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.linking_graph()->links().empty());

    ASSERT_TRUE(lm.linking_graph()->links().size() == 3);
    ASSERT_TRUE(lm.linking_graph()->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.linking_graph()->links()[2]->sender_a()->uuid() == c->uuid());

    lm.emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 1);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);

    lm.remove_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::UD);

    ASSERT_TRUE(lm.links().size() == 2);

    ASSERT_FALSE(lm.links().empty());
    ASSERT_TRUE(lm.links()[1]->sender_a()->uuid() == c->uuid());

    lm.emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 2);
    ASSERT_EQ(spy2.count(), 2);
    ASSERT_EQ(spy3.count(), 1);

    lm.emit_link_event(c, "__position__");

    ASSERT_EQ(spy1.count(), 3);
    ASSERT_EQ(spy2.count(), 3);
    ASSERT_EQ(spy3.count(), 2);
}

TEST_F(SIGRunLinkingManagerTest, link_creation_bi)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::BD));

    ASSERT_FALSE(lm.links().empty());
    ASSERT_EQ(lm.links().size(), 6);

    // test for a (birectional) lm.links[0] therefore depicts a->b AND lm.links[1] b->a
    ASSERT_TRUE(lm.links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.links()[1]->sender_a()->uuid() == b->uuid());
    // test for b birectional) lm.links[2] therefore depicts b->c AND lm.links[3] c->b
    ASSERT_TRUE(lm.links()[2]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.links()[3]->sender_a()->uuid() == c->uuid());
    // test for c birectional) lm.links[4] therefore depicts c->a AND lm.links[5] a->c
    ASSERT_TRUE(lm.links()[4]->sender_a()->uuid() == c->uuid());
    ASSERT_TRUE(lm.links()[5]->sender_a()->uuid() == a->uuid());
}

TEST_F(SIGRunLinkingManagerTest, is_linked_bi)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::BD));

    ASSERT_FALSE(lm.links().empty());
    ASSERT_EQ(lm.links().size(), 2); // bidirectional links are two unidirectional links internally

    ASSERT_TRUE(lm.is_linked(a, "__position__", b, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.is_linked(b, "__position__", a, "__position__", ILink::LINK_TYPE::BD));
}

TEST_F(SIGRunLinkingManagerTest, link_event_execution_bi)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);

    a->set_name("a");
    b->set_name("b");
    c->set_name("c");

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::BD));

    ASSERT_TRUE(lm.links().size() == 6);

    ASSERT_EQ(lm.links()[4]->sender_a()->uuid(), c->uuid());

    lm.emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 1);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);
}

TEST_F(SIGRunLinkingManagerTest, link_removal_bi)
{
    std::string path = "res/region";

    std::vector<std::tuple<std::string, std::string>> files;
    std::vector<std::string> classes;

    PluginCollector().collect("/" + path, files);
    Scripting script;

    const std::string& full_path = std::get<0>(files[0]);
    const std::string& name = std::get<1>(files[0]);

    std::string module_name = name.substr(0, name.find_last_of('.'));
    std::string rpath = full_path.substr(full_path.find(path)) + "/" + name;

    script.load_class_names(classes, rpath);

    bp::object o = script.si_plugin(module_name, rpath, classes[0]);

    classes.clear();

    const std::string& full_path2 = std::get<0>(files[1]);
    const std::string& name2 = std::get<1>(files[1]);

    module_name = name2.substr(0, name2.find_last_of('.'));
    rpath = full_path2.substr(full_path2.find(path)) + "/" + name2;

    script.load_class_names(classes, rpath);

    std::shared_ptr<bp::object> t = std::make_shared<bp::object>(script.si_plugin(module_name, rpath, classes[0]));

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, *o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, *t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, *o, 1920, 1080);
    std::shared_ptr<Region> d = std::make_shared<Region>(contour2, *t, 1920, 1080);


    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy4(d.get(), &Region::LINK_SIGNAL);

    a->set_name("A");
    b->set_name("B");
    c->set_name("C");
    d->set_name("D");

    ASSERT_TRUE(lm.add_link(a, "__position__", b, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(c, "__position__", a, "__position__", ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm.add_link(c, "__position__", d, "__position__", ILink::LINK_TYPE::UD));

    ASSERT_TRUE(lm.links().size() == 7);

    ASSERT_EQ(lm.links()[0]->sender_a()->uuid(), a->uuid());
    ASSERT_EQ(lm.links()[5]->sender_a()->uuid(), a->uuid());

    lm.emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 1);
    ASSERT_EQ(spy2.count(), 1);
    ASSERT_EQ(spy3.count(), 1);
    ASSERT_EQ(spy4.count(), 1);

    lm.remove_link(b, "__position__", c, "__position__", ILink::LINK_TYPE::BD);

    ASSERT_EQ(lm.links().size(), 5);

    ASSERT_EQ(lm.links()[0]->sender_a()->uuid(), a->uuid());
    ASSERT_EQ(lm.links()[0]->receiver_b()->uuid(), b->uuid());

    ASSERT_EQ(lm.links()[1]->sender_a()->uuid(), b->uuid());
    ASSERT_EQ(lm.links()[1]->receiver_b()->uuid(), a->uuid());

    ASSERT_EQ(lm.links()[2]->sender_a()->uuid(), c->uuid());
    ASSERT_EQ(lm.links()[2]->receiver_b()->uuid(), a->uuid());

    ASSERT_EQ(lm.links()[3]->sender_a()->uuid(), a->uuid());
    ASSERT_EQ(lm.links()[3]->receiver_b()->uuid(), c->uuid());

    lm.emit_link_event(a, "__position__");

    ASSERT_EQ(spy1.count(), 2);
    ASSERT_EQ(spy2.count(), 2);
    ASSERT_EQ(spy3.count(), 2);
    ASSERT_EQ(spy4.count(), 2);

    lm.emit_link_event(b, "__position__");

    ASSERT_EQ(spy1.count(), 3);
    ASSERT_EQ(spy2.count(), 3);
    ASSERT_EQ(spy3.count(), 3);
    ASSERT_EQ(spy4.count(), 3);

    lm.emit_link_event(c, "__position__");

    ASSERT_EQ(spy1.count(), 4);
    ASSERT_EQ(spy2.count(), 4);
    ASSERT_EQ(spy3.count(), 4);
    ASSERT_EQ(spy4.count(), 4);
}