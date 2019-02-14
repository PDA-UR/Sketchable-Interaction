
import unittest as ut
import unittest.mock as mock

from lib.si import SI, SIRegion


class SITest(ut.TestCase):
    def test_si_init(self):
        reference = SI()

        self.assertNotEqual(reference, None)

    @mock.patch.object(SI, 'run')
    def test_run(self, mock_si_run):
        reference = SI()

        mock_si_run.return_value = True
        self.assertTrue(reference.run())

        mock_si_run.return_value = False
        self.assertFalse(reference.run())

    @mock.patch.object(SI, "__push_region_to_si_core__")
    def test_si_add_region(self, mock_si_push_region_to_si_core):
        reference = SI()

        reference.add_region(None)
        self.assertFalse(mock_si_push_region_to_si_core.called, "None object as region")

        def test_callback_a():
            pass

        def test_callback_b():
            pass

        def test_callback_c():
            pass

        region = SIRegion(test_callback_a, test_callback_b, test_callback_c).get_instance()

        reference.add_region(region)
        self.assertTrue(mock_si_push_region_to_si_core.called, "region object as region")
        mock_si_push_region_to_si_core.assert_called_with(region)
