import unittest
import sys

from libPySI import PySIEffect

class PySIPySIEffectTest(unittest.TestCase):
    def test(self):
        self.assertTrue(True, True)

    @staticmethod
    def start():
        return unittest.TextTestRunner(stream=sys.stdout, verbosity=3).run(unittest.TestLoader().loadTestsFromTestCase(PySIPySIEffectTest)).wasSuccessful()
