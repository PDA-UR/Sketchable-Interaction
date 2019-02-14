
import unittest as ut
import unittest.mock as mock

from lib.si import SI, SIRegion

from PyQt5 import QtWidgets


class SIRegionTest(ut.TestCase):
    def test_init(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        reference = SI()

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c)

        self.assertTrue(region is not None)

    def test_find_main_window(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

    def test_clean_up(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

    def test_get_instance(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

    def test_callbacks(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass