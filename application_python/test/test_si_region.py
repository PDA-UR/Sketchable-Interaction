
import unittest as ut
import unittest.mock as mock

from lib.si import SI, SIRegion

from PyQt5 import QtWidgets


"""
see test_si.py for details concerning testing architecture
"""

class SIRegionTest(ut.TestCase):
    def si_region_init(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        self.reference = SI()

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c)

        self.assertTrue(region is not None)

    def si_region_find_main_window(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c)

        self.assertFalse(region.parent() is None)

    @mock.patch.object(SIRegion, 'clean_up')
    def si_region_clean_up(self, mock_clean_up):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c)

        region.on_destroy(1)

        self.assertTrue(mock_clean_up.called, "cleaned up")

    def si_region_get_instance(self):
        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c)

        self.assertTrue(region.get_instance() is not None)
        self.assertTrue(type(region.get_instance()) is int)

    def si_region_callbacks(self):
        def test_callback_a():
            return 0

        def test_callback_b():
            return 1

        def test_callback_c():
            return 2

        # untestable as of now; unsure whether this is covered by engine enough or not


    def functions(self):
        return [
            self.si_region_init,
            self.si_region_find_main_window,
            self.si_region_clean_up,
            self.si_region_get_instance,
            self.si_region_callbacks
        ]

    def test_functions(self):
        for f in self.functions():
            try:
                f()
            except Exception as e:
                self.fail("{0} failed ({1}: {2})".format(f, type(e), e))
