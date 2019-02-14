from ctypes import *
from PyQt5 import QtWidgets, QtCore
import sys


class SI:
    lib = cdll.LoadLibrary("bin/libsi-core.so")

    def __init__(self):
        argc = len(sys.argv)
        argv = (POINTER(c_char) * (argc + 1))()

        for i, arg in enumerate(sys.argv):
            enc_arg = arg.encode('utf-8')
            argv[i] = create_string_buffer(enc_arg)

        self.si_instance = self.lib.si_create_instance(argc, argv, False)

    def run(self):
        return True if self.lib.si_run(self.si_instance) == 0 else False

    def add_region(self, region):
        if region is not None:
            self.__push_region_to_si_core__(region)

        return False

    def __push_region_to_si_core__(self, region):
        self.lib.si_add_region(self.si_instance, region)


class SIRegion(QtWidgets.QWidget):
    def __init__(self, orec, orcc, orlc):
        self.references = []
        self.plugin_instance = None

        try:
            parent = self.find_main_window()
            super(SIRegion, self).__init__(parent)

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
        except AttributeError:
            raise AttributeError

    def find_main_window(self):
        app = QtWidgets.QApplication.instance()

        if app is None:
            raise AttributeError

        for widget in app.topLevelWidgets():
            if isinstance(widget, QtWidgets.QMainWindow):
                return widget.findChild(QtWidgets.QWidget, "main_canvas")

        return None

    def clean_up(self):
        self.setParent(None)
        self.references = []
        self.plugin_instance = None

    def on_destroy(self, uuid):
        self.clean_up()
        self.close()

        print("Hello Destroy Python")

        return 0

    def on_create(self, uuid):
        print("Hello Create Python")

        return 0

    def get_instance(self):
        return self.plugin_instance


if __name__ == '__main__':
    pass
