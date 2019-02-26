from ctypes import *
from PyQt5 import QtWidgets, QtCore, QtGui
import sys
import math
from enum import Enum


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


class SIInteractionSourceRegionShapeTypes(Enum):
    CIRCLE = 0
    SQUARE = 1
    CUSTOM = 9


class SIInteractionSourceType(Enum):
    MOUSE = 0
    TOUCH = 1
    TANGIBLE = 2
    CUSTOM = 9


class MainInteractionSourceRegion(SIRegion):
    def __init__(self, oce, occ, ocl, source_type, shape_type=SIInteractionSourceRegionShapeTypes.CIRCLE.value, *args, **kwargs):
        super(MainInteractionSourceRegion, self).__init__(oce, occ, ocl)

        if shape_type is SIInteractionSourceRegionShapeTypes.CIRCLE.value:
            if len(args) == 0:
                x_values, y_values, length = self.circle((0, 0, 10), num_segments=30)
            else:
                x_values, y_values, length = self.circle(args, num_segments=30)
        elif shape_type is SIInteractionSourceRegionShapeTypes.SQUARE.value:
            if len(args) == 0:
                x_values, y_values, length = self.square((0, 0, 10, 10))
            else:
                x_values, y_values, length = self.square(args)
        else:
            x_values, y_values, length = kwargs['function'](args)

        SI.lib.si_region_set_as_main_interaction_source(None, self.plugin_instance, x_values, y_values, length)

    # args[0] is cx, args[1] is cy, args[2] is width, args[3] is height
    def square(self, args):
        cx = args[0]
        cy = args[1]
        width = args[2]
        height = args[3]

        x_values = [cx, cx, cx + width, cx + width]
        y_values = [cy, cy + height, cy + height, cy]

        length = len(x_values)

        length = len(x_values)
        x_arr = (c_int * length)()
        y_arr = (c_int * length)()

        for i in range(length):
            x_arr[i] = int(x_values[i])
            y_arr[i] = int(y_values[i])

        return x_arr, y_arr, length

    # args[0] is cx, args[1] is cy, args[2] is radius
    def circle(self, args, num_segments):
        x_values = []
        y_values = []

        for i in range(num_segments):
            theta = 2.0 * math.pi * i / num_segments

            x_values.append(args[0] + args[2] * math.cos(theta))
            y_values.append(args[1] + args[2] * math.sin(theta))

        length = len(x_values)
        x_arr = (c_int * length)()
        y_arr = (c_int * length)()

        for i in range(length):
            x_arr[i] = int(x_values[i])
            y_arr[i] = int(y_values[i])

        return x_arr, y_arr, length


if __name__ == '__main__':
    pass
