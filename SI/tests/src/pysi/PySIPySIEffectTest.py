import unittest
import sys

from libPySI import PySIEffect


def inner_test():
    return 0


def inner_test2():
    return 0

region_name = "TEST_CASES"
region_type = -1


class TestClass(PySIEffect.PySIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(TestClass, self).__init__(shape, uuid, "", kwargs)


class PySIPySIEffectTest(unittest.TestCase):
    def test_scope_variable_ON_ENTER(self):
        self.assertNotEqual(PySIEffect.ON_ENTER, None)

    def test_scope_variable_ON_CONTINUOUS(self):
        self.assertNotEqual(PySIEffect.ON_CONTINUOUS, None)

    def test_scope_variable_ON_LEAVE(self):
        self.assertNotEqual(PySIEffect.ON_LEAVE, None)


    def test_scope_variable_MOVE(self):
        self.assertNotEqual(PySIEffect.MOVE, None)

    def test_scope_variable_BTN(self):
        self.assertNotEqual(PySIEffect.BTN, None)

    def test_scope_variable_OPEN_ENTRY(self):
        self.assertNotEqual(PySIEffect.OPEN_ENTRY, None)

    def test_scope_variable_PARENT(self):
        self.assertNotEqual(PySIEffect.PARENT, None)

    def test_scope_variable_SKETCH(self):
        self.assertNotEqual(PySIEffect.SKETCH, None)

    def test_scope_variable_CLICK(self):
        self.assertNotEqual(PySIEffect.CLICK, None)

    def test_scope_variable_DELETION(self):
        self.assertNotEqual(PySIEffect.DELETION, None)


    def test_scope_variable_POSITION(self):
        self.assertNotEqual(PySIEffect.POSITION, None)

    def test_scope_variable_ROTATION(self):
        self.assertNotEqual(PySIEffect.ROTATION, None)

    def test_scope_variable_SCALE(self):
        self.assertNotEqual(PySIEffect.SCALE, None)

    def test_scope_variable_COLOR(self):
        self.assertNotEqual(PySIEffect.COLOR, None)

    def test_scope_variable_GEOMETRY(self):
        self.assertNotEqual(PySIEffect.GEOMETRY, None)


    def test_scope_variable_SI_STD_NAME_DIRECTORY(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_DIRECTORY, None)

    def test_scope_variable_SI_STD_NAME_TEXTFILE(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_TEXTFILE, None)

    def test_scope_variable_SI_STD_NAME_BUTTON(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_BUTTON, None)

    def test_scope_variable_SI_STD_NAME_TAG(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_TAG, None)

    def test_scope_variable_SI_STD_NAME_DELETION(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_DELETION, None)

    def test_scope_variable_SI_STD_NAME_OPEN_ENTRY(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_OPEN_ENTRY, None)

    def test_scope_variable_SI_STD_NAME_MOUSE_CURSOR(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_MOUSE_CURSOR, None)

    def test_scope_variable_SI_STD_NAME_CANVAS(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_CANVAS, None)

    def test_scope_variable_SI_STD_NAME_SIMPLE_NOTIFICATION(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_SIMPLE_NOTIFICATION, None)

    def test_scope_variable_SI_STD_NAME_ENTRY(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_ENTRY, None)

    def test_scope_variable_SI_STD_NAME_CONTAINER(self):
        self.assertNotEqual(PySIEffect.SI_STD_NAME_CONTAINER, None)

    def test_scope_datastructure_LinkRelation(self):
        lr = PySIEffect.LinkRelation("test", "attrib_test", "test2", "attrib_test2")

        self.assertNotEqual(lr, None)

        self.assertEqual(lr.sender, "test")
        self.assertEqual(lr.sender_attrib, "attrib_test")
        self.assertEqual(lr.recv, "test2")
        self.assertEqual(lr.recv_attrib, "attrib_test2")

    def test_scope_datastructure_Color(self):
        red = PySIEffect.Color(255, 0, 0, 255)
        green = PySIEffect.Color(0, 255, 0, 255)
        blue = PySIEffect.Color(0, 0, 255, 255)
        no_alpha = PySIEffect.Color(255, 0, 0, 0)

        self.assertEqual(red.r, 255)
        self.assertEqual(red.g, 0)
        self.assertEqual(red.b, 0)
        self.assertEqual(red.a, 255)

        self.assertEqual(green.r, 0)
        self.assertEqual(green.g, 255)
        self.assertEqual(green.b, 0)
        self.assertEqual(green.a, 255)

        self.assertEqual(blue.r, 0)
        self.assertEqual(blue.g, 0)
        self.assertEqual(blue.b, 255)
        self.assertEqual(blue.a, 255)

        self.assertEqual(no_alpha.a, 0)

    def test_scope_datastructure_Point3(self):
        p = PySIEffect.Point3(1337, 69, 1)

        self.assertEqual(p.x, 1337)
        self.assertEqual(p.y, 69)
        self.assertEqual(p.z, 1)

    def test_scope_datastructure_Point2(self):
        p = PySIEffect.Point2(1337, 69)

        self.assertEqual(p.x, 1337)
        self.assertEqual(p.y, 69)

    def test_scope_datastructure_PointVector(self):
        pv = PySIEffect.PointVector()
        self.assertEqual(len(pv), 0)

        pv = PySIEffect.PointVector([[1, 1]])
        self.assertEqual(len(pv), 1)

        pv = PySIEffect.PointVector([[1, 1], [2, 2], [3, 3], [4, 4]])
        self.assertEqual(len(pv), 4)

        # get
        self.assertEqual(pv[0].x, PySIEffect.Point2(1, 1).x)
        self.assertEqual(pv[0].y, PySIEffect.Point2(1, 1).y)

        # get_slice
        self.assertEqual(len(pv[:2]), 2)
        self.assertEqual(len(pv[::2]), 2)

        self.assertEqual(pv[0].x, PySIEffect.Point2(1, 1).x)
        self.assertEqual(pv[0].y, PySIEffect.Point2(1, 1).y)

        qv = pv[::2]
        self.assertEqual(qv[1].x, PySIEffect.Point2(3, 3).x)
        self.assertEqual(qv[1].y, PySIEffect.Point2(3, 3).y)
        self.assertEqual(qv[0].x, PySIEffect.Point2(1, 1).x)
        self.assertEqual(qv[0].y, PySIEffect.Point2(1, 1).y)

        # set
        qv[1] = [1337, 69]
        self.assertEqual(qv[1].x, PySIEffect.Point2(1337, 69).x)
        self.assertEqual(qv[1].y, PySIEffect.Point2(1337, 69).y)

        # del
        self.assertEqual(len(pv), 4)
        del pv[1]
        self.assertEqual(len(pv), 3)

        self.assertEqual(pv[1].x, PySIEffect.Point2(3, 3).x)
        self.assertEqual(pv[1].y, PySIEffect.Point2(3, 3).y)

        # add
        self.assertEqual(len(pv), 3)

        pv.append([1337, 69])
        self.assertEqual(len(pv), 4)

        self.assertEqual(pv[3].x, PySIEffect.Point2(1337, 69).x)
        self.assertEqual(pv[3].y, PySIEffect.Point2(1337, 69).y)

        # in
        for p in qv:
            self.assertNotEqual(p, None)
            self.assertTrue((p.x == 1 or p.x == 1337) and (p.y == 1 or p.y == 69))

        self.assertTrue(PySIEffect.Point3(1337, 69, 1) in qv)

        # index
        self.assertEqual(qv.index(qv[1]), 1)
        self.assertEqual(qv.index(PySIEffect.Point3(1337, 69, 1)), 1)
        self.assertEqual(qv.index(PySIEffect.Point3(1, 1, 1)), 0)

    def test_scope_datastructure_LinkRelationVector(self):
        lv = PySIEffect.LinkRelationVector()
        self.assertNotEqual(lv, None)

        lv = PySIEffect.LinkRelationVector([["test", "test_attrib", "test2", "test_attrib2"], ["test3", "test_attrib3", "test4", "test_attrib4"]])

        self.assertEqual(len(lv), 2)

        # get

        self.assertEqual(lv[0].sender, "test")
        self.assertEqual(lv[0].sender_attrib, "test_attrib")
        self.assertEqual(lv[0].recv, "test2")
        self.assertEqual(lv[0].recv_attrib, "test_attrib2")

        self.assertEqual(lv[1].sender, "test3")
        self.assertEqual(lv[1].sender_attrib, "test_attrib3")
        self.assertEqual(lv[1].recv, "test4")
        self.assertEqual(lv[1].recv_attrib, "test_attrib4")

        # get slice
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

        # set
        lv[0] = ["test5", "test_attrib5", "test6", "test_attrib6"]

        self.assertEqual(lv[0].sender, "test5")
        self.assertEqual(lv[0].sender_attrib, "test_attrib5")
        self.assertEqual(lv[0].recv, "test6")
        self.assertEqual(lv[0].recv_attrib, "test_attrib6")

        # del
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

        # add
        lv.append(["test7", "test_attrib7", "test8", "test_attrib8"])
        self.assertEqual(len(lv), 3)

        self.assertEqual(lv[2].sender, "test7")
        self.assertEqual(lv[2].sender_attrib, "test_attrib7")
        self.assertEqual(lv[2].recv, "test8")
        self.assertEqual(lv[2].recv_attrib, "test_attrib8")

        # in
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

        # index
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

    def test_scope_datastructure_StringVector(self):
        sv = PySIEffect.StringVector()

        self.assertNotEqual(sv, None)
        self.assertEqual(len(sv), 0)

        sv = PySIEffect.StringVector(["hello", "world", "test", "test2"])
        self.assertEqual(len(sv), 4)

        # get
        # StringVector is currently Write-Only


        # get slice
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

        # set
        sv[0] = "HELLO"

        # del
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

        # add
        sv.append("TEST3")
        self.assertEqual(len(sv), 5)

        # in
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

        # index
        # same functionality for all exposed vectors
        # therefore, tested in PointVector

    def test_scope_datastructure_PartialContour(self):
        pc = PySIEffect.PartialContour()

        self.assertEqual(len(pc), 0)

        pc = PySIEffect.PartialContour({
            "TEST": PySIEffect.PointVector([[1, 1, 1], [2, 2, 2], [3, 3, 3]]),
            "TEST2": PySIEffect.PointVector([[4, 4, 1], [5, 5, 2], [6, 6, 3]])
        })

        self.assertEqual(len(pc), 2)

        # get
        test = pc["TEST"]
        self.assertNotEqual(test, None)

        self.assertEqual(pc["TEST"][0].x, 1)
        self.assertEqual(pc["TEST"][1].x, 2)
        self.assertEqual(pc["TEST"][2].x, 3)

        # set

        pc["TEST"] = PySIEffect.PointVector([9, 9, 1])
        self.assertEqual(pc["TEST"][0].x, 9)

        pc["TEST3"] = PySIEffect.PointVector([8, 8, 1])
        self.assertEqual(pc["TEST3"][0].x, 8)
        self.assertEqual(pc["TEST3"][0].y, 8)

        # del

        self.assertEqual(len(pc), 3)

        del pc["TEST2"]

        self.assertEqual(len(pc), 2)

        # in

        self.assertFalse("TEST2" in pc)
        self.assertTrue("TEST" in pc)
        self.assertTrue("TEST3" in pc)

        # keys

        keys = pc.keys()

        self.assertNotEqual(keys, None)
        self.assertEqual(len(keys), 2)
        self.assertEqual(keys[0], "TEST")
        self.assertEqual(keys[1], "TEST3")

        # values

        values = pc.values()

        self.assertNotEqual(values, None)
        self.assertEqual(len(values), 2)

        self.assertEqual(values[0][0].x, 9)
        self.assertEqual(values[1][0].x, 8)
        # items

        items = pc.items()

        self.assertEqual(len(items), 2)

        keys = [t[0] for t in items]
        values = [t[1] for t in items]

        self.assertNotEqual(keys, None)
        self.assertEqual(len(keys), 2)
        self.assertEqual(keys[0], "TEST")
        self.assertEqual(keys[1], "TEST3")

        self.assertNotEqual(values, None)
        self.assertEqual(len(values), 2)

        self.assertEqual(values[0][0].x, 9)
        self.assertEqual(values[1][0].x, 8)

        # index
        # unused

    def test_scope_datastructure_String2FunctionMap(self):
        sfm = PySIEffect.String2FunctionMap()

        self.assertNotEqual(sfm, None)
        self.assertEqual(len(sfm), 0)

        sfm = PySIEffect.String2FunctionMap({
            "test": inner_test,
            "test2": inner_test2
        })

        # self.assertEqual(len(sfm), 1)

        # set
        sfm["test"] = inner_test2

        # String2FunctionMap is write-only from python and used in SIGRun
        # except for set, all other functionality is generic and tested in PartialContour

    def test_scope_datastructure_String2_String2FunctionMap_Map(self):
        ssfmm = PySIEffect.String2_String2FunctionMap_Map()

        self.assertNotEqual(ssfmm, None)
        self.assertEqual(len(ssfmm), 0)

        ssfmm = PySIEffect.String2_String2FunctionMap_Map({
            "TEST": {"TEST1_1": None, "TEST1_2": None, "TEST1_3": None},
            "TEST2": {"TEST2_1": None, "TEST2_2": None, "TEST2_3": None}
        })

        self.assertEqual(len(ssfmm), 2)

        #set

        ssfmm["TEST"] = {"TEST4_1": None, "TEST4_2": None, "TEST4_3": None}

        # String2_String2FunctionMap_Map is write-only from python and used in SIGRun
        # except for set, all other functionality is generic and tested in PartialContour and String2FunctionMap

    def test_scope_datastructure_enum_DataType(self):
        self.assertNotEqual(PySIEffect.DataType, None)
        self.assertNotEqual(PySIEffect.DataType.INT, None)
        self.assertNotEqual(PySIEffect.DataType.FLOAT, None)
        self.assertNotEqual(PySIEffect.DataType.BOOL, None)
        self.assertNotEqual(PySIEffect.DataType.STRING, None)

    def test_scope_datastructure_enum_EffectType(self):
        self.assertNotEqual(PySIEffect.EffectType, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_CANVAS, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_CURSOR, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_MOUSE_CURSOR, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_EXTERNAL_APPLICATION_CONTAINER, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_DIRECTORY, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_TEXT_FILE, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_IMAGE_FILE, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_UNKNOWN_FILE, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_DELETION, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_BUTTON, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_NOTIFICATION, None)
        self.assertNotEqual(PySIEffect.EffectType.SI_CUSTOM, None)

    def test_class_variables(self):
        pyeff = TestClass()

        self.assertNotEqual(pyeff, None)
        self.assertNotEqual(pyeff.__partial_regions__, None)
        self.assertNotEqual(pyeff.cap_emit, None)
        self.assertNotEqual(pyeff.cap_recv, None)
        self.assertNotEqual(pyeff.cap_link_emit, None)
        self.assertNotEqual(pyeff.cap_link_recv, None)
        self.assertNotEqual(pyeff.__registered_regions__, None)
        self.assertNotEqual(pyeff.color, None)
        self.assertNotEqual(pyeff.x, None)
        self.assertNotEqual(pyeff.y, None)
        self.assertNotEqual(pyeff.width, None)
        self.assertNotEqual(pyeff.height, None)
        self.assertNotEqual(pyeff.angle_degrees, None)
        self.assertNotEqual(pyeff.scale, None)
        self.assertNotEqual(pyeff.name, None)
        self.assertNotEqual(pyeff._uuid, None)
        self.assertNotEqual(pyeff.source, None)
        self.assertNotEqual(pyeff.qml_path, None)
        self.assertNotEqual(pyeff.region_type, None)
        self.assertNotEqual(pyeff.left_mouse_clicked, None)
        self.assertNotEqual(pyeff.right_mouse_clicked, None)
        self.assertNotEqual(pyeff.middle_mouse_clicked, None)
        self.assertNotEqual(pyeff.link_relations, None)
        self.assertNotEqual(pyeff.shape, None)
        self.assertNotEqual(pyeff.aabb, None)
        self.assertNotEqual(pyeff.has_data_changed, None)
        self.assertNotEqual(pyeff.mouse_wheel_angle_px, None)
        self.assertNotEqual(pyeff.mouse_wheel_angle_degrees, None)

    def test_class_functions(self):
        pyeff = TestClass()

        self.assertNotEqual(pyeff.__init__, None)
        self.assertNotEqual(pyeff.__add_data__, None)
        self.assertNotEqual(pyeff.__signal_deletion__, None)
        self.assertNotEqual(pyeff.__show_folder_contents_page__, None)
        self.assertNotEqual(pyeff.__embed_file_standard_appliation_into_context__, None)
        self.assertNotEqual(pyeff.__destroy_embedded_window__, None)

    @staticmethod
    def start():
        utest = unittest.TextTestRunner(stream=sys.stdout, verbosity=2).run(unittest.TestLoader().loadTestsFromTestCase(PySIPySIEffectTest))

        print(utest.failures)

        return utest.wasSuccessful()
