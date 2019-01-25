from ctypes import *
from PyQt5 import QtWidgets, QtCore
import sys


class SI:
    lib = cdll.LoadLibrary('../si_engine/core/cmake-build-debug/libsi-core.so')

    def __init__(self):

        argc = len(sys.argv)
        argv = (POINTER(c_char) * (argc + 1))()

        for i, arg in enumerate(sys.argv):
            enc_arg = arg.encode('utf-8')
            argv[i] = create_string_buffer(enc_arg)

        self.si_instance = self.lib.si_create_instance(argc, argv)

    def run(self):
        return self.lib.si_run(self.si_instance)

    def register_plugin(self, plugin):
        return self.lib.si_register_plugin(self.si_instance, plugin)


class SIPlugin(QtWidgets.QWidget):
    def __init__(self, typ, orec, orcc, orlc, occ, odc):
        super(SIPlugin, self).__init__(self.findMainWindow())

        orec_type = CFUNCTYPE(c_int, c_long)
        orcc_type = CFUNCTYPE(c_int)
        orlc_type = CFUNCTYPE(c_int, c_long)
        occ_type = CFUNCTYPE(c_int)
        odc_type = CFUNCTYPE(c_int)

        orecf = orec_type(orec)
        orccf = orcc_type(orcc)
        orlcf = orlc_type(orlc)
        occf = occ_type(occ)
        odcf = odc_type(odc)

        self.references = [orecf, orccf, orlcf, occf, odcf]
        self.plugin_instance = SI.lib.si_plugin_create_instance(typ.encode('utf-8'), orecf, orccf, orlcf, occf, odcf)

    def findMainWindow(self):
        app = QtWidgets.QApplication.instance()
        for widget in app.topLevelWidgets():
            if isinstance(widget, QtWidgets.QMainWindow):
                return widget
        return None

    def get_instance(self):
        return self.plugin_instance


if __name__ == '__main__':
    pass
