from ctypes import *


class SI:
    def __init__(self):
        self.lib = cdll.LoadLibrary('../si_engine/core/cmake-build-debug/libsi-core.so')

    def run(self):
        self.lib.restype = c_int

        return int(self.lib.run())

    def register_plugin(self, p):
        self.lib.register_plugin.argtypes = [POINTER(c_void_p)]
        self.lib.register_plugin.restype = c_int

        p_ptr = c_void_p(p)

        return int(self.lib.register_plugin(p_ptr))

if __name__ == '__main__':
    pass

