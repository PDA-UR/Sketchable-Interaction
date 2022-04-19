
from libPySI import PySI
from plugins.__loaded_plugins__.standard_environment_library.canvas import Canvas
from plugins.__loaded_plugins__.standard_environment_library.palette import Palette
from plugins.__loaded_plugins__.standard_environment_library.cursor import Cursor
from plugins.__loaded_plugins__.standard_environment_library.deletion import Deletion
from plugins.__loaded_plugins__.standard_environment_library.slider import SliderBase
from plugins.__loaded_plugins__.standard_environment_library.slider import SliderTargetDummy
from plugins.standard_environment_library.plot.Plot import Plot
from plugins.standard_environment_library.presentation.Presentation import Presentation
from plugins.standard_environment_library.lasso.Lasso import Lasso
from plugins.standard_environment_library.video.Video import Video
from plugins.standard_environment_library.terminal.Terminal import Terminal

import math

def add_canvas(kwargs={}):
    canvas_shape = [[0, 0],
                    [0, PySI.Startup.context_dimensions()[1]],
                    [PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]],
                    [PySI.Startup.context_dimensions()[0], 0]]

    PySI.Startup.create_region_by_name(canvas_shape, Canvas.Canvas.regionname, kwargs)

def add_mouse_cursor(kwargs):
    mouse_shape = [[0, 0],
                   [0, Cursor.Cursor.region_height],
                   [Cursor.Cursor.region_width, Cursor.Cursor.region_height],
                   [Cursor.Cursor.region_width, 0]]

    PySI.Startup.create_region_by_name(mouse_shape, Cursor.Cursor.regionname, kwargs)

def add_palette():
    palette_shape = [[PySI.Startup.context_dimensions()[0] - 400, 75],
                     [PySI.Startup.context_dimensions()[0] - 400, 475],
                     [PySI.Startup.context_dimensions()[0] - 100, 475],
                     [PySI.Startup.context_dimensions()[0] - 100, 75]]

    PySI.Startup.create_region_by_name(palette_shape, Palette.Palette.regionname, {})

def add_unredo():
    pass
    # w, h = PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]
    # redo_shape = [[w - 100, h - 100],
    #               [w - 100, h - 5],
    #               [w - 5, h - 5],
    #               [w - 5, h - 100]]
    #
    # undo_shape = [[w - 100 - 5 - 95, h - 100],
    #               [w - 100 - 5 - 95, h - 5],
    #               [w - 100 - 5, h - 5],
    #               [w - 100 - 5, h - 100]]
    #
    # PySI.Startup.create_region_by_name(undo_shape, Undo.Undo.regionname, {})
    # PySI.Startup.create_region_by_name(redo_shape, Redo.Redo.regionname, {})

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

        PySI.Startup.create_region_by_name(r_shape, Deletion.Deletion.regionname, kwargs)


def add_slider(shape, c):
    PySI.Startup.create_region_by_name(shape, SliderBase.SliderBase.regionname, {"color_channel": c})


def add_slider_target(shape):
    PySI.Startup.create_region_by_name(shape, SliderTargetDummy.SliderTargetDummy.regionname, {})

def add_camera_calibration():
    shape = [[0, 0],
             [0, PySI.Startup.context_dimensions()[1]],
             [PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]],
             [PySI.Startup.context_dimensions()[0], 0]]

    PySI.Startup.create_region_by_name(shape, ScanCameraAreaDetection.regionname, {})

def add_annotation_color():
    w, h = PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]
    tw, th = 100, 100
    x = w // 2 - tw // 2
    y = 200
    shape = [[x, y], [x, y + th], [x + tw, y + th], [x + tw, y]]
    PySI.Startup.create_region_by_name(shape, Color.regionname, {"color": PySI.Color(100, 100, 20, 255)})

def add_terminal():
    x = 10
    y = 10
    w = 300
    h = 200
    shape = [[x, y], [x, y + h], [x + w, y + h], [x + w, y]]
    PySI.Startup.create_region_by_name(shape, Terminal.regionname, {})


APPLICATION = 1
COLOR_PICKER = 2
CAMERA_CALIBRATION = 4
BENCHMARK = 8
TABLE_AREA_CALIBRATION = 16

def start_application():
    rgba = {}
    # rgba = {"rgba": (0, 0, 0, 255)}

    add_canvas(rgba)
    add_mouse_cursor({"draw": "RMB"})
    add_palette()
    add_unredo()
    add_terminal()
    # add_annotation_color()

def on_start():
    PySI.Startup.enable(PySI.Configuration.SI_ANTI_ALIASING_8x)
    # PySI.Startup.enable(PySI.Configuration.SI_CRASH_DUMP)

    PySI.Startup.logger_log(True)
    PySI.Startup.logger_set_log_output(PySI.Logger.SI_LOG_SHOW_ALL)
    PySI.Startup.logger_quench_messages_from_class("linkingmanager")
    PySI.Startup.logger_quench_messages_from_class("recognizer")
    # PySI.Startup.logger_quench_messages_from_class("mainwindow")

    PySI.Startup.set_pen_color(PySI.Configuration.PEN_CLOLOR_BLACK)
    # PySI.Startup.set_pen_color(PySI.Configuration.PEN_CLOLOR_WHITE)

    PySI.Startup.exclude_plugins([
        Plot.regionname,
        Presentation.regionname,
        Lasso.regionname,
        Video.regionname
    ])

    CHOICE = APPLICATION

    if APPLICATION & CHOICE:
        start_application()
    elif COLOR_PICKER & CHOICE:
        add_canvas()
        add_mouse_cursor({"draw": "RMB"})
        add_slider([[500, 500], [500, 530], [800, 530], [800, 500]], "r")
        add_slider([[500, 600], [500, 630], [800, 630], [800, 600]], "g")
        add_slider([[500, 700], [500, 730], [800, 730], [800, 700]], "b")
        add_slider_target([[1000, 570], [1000, 670], [1100, 670], [1100, 570]])
    elif CAMERA_CALIBRATION & CHOICE:
        add_canvas()
        add_mouse_cursor({"draw": "RMB"})
        add_camera_calibration()
    elif BENCHMARK & CHOICE:
        add_canvas()
        add_mouse_cursor({"draw": "RMB"})
        add_many_regions(500)
    elif TABLE_AREA_CALIBRATION & CHOICE:
        add_canvas()
        add_mouse_cursor({"draw": "RMB"})

        w, h = PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]

        PySI.Startup.create_region_by_name([[1, 1], [1, h - 1], [w + 1, h - 1], [w + 1, 1]], TableArea.regionname, {})