from ctypes import *
from PyQt5 import QtWidgets, QtCore
import sys


class SI:
    lib = cdll.LoadLibrary('../si_engine/core/cmake-build-debug/libsi-core.so')

    def __init__(self, advanced_init=False):

        argc = len(sys.argv)
        argv = (POINTER(c_char) * (argc + 1))()

        for i, arg in enumerate(sys.argv):
            enc_arg = arg.encode('utf-8')
            argv[i] = create_string_buffer(enc_arg)

        self.si_instance = self.lib.si_create_instance(argc, argv, advanced_init)

    def run(self):
        return self.lib.si_run(self.si_instance)

    # rather override
    def set_main_canvas(self, canvas):
        return self.lib.si_set_main_canvas(self.si_instance, canvas)

    def add_region(self, region):
        return self.lib.si_add_region(self.si_instance, region)


class SIRegion(QtWidgets.QWidget):
    def __init__(self, orec, orcc, orlc):
        super(SIRegion, self).__init__(self.findMainWindow())

        orec_type = CFUNCTYPE(c_int, c_long)
        orcc_type = CFUNCTYPE(c_int, c_long)
        orlc_type = CFUNCTYPE(c_int, c_long)
        orocc_type = CFUNCTYPE(c_int, c_long)
        ordc_type = CFUNCTYPE(c_int, c_long)

        orecf = orec_type(orec)
        orccf = orcc_type(orcc)
        orlcf = orlc_type(orlc)
        oroccf = orocc_type(self.on_create)
        ordcf = ordc_type(self.on_destroy)

        self.references = [orecf, orccf, orlcf, oroccf, ordcf]
        self.plugin_instance = SI.lib.si_region_create_instance(orecf, orccf, orlcf, oroccf, ordcf)

    def findMainWindow(self):
        app = QtWidgets.QApplication.instance()
        for widget in app.topLevelWidgets():
            if isinstance(widget, QtWidgets.QMainWindow):
                return widget.findChild(QtWidgets.QWidget, "main_canvas")

        return None

    def on_destroy(self, uuid):
        self.setParent(None)
        self.close()
        self.references = []

        print("Hello Destroy Python")

        return 0

    def on_create(self, uuid):
        print("Hello Create Python")

        return 0

    def get_instance(self):
        return self.plugin_instance


if __name__ == '__main__':
    pass
