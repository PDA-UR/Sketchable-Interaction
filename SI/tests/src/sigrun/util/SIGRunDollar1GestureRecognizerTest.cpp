
#include <glm/glm.hpp>

#include "SIGRunDollar1GestureRecognizerTest.hpp"
#include <sigrun/util/Dollar1GestureRecognizer.hpp>

TEST_F(SIGRunDollar1GestureRecognizerTest, recognize_triangle)
{
//    std::vector<glm::vec3> vec_of_vec3
//    {
//        glm::vec3(712.000000, 693.000000, 1),
//        glm::vec3(714.000000, 689.000000, 1),
//        glm::vec3(718.000000, 681.000000, 1),
//        glm::vec3(724.000000, 668.000000, 1),
//        glm::vec3(731.000000, 657.000000, 1),
//        glm::vec3(737.000000, 647.000000, 1),
//        glm::vec3(744.000000, 635.000000, 1),
//        glm::vec3(752.000000, 621.000000, 1),
//        glm::vec3(761.000000, 607.000000, 1),
//        glm::vec3(768.000000, 593.000000, 1),
//        glm::vec3(773.000000, 587.000000, 1),
//        glm::vec3(778.000000, 579.000000, 1),
//        glm::vec3(785.000000, 567.000000, 1),
//        glm::vec3(793.000000, 554.000000, 1),
//        glm::vec3(801.000000, 542.000000, 1),
//        glm::vec3(805.000000, 535.000000, 1),
//        glm::vec3(807.000000, 530.000000, 1),
//        glm::vec3(809.000000, 526.000000, 1),
//        glm::vec3(810.000000, 524.000000, 1),
//        glm::vec3(813.000000, 528.000000, 1),
//        glm::vec3(816.000000, 533.000000, 1),
//        glm::vec3(822.000000, 546.000000, 1),
//        glm::vec3(829.000000, 561.000000, 1),
//        glm::vec3(838.000000, 583.000000, 1),
//        glm::vec3(857.000000, 611.000000, 1),
//        glm::vec3(869.000000, 635.000000, 1),
//        glm::vec3(880.000000, 656.000000, 1),
//        glm::vec3(890.000000, 674.000000, 1),
//        glm::vec3(895.000000, 685.000000, 1),
//        glm::vec3(898.000000, 691.000000, 1),
//        glm::vec3(899.000000, 693.000000, 1),
//        glm::vec3(897.000000, 693.000000, 1),
//        glm::vec3(896.000000, 694.000000, 1),
//        glm::vec3(849.000000, 694.000000, 1),
//        glm::vec3(836.000000, 692.000000, 1),
//        glm::vec3(827.000000, 691.000000, 1),
//        glm::vec3(804.000000, 691.000000, 1),
//        glm::vec3(802.000000, 692.000000, 1),
//        glm::vec3(792.000000, 692.000000, 1),
//        glm::vec3(791.000000, 693.000000, 1),
//        glm::vec3(745.000000, 693.000000, 1),
//        glm::vec3(745.000000, 692.000000, 1),
//        glm::vec3(712.000000, 693.000000, 1),
//    }, result;
//
//    Recognizer r;
//
//    ASSERT_GT(r.recognize(result, vec_of_vec3), 0.8f);
//
//    std::vector<glm::vec3> expected
//    {
//        glm::vec3(712.000000, 524.000000, 1),
//        glm::vec3(899.000000, 524.000000, 1),
//        glm::vec3(805.5, 778.556824, 1)
//    };
//
//    ASSERT_EQ(expected.size(), result.size());
//
//    for(int32_t i = 0; i < expected.size(); ++i)
//    {
//        ASSERT_NEAR(expected[i].x, result[i].x, 1.0);
//        ASSERT_NEAR(expected[i].y, result[i].y, 1.0);
//    }
}

TEST_F(SIGRunDollar1GestureRecognizerTest, recognize_rect)
{
    std::vector<glm::vec3> result;
    Recognizer r;

    // clockwise start tlc
    std::vector<glm::vec3> rect
    {
            glm::vec3(608.000000, 558.000000, 1),
            glm::vec3(623.000000, 555.000000, 1),
            glm::vec3(651.000000, 549.000000, 1),
            glm::vec3(700.000000, 544.000000, 1),
            glm::vec3(774.000000, 540.000000, 1),
            glm::vec3(868.000000, 535.000000, 1),
            glm::vec3(950.000000, 530.000000, 1),
            glm::vec3(1038.000000, 527.000000, 1),
            glm::vec3(1104.000000, 525.000000, 1),
            glm::vec3(1139.000000, 523.000000, 1),
            glm::vec3(1146.000000, 521.000000, 1),
            glm::vec3(1148.000000, 521.000000, 1),
            glm::vec3(1148.000000, 521.000000, 1),
            glm::vec3(1149.000000, 521.000000, 1),
            glm::vec3(1150.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1151.000000, 521.000000, 1),
            glm::vec3(1152.000000, 521.000000, 1),
            glm::vec3(1152.000000, 521.000000, 1),
            glm::vec3(1153.000000, 524.000000, 1),
            glm::vec3(1156.000000, 538.000000, 1),
            glm::vec3(1167.000000, 578.000000, 1),
            glm::vec3(1181.000000, 648.000000, 1),
            glm::vec3(1195.000000, 701.000000, 1),
            glm::vec3(1203.000000, 734.000000, 1),
            glm::vec3(1207.000000, 751.000000, 1),
            glm::vec3(1211.000000, 767.000000, 1),
            glm::vec3(1212.000000, 778.000000, 1),
            glm::vec3(1215.000000, 792.000000, 1),
            glm::vec3(1218.000000, 806.000000, 1),
            glm::vec3(1219.000000, 809.000000, 1),
            glm::vec3(1219.000000, 811.000000, 1),
            glm::vec3(1220.000000, 813.000000, 1),
            glm::vec3(1220.000000, 814.000000, 1),
            glm::vec3(1220.000000, 815.000000, 1),
            glm::vec3(1220.000000, 816.000000, 1),
            glm::vec3(1216.000000, 817.000000, 1),
            glm::vec3(1209.000000, 819.000000, 1),
            glm::vec3(1179.000000, 822.000000, 1),
            glm::vec3(1132.000000, 829.000000, 1),
            glm::vec3(1039.000000, 834.000000, 1),
            glm::vec3(975.000000, 836.000000, 1),
            glm::vec3(898.000000, 841.000000, 1),
            glm::vec3(818.000000, 849.000000, 1),
            glm::vec3(762.000000, 856.000000, 1),
            glm::vec3(696.000000, 858.000000, 1),
            glm::vec3(640.000000, 861.000000, 1),
            glm::vec3(612.000000, 863.000000, 1),
            glm::vec3(602.000000, 864.000000, 1),
            glm::vec3(601.000000, 865.000000, 1)
    };

    std::vector<glm::vec3> expected
    {
        glm::vec3(601.000000, 521.000000, 1),
        glm::vec3(601.000000, 865.000000, 1),
        glm::vec3(1220.000000, 865.000000, 1),
        glm::vec3(1220.000000, 521.000000, 1)
    };

    r.recognize(result, rect);

    ASSERT_EQ(expected.size(), result.size());

    Print::print(result);
    Print::print(expected);

    for(int i = 0; i < result.size(); ++i)
    {
        ASSERT_EQ(expected[i].x, result[i].x);
        ASSERT_EQ(expected[i].y, result[i].y);
    }

    result.clear();
}

TEST_F(SIGRunDollar1GestureRecognizerTest, recognize_diamond)
{
    //clockwise
    std::vector<glm::vec3> perfect_diamond
    {
        {100, 100, 1},
        {300, 0, 1},
        {500, 100, 1},
        {300, 200, 10}
    }, result;

    Recognizer r;

    r.recognize(result, perfect_diamond);

    ASSERT_EQ(result.size(), perfect_diamond.size());

//    for(int i = 0; i < result.size(); ++i)
//    {
//        ASSERT_EQ(perfect_diamond[i].x, result[i].x);
//        ASSERT_EQ(perfect_diamond[i].y, result[i].y);
//    }
}

TEST_F(SIGRunDollar1GestureRecognizerTest, recognize_horizontal_ellipsoid)
{

}

TEST_F(SIGRunDollar1GestureRecognizerTest, recognize_vertical_ellipsoid)
{

}

TEST_F(SIGRunDollar1GestureRecognizerTest, recognize_circle)
{
    Recognizer r;

    // clockwise
    std::vector<glm::vec3> circle
    {
        glm::vec3(712.000000, 537.000000, 1), glm::vec3(712.000000, 537.000000, 1), glm::vec3(712.000000, 537.000000, 1), glm::vec3(712.000000, 537.000000, 1), glm::vec3(712.000000, 537.000000, 1),
        glm::vec3(712.000000, 537.000000, 1), glm::vec3(712.000000, 537.000000, 1), glm::vec3(713.000000, 536.000000, 1), glm::vec3(715.000000, 536.000000, 1), glm::vec3(720.000000, 536.000000, 1),
        glm::vec3(728.000000, 535.000000, 1), glm::vec3(741.000000, 536.000000, 1), glm::vec3(742.000000, 536.000000, 1), glm::vec3(744.000000, 537.000000, 1), glm::vec3(745.000000, 537.000000, 1),
        glm::vec3(748.000000, 539.000000, 1), glm::vec3(753.000000, 543.000000, 1), glm::vec3(755.000000, 545.000000, 1), glm::vec3(758.000000, 547.000000, 1), glm::vec3(760.000000, 549.000000, 1),
        glm::vec3(763.000000, 554.000000, 1), glm::vec3(766.000000, 560.000000, 1), glm::vec3(767.000000, 562.000000, 1), glm::vec3(769.000000, 567.000000, 1), glm::vec3(771.000000, 572.000000, 1),
        glm::vec3(772.000000, 578.000000, 1), glm::vec3(773.000000, 584.000000, 1), glm::vec3(773.000000, 589.000000, 1), glm::vec3(773.000000, 593.000000, 1), glm::vec3(774.000000, 600.000000, 1),
        glm::vec3(774.000000, 606.000000, 1), glm::vec3(774.000000, 611.000000, 1), glm::vec3(774.000000, 616.000000, 1), glm::vec3(774.000000, 619.000000, 1), glm::vec3(772.000000, 623.000000, 1),
        glm::vec3(772.000000, 624.000000, 1), glm::vec3(770.000000, 629.000000, 1), glm::vec3(769.000000, 630.000000, 1), glm::vec3(766.000000, 635.000000, 1), glm::vec3(764.000000, 637.000000, 1),
        glm::vec3(760.000000, 639.000000, 1), glm::vec3(758.000000, 643.000000, 1), glm::vec3(755.000000, 646.000000, 1), glm::vec3(753.000000, 647.000000, 1), glm::vec3(748.000000, 651.000000, 1),
        glm::vec3(744.000000, 655.000000, 1), glm::vec3(740.000000, 658.000000, 1), glm::vec3(739.000000, 659.000000, 1), glm::vec3(737.000000, 661.000000, 1), glm::vec3(735.000000, 663.000000, 1),
        glm::vec3(730.000000, 665.000000, 1), glm::vec3(725.000000, 668.000000, 1), glm::vec3(722.000000, 669.000000, 1), glm::vec3(718.000000, 670.000000, 1), glm::vec3(714.000000, 671.000000, 1),
        glm::vec3(710.000000, 672.000000, 1), glm::vec3(706.000000, 672.000000, 1), glm::vec3(703.000000, 672.000000, 1), glm::vec3(699.000000, 672.000000, 1), glm::vec3(696.000000, 672.000000, 1),
        glm::vec3(692.000000, 671.000000, 1), glm::vec3(689.000000, 670.000000, 1), glm::vec3(687.000000, 669.000000, 1), glm::vec3(681.000000, 665.000000, 1), glm::vec3(677.000000, 662.000000, 1),
        glm::vec3(674.000000, 661.000000, 1), glm::vec3(670.000000, 657.000000, 1), glm::vec3(668.000000, 652.000000, 1), glm::vec3(664.000000, 645.000000, 1), glm::vec3(662.000000, 640.000000, 1),
        glm::vec3(660.000000, 634.000000, 1), glm::vec3(658.000000, 630.000000, 1), glm::vec3(657.000000, 623.000000, 1), glm::vec3(656.000000, 621.000000, 1), glm::vec3(655.000000, 616.000000, 1),
        glm::vec3(653.000000, 610.000000, 1), glm::vec3(653.000000, 605.000000, 1), glm::vec3(653.000000, 602.000000, 1), glm::vec3(653.000000, 598.000000, 1), glm::vec3(653.000000, 595.000000, 1),
        glm::vec3(653.000000, 592.000000, 1), glm::vec3(654.000000, 588.000000, 1), glm::vec3(654.000000, 586.000000, 1), glm::vec3(657.000000, 582.000000, 1), glm::vec3(658.000000, 580.000000, 1),
        glm::vec3(659.000000, 578.000000, 1), glm::vec3(661.000000, 574.000000, 1), glm::vec3(664.000000, 570.000000, 1), glm::vec3(666.000000, 567.000000, 1), glm::vec3(667.000000, 565.000000, 1),
        glm::vec3(669.000000, 563.000000, 1), glm::vec3(670.000000, 562.000000, 1), glm::vec3(672.000000, 558.000000, 1), glm::vec3(675.000000, 556.000000, 1), glm::vec3(676.000000, 555.000000, 1),
        glm::vec3(677.000000, 554.000000, 1), glm::vec3(677.000000, 554.000000, 1), glm::vec3(678.000000, 553.000000, 1), glm::vec3(679.000000, 553.000000, 1), glm::vec3(682.000000, 552.000000, 1),
        glm::vec3(683.000000, 551.000000, 1), glm::vec3(684.000000, 551.000000, 1), glm::vec3(685.000000, 550.000000, 1), glm::vec3(687.000000, 550.000000, 1), glm::vec3(689.000000, 549.000000, 1),
        glm::vec3(693.000000, 549.000000, 1), glm::vec3(695.000000, 549.000000, 1), glm::vec3(697.000000, 549.000000, 1), glm::vec3(697.000000, 549.000000, 1), glm::vec3(697.000000, 549.000000, 1),
        glm::vec3(697.000000, 549.000000, 1), glm::vec3(697.000000, 549.000000, 1), glm::vec3(698.000000, 549.000000, 1), glm::vec3(698.000000, 549.000000, 1), glm::vec3(698.000000, 549.000000, 1),
        glm::vec3(698.000000, 549.000000, 1), glm::vec3(698.000000, 549.000000, 1)
    }, result;

    result.clear();

    r.recognize(result, circle);
}
