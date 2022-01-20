

#include "SIGRunLinkingManagerTest.hpp"

#include <sigrun/context/managers/LinkingManager.hpp>
#include <sigrun/context/managers/helpers/linking/Link.hpp>
#include <sigrun/plugin/Scripting.hpp>
#include <sigrun/plugin/PluginCollector.hpp>
#include <QtTest/QSignalSpy>
#include <sigrun/util/UUID.hpp>
#include <sigrun/context/Context.hpp>

TEST_F(SIGRunLinkingManagerTest, construction)
{
    EXPECT_NO_FATAL_FAILURE(LinkingManager lm);
}

//
// UNIDIRECTIONAL CASE
//

TEST_F(SIGRunLinkingManagerTest, link_creation_uni)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm.links().empty());

    ASSERT_TRUE(lm.links().size() == 3);
    ASSERT_TRUE(lm.links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm.links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm.links()[2]->sender_a()->uuid() == c->uuid());
}

TEST_F(SIGRunLinkingManagerTest, is_linked_uni)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);

    LinkingManager lm;

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm.add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));

    ASSERT_TRUE(lm.is_linked(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm.is_linked(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(c, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(b, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_FALSE(lm.is_linked(a, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
}

TEST_F(SIGRunLinkingManagerTest, link_event_execution_uni)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);
    std::shared_ptr<Region> d = std::make_shared<Region>(contour2, t, 1920, 1080);

    LinkingManager* lm = d_c.linking_manager();

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy4(d.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm->add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm->add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, d, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm->links().empty());

    ASSERT_TRUE(lm->links().size() == 4);
    ASSERT_TRUE(lm->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm->links()[2]->sender_a()->uuid() == c->uuid());
    ASSERT_TRUE(lm->links()[3]->sender_a()->uuid() == c->uuid());

    lm->emit_link_event(a, SI_CAPABILITY_LINK_POSITION);
}

TEST_F(SIGRunLinkingManagerTest, link_removal_uni)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);

    LinkingManager* lm = d_c.linking_manager();

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm->add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm->add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));

    ASSERT_FALSE(lm->links().empty());

    ASSERT_TRUE(lm->links().size() == 3);
    ASSERT_TRUE(lm->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm->links()[1]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm->links()[2]->sender_a()->uuid() == c->uuid());

    lm->emit_link_event(a, SI_CAPABILITY_LINK_POSITION);

    lm->remove_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD);

    ASSERT_TRUE(lm->links().size() == 2);

    ASSERT_FALSE(lm->links().empty());
    ASSERT_TRUE(lm->links()[1]->sender_a()->uuid() == c->uuid());

    lm->emit_link_event(a, SI_CAPABILITY_LINK_POSITION);

    lm->emit_link_event(c, SI_CAPABILITY_LINK_POSITION);
}

TEST_F(SIGRunLinkingManagerTest, link_creation_bi)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);

    LinkingManager* lm = d_c.linking_manager();

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm->add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));

    ASSERT_FALSE(lm->links().empty());
    ASSERT_EQ(lm->links().size(), 6);

    // test for a (birectional) lm.links[0] therefore depicts a->b AND lm.links[1] b->a
    ASSERT_TRUE(lm->links()[0]->sender_a()->uuid() == a->uuid());
    ASSERT_TRUE(lm->links()[1]->sender_a()->uuid() == b->uuid());
    // test for b birectional) lm.links[2] therefore depicts b->c AND lm.links[3] c->b
    ASSERT_TRUE(lm->links()[2]->sender_a()->uuid() == b->uuid());
    ASSERT_TRUE(lm->links()[3]->sender_a()->uuid() == c->uuid());
    // test for c birectional) lm.links[4] therefore depicts c->a AND lm.links[5] a->c
    ASSERT_TRUE(lm->links()[4]->sender_a()->uuid() == c->uuid());
    ASSERT_TRUE(lm->links()[5]->sender_a()->uuid() == a->uuid());
}

TEST_F(SIGRunLinkingManagerTest, is_linked_bi)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);

    LinkingManager* lm = d_c.linking_manager();

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm->add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));

    ASSERT_FALSE(lm->links().empty());
    ASSERT_EQ(lm->links().size(), 2); // bidirectional links are two unidirectional links internally

    ASSERT_TRUE(lm->is_linked(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->is_linked(b, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
}

TEST_F(SIGRunLinkingManagerTest, link_event_execution_bi)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);

    LinkingManager* lm = d_c.linking_manager();

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm->add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));

    ASSERT_TRUE(lm->links().size() == 6);

    ASSERT_EQ(lm->links()[4]->sender_a()->uuid(), c->uuid());

    lm->emit_link_event(a, SI_CAPABILITY_LINK_POSITION);
}

TEST_F(SIGRunLinkingManagerTest, link_removal_bi)
{
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX);
    std::string directory(buf);

    bp::import("sys").attr("path").attr("insert")(0, directory + "/tests/res/region");

    bp::object o = bp::import("Dummy1");
    o.attr(SI_INTERNAL_NAME) = "Dummy1";

    bp::object t = bp::import("Dummy2");
    t.attr(SI_INTERNAL_NAME) = "Dummy2";

    std::vector<glm::vec3> contour1{glm::vec3(100, 100, 1), glm::vec3(100, 600, 1), glm::vec3(600, 600, 1),
                                    glm::vec3(600, 100, 1)};
    std::vector<glm::vec3> contour2{glm::vec3(150, 150, 1), glm::vec3(150, 550, 1), glm::vec3(550, 550, 1),
                                    glm::vec3(550, 150, 1)};

    std::shared_ptr<Region> a = std::make_shared<Region>(contour1, o, 1920, 1080);
    std::shared_ptr<Region> b = std::make_shared<Region>(contour2, t, 1920, 1080);
    std::shared_ptr<Region> c = std::make_shared<Region>(contour2, o, 1920, 1080);
    std::shared_ptr<Region> d = std::make_shared<Region>(contour2, t, 1920, 1080);


    LinkingManager* lm = d_c.linking_manager();

    QSignalSpy spy1(a.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy2(b.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy3(c.get(), &Region::LINK_SIGNAL);
    QSignalSpy spy4(d.get(), &Region::LINK_SIGNAL);

    ASSERT_TRUE(lm->add_link(a, SI_CAPABILITY_LINK_POSITION, b, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, a, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD));
    ASSERT_TRUE(lm->add_link(c, SI_CAPABILITY_LINK_POSITION, d, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::UD));

    ASSERT_TRUE(lm->links().size() == 7);

    ASSERT_EQ(lm->links()[0]->sender_a()->uuid(), a->uuid());
    ASSERT_EQ(lm->links()[5]->sender_a()->uuid(), a->uuid());

    lm->emit_link_event(a, SI_CAPABILITY_LINK_POSITION);

    lm->remove_link(b, SI_CAPABILITY_LINK_POSITION, c, SI_CAPABILITY_LINK_POSITION, ILink::LINK_TYPE::BD);

    ASSERT_EQ(lm->links().size(), 5);

    ASSERT_EQ(lm->links()[0]->sender_a()->uuid(), a->uuid());
    ASSERT_EQ(lm->links()[0]->receiver_b()->uuid(), b->uuid());

    ASSERT_EQ(lm->links()[1]->sender_a()->uuid(), b->uuid());
    ASSERT_EQ(lm->links()[1]->receiver_b()->uuid(), a->uuid());

    ASSERT_EQ(lm->links()[2]->sender_a()->uuid(), c->uuid());
    ASSERT_EQ(lm->links()[2]->receiver_b()->uuid(), a->uuid());

    ASSERT_EQ(lm->links()[3]->sender_a()->uuid(), a->uuid());
    ASSERT_EQ(lm->links()[3]->receiver_b()->uuid(), c->uuid());

    lm->emit_link_event(a, SI_CAPABILITY_LINK_POSITION);


    lm->emit_link_event(b, SI_CAPABILITY_LINK_POSITION);


    lm->emit_link_event(c, SI_CAPABILITY_LINK_POSITION);

}