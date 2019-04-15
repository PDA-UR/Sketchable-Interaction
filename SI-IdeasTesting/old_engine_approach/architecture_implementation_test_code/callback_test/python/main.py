
from ctypes import *


def test_callback_enter(integer):
    print("Entered: ", str(integer))

def test_callback_continuous(floating):
    print("Continuous: ", floating)

def test_callback_leave(string):
    print("Leave: ", str(string.decode()))

if __name__ == '__main__':
    d = cdll.LoadLibrary(r"../cpp/cmake-build-debug/libcpp.so")

    callback_type_enter = CFUNCTYPE(None, c_int)
    callback_type_continuous = CFUNCTYPE(None, c_float)
    callback_type_leave = CFUNCTYPE(None, c_char_p)

    callback_enter = callback_type_enter(test_callback_enter)
    callback_continuous = callback_type_continuous(test_callback_continuous)
    callback_leave = callback_type_leave(test_callback_leave)
    d.set_callback(callback_enter, callback_continuous, callback_leave)

    d.trigger()
