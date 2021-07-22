
from libPySI import PySI
from plugins.__loaded_plugins__.standard_environment_library.notification import SimpleNotification
from plugins.__loaded_plugins__.standard_environment_library.filesystem import Directory
from plugins.__loaded_plugins__.standard_environment_library.cursor import Cursor
from plugins.__loaded_plugins__.standard_environment_library.deletion import Deletion
from plugins.__loaded_plugins__.standard_environment_library.slider import SliderBase
from plugins.__loaded_plugins__.standard_environment_library.slider import SliderTargetDummy
from plugins.__loaded_plugins__.standard_environment_library.unredo import Undo, Redo
from plugins.standard_environment_library.tangible.camera.ScanCameraAreaDetection import ScanCameraAreaDetection

import math

def add_canvas():
    canvas_shape = [[0, 0],
                    [0, PySI.Startup.context_dimensions()[1]],
                    [PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]],
                    [PySI.Startup.context_dimensions()[0], 0]]

    PySI.Startup.create_region_by_type(canvas_shape, PySI.EffectType.SI_CANVAS, {})

def add_mouse_cursor():
    mouse_shape = [[0, 0],
                   [0, Cursor.Cursor.region_height],
                   [Cursor.Cursor.region_width, Cursor.Cursor.region_height],
                   [Cursor.Cursor.region_width, 0]]

    PySI.Startup.create_region_by_class(mouse_shape, Cursor, {})

def add_simple_notification():
    x = PySI.Startup.context_dimensions()[0] / 2 - SimpleNotification.SimpleNotification.region_width / 2

    simple_notification_shape = [[x, 75],
                                 [x, 75 + SimpleNotification.SimpleNotification.region_height],
                                 [x + SimpleNotification.SimpleNotification.region_width, 75 + SimpleNotification.SimpleNotification.region_height],
                                 [x + SimpleNotification.SimpleNotification.region_width, 75]]

    PySI.Startup.create_region_by_class(simple_notification_shape, SimpleNotification, {})

def add_palette():
    palette_shape = [[PySI.Startup.context_dimensions()[0] - 400, 75],
                     [PySI.Startup.context_dimensions()[0] - 400, 475],
                     [PySI.Startup.context_dimensions()[0] - 100, 475],
                     [PySI.Startup.context_dimensions()[0] - 100, 75]]

    PySI.Startup.create_region_by_type(palette_shape, PySI.EffectType.SI_PALETTE, {})

def add_start_directory():
    directory_path = "" # if empty, the Desktop will be used

    directory_shape = [[75, 75],
                       [75, 75 + Directory.Directory.region_height],
                       [75 + Directory.Directory.region_width, 75 + Directory.Directory.region_height],
                       [75 + Directory.Directory.region_width, 75]]

    kwargs = {"cwd": directory_path, "parent": ""}
    PySI.Startup.create_region_by_type(directory_shape, PySI.EffectType.SI_DIRECTORY, kwargs)

def add_unredo():
    w, h = PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]
    redo_shape = [[w - 100, h - 100],
                  [w - 100, h - 5],
                  [w - 5, h - 5],
                  [w - 5, h - 100]]

    undo_shape = [[w - 100 - 5 - 95, h - 100],
                  [w - 100 - 5 - 95, h - 5],
                  [w - 100 - 5, h - 5],
                  [w - 100 - 5, h - 100]]

    PySI.Startup.create_region_by_class(undo_shape, Undo, {})
    PySI.Startup.create_region_by_class(redo_shape, Redo, {})

## Author: RW
def add_many_regions(num = 100, area_width= 1600, area_height=800):
    left = (PySI.Startup.context_dimensions()[0] - area_width) // 2
    top = (PySI.Startup.context_dimensions()[1] - area_height) // 2
    num_h = math.ceil(math.sqrt(num / (area_width/area_height)))
    num_w = math.floor(num/num_h)
    # print(f"creating region matrix: {num_w}, {num_h}")

    directory_path = ""

    for i in range(num):
        row = i // num_w
        col = i % num_w
        # print(f"Creating region {i}: row {row}, col {col}")
        x = col * (area_width // num_w) + left
        y = row * (area_height // num_h) + top
        w = int(area_width // num_w * 0.9)
        h = int(area_height // num_h * 0.9)
        r_shape = [[x,y], [x, y+h], [x+w, y+h], [x+w, y]]

        kwargs = {"cwd": directory_path, "parent": ""}

        PySI.Startup.create_region_by_class(r_shape, Deletion, kwargs)


def add_slider(shape, c):
    PySI.Startup.create_region_by_class(shape, SliderBase, {"color_channel": c})


def add_slider_target(shape):
    PySI.Startup.create_region_by_class(shape, SliderTargetDummy, {})

def add_camera_calibration():
    shape = [[0, 0],
             [0, PySI.Startup.context_dimensions()[1]],
             [PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]],
             [PySI.Startup.context_dimensions()[0], 0]]

    PySI.Startup.create_region_by_name(shape, ScanCameraAreaDetection.regionname, {})

def on_start():
    # PySI.Startup.disable(PySI.Configuration.SI_CRASH_DUMP)
    PySI.Startup.enable(PySI.Configuration.SI_ANTI_ALIASING_8x)

    PySI.Startup.logger_log(True)
    PySI.Startup.logger_set_log_output(PySI.Logger.SI_LOG_SHOW_ALL)
    PySI.Startup.logger_quench_messages_from_class("linkingmanager")
    PySI.Startup.logger_quench_messages_from_class("recognizer")
    PySI.Startup.logger_quench_messages_from_class("mainwindow")

    APPLICATION = 1
    COLOR_PICKER = 2
    CAMERA_CALIBRATION = 4
    BENCHMARK = 8

    CHOICE = APPLICATION

    add_canvas()
    add_mouse_cursor()

    if(APPLICATION & CHOICE):
        add_simple_notification()
        add_palette()
        add_start_directory()
        add_unredo()
    elif(COLOR_PICKER & CHOICE):
        add_slider([[500, 500], [500, 530], [800, 530], [800, 500]], "r")
        add_slider([[500, 600], [500, 630], [800, 630], [800, 600]], "g")
        add_slider([[500, 700], [500, 730], [800, 730], [800, 700]], "b")
        add_slider_target([[1000, 570], [1000, 670], [1100, 670], [1100, 570]])
    elif(CAMERA_CALIBRATION & CHOICE):
        add_camera_calibration()
    elif(BENCHMARK & CHOICE):
        add_many_regions(500)