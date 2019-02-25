from ctypes import *
from PyQt5 import QtWidgets, QtCore
import sys
import math


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

    def quit(self):
        self.lib.si_delete_instance(self.si_instance)

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
        self.close()

        self.clean_up()

        print("Hello Destroy Python")

        return 0

    def on_create(self, uuid):
        print("Hello Create Python")

        return 0

    def get_instance(self):
        return self.plugin_instance


class MainCanvasRegion(SIRegion):
    def __init__(self, orec, orcc, orlc):
        super(MainCanvasRegion, self).__init__(orec, orcc, orlc)


class MainInteractionSourceRegion(SIRegion):
    def __init__(self):
        super(MainInteractionSourceRegion, self).__init__(self.on_region_enter, self.on_region_continuous, self.on_region_leave)

        x_values, y_values, length = self.circle(0, 0, 10, 30)

        SI.lib.si_region_set_as_main_interaction_source(None, self.plugin_instance, x_values, y_values, length)

    def circle(self, cx, cy, radius, num_segments):
        x_values = []
        y_values = []

        for i in range(num_segments):
            theta = 2.0 * math.pi * i / num_segments

            x = radius * math.cos(theta)
            y = radius * math.sin(theta)

            x_values.append(cx + x)
            y_values.append(cy + y)

        length = len(x_values)
        x_arr = (c_int * length)()
        y_arr = (c_int * length)()

        for i in range(length):
            x_arr[i] = int(x_values[i])
            y_arr[i] = int(y_values[i])

        return x_arr, y_arr, length

    def on_region_enter(self, uuid):
        print("Hello Enter Mouse Python")
        return 0

    def on_region_continuous(self, uuid):
        print("Hello Conti Mouse Python")
        return 0

    def on_region_leave(self, uuid):
        print("Hello Leave Mouse Python")

        return 0

if __name__ == '__main__':
    pass
