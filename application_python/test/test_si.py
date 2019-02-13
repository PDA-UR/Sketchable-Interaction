
import unittest as ut

from lib.si import SI


class SITest(ut.TestCase):
    def test_si_init(self):
        si = SI()

        self.assertNotEqual(si, None)

if __name__ == '__main__':
    suite = ut.TestLoader().discover("test", pattern=test_filename)
    ut.TextTestRunner(verbosity=2).run(suite)
