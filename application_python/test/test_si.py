
import unittest as ut
import unittest.mock as mock

from lib.si import SI, SIRegion


"""
Apparently python3 unittest module sorts functions based on name.
Therefore, the order of definition of functions in test classes does not matter.
For integration tests the order generally does not matter.
However, there are small instances where we want a specific order, such as testing behaviour with or without SI context.
I prefer setting up a monolithic order to having python3 sort the functions by name and executing them afterwards.
Of course, both are arbitrary sequences, however the monolithic order is more intuitive and faster to make sense of.

I recommend using the pattern below for future test implementations
"""

import os
os.environ['QT_QPA_PLATFORM'] = 'offscreen'


class SITest(ut.TestCase):
    def setUp(self):
        self.reference = SI()

    def tearDown(self):
        self.reference.quit()

    def si_init(self):
        self.assertNotEqual(self.reference, None)

    @mock.patch.object(SI, 'run')
    def si_run(self, mock_si_run):
        mock_si_run.return_value = True
        self.assertTrue(self.reference.run())

        mock_si_run.return_value = False
        self.assertFalse(self.reference.run())

    @mock.patch.object(SI, "__push_region_to_si_core__")
    def si_add_region(self, mock_si_push_region_to_si_core):
        self.reference.add_region(None)
        self.assertFalse(mock_si_push_region_to_si_core.called, "None object as region")

        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c)

        self.reference.add_region(region.get_instance())
        self.assertTrue(mock_si_push_region_to_si_core.called, "region object as region")
        mock_si_push_region_to_si_core.assert_called_with(region.get_instance())

        region.on_destroy(1)

    def functions(self):
        return [
            self.si_init,
            self.si_run,
            self.si_add_region]

    def test_functions(self):
        for f in self.functions():
            try:
                f()
            except Exception as e:
                self.fail("{0} failed ({1}: {2})".format(f, type(e), e))

