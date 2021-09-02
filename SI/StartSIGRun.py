
from libPySI import PySI
from plugins.__loaded_plugins__.standard_environment_library.notification import SimpleNotification
from plugins.__loaded_plugins__.standard_environment_library.canvas import Canvas
from plugins.__loaded_plugins__.standard_environment_library.filesystem import Directory
from plugins.__loaded_plugins__.standard_environment_library.palette import Palette
from plugins.__loaded_plugins__.standard_environment_library.cursor import Cursor
from plugins.__loaded_plugins__.standard_environment_library.deletion import Deletion
from plugins.__loaded_plugins__.standard_environment_library.slider import SliderBase
from plugins.__loaded_plugins__.standard_environment_library.slider import SliderTargetDummy
from plugins.__loaded_plugins__.standard_environment_library.tag import Tag
from plugins.__loaded_plugins__.standard_environment_library.presentation import Presentation
from plugins.__loaded_plugins__.standard_environment_library.preview import Preview
from plugins.__loaded_plugins__.standard_environment_library.lasso import Lasso
from plugins.__loaded_plugins__.standard_environment_library.plot import Plot
from plugins.__loaded_plugins__.standard_environment_library.image_editor import ImageEditor
from plugins.__loaded_plugins__.standard_environment_library.video import Video
from plugins.__loaded_plugins__.standard_environment_library.filesystem import OpenEntry
from plugins.__loaded_plugins__.standard_environment_library.terminal import Terminal
from plugins.__loaded_plugins__.standard_environment_library.unredo import Undo, Redo
from plugins.__loaded_plugins__.standard_environment_library.automation import ConveyorBelt, ConveyorBeltMerger, ConveyorBeltSplitter
from plugins.standard_environment_library.tangible.camera.ScanCameraAreaDetection import ScanCameraAreaDetection
from plugins.standard_environment_library.tangible.camera.TableArea import TableArea
from plugins.standard_environment_library.tangible.document.tools.Color import Color

import math

def add_canvas(kwargs={}):
    canvas_shape = [[0, 0],
                    [0, PySI.Startup.context_dimensions()[1]],
                    [PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]],
                    [PySI.Startup.context_dimensions()[0], 0]]

    PySI.Startup.create_region_by_name(canvas_shape, Canvas.Canvas.regionname, kwargs)

def add_mouse_cursor():
    mouse_shape = [[0, 0],
                   [0, Cursor.Cursor.region_height],
                   [Cursor.Cursor.region_width, Cursor.Cursor.region_height],
                   [Cursor.Cursor.region_width, 0]]

    PySI.Startup.create_region_by_name(mouse_shape, Cursor.Cursor.regionname, {})

def add_simple_notification():
    x = PySI.Startup.context_dimensions()[0] / 2 - SimpleNotification.SimpleNotification.region_width / 2

    simple_notification_shape = [[x, 75],
                                 [x, 75 + SimpleNotification.SimpleNotification.region_height],
                                 [x + SimpleNotification.SimpleNotification.region_width, 75 + SimpleNotification.SimpleNotification.region_height],
                                 [x + SimpleNotification.SimpleNotification.region_width, 75]]

    PySI.Startup.create_region_by_name(simple_notification_shape, SimpleNotification.SimpleNotification.regionname, {})

def add_palette():
    palette_shape = [[PySI.Startup.context_dimensions()[0] - 400, 75],
                     [PySI.Startup.context_dimensions()[0] - 400, 475],
                     [PySI.Startup.context_dimensions()[0] - 100, 475],
                     [PySI.Startup.context_dimensions()[0] - 100, 75]]

    PySI.Startup.create_region_by_name(palette_shape, Palette.Palette.regionname, {})

def add_start_directory():
    directory_path = "/home/juergen/Desktop/"

    directory_shape = [[75, 75],
                       [75, 75 + Directory.Directory.region_height],
                       [75 + Directory.Directory.region_width, 75 + Directory.Directory.region_height],
                       [75 + Directory.Directory.region_width, 75]]

    kwargs = {"cwd": directory_path, "parent": ""}
    PySI.Startup.create_region_by_name(directory_shape, Directory.Directory.regionname, kwargs)

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

    PySI.Startup.create_region_by_name(undo_shape, Undo.Undo.regionname, {})
    PySI.Startup.create_region_by_name(redo_shape, Redo.Redo.regionname, {})

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

APPLICATION = 1
COLOR_PICKER = 2
CAMERA_CALIBRATION = 4
BENCHMARK = 8
TABLE_AREA_CALIBRATION = 16

def start_application():
    # rgba = {}
    rgba = {"rgba": (0, 0, 0, 255)}

    add_canvas(rgba)
    add_mouse_cursor()
    add_simple_notification()
    add_palette()
    add_start_directory()
    add_unredo()
    add_annotation_color()

def on_start():
    # PySI.Startup.disable(PySI.Configuration.SI_CRASH_DUMP)
    PySI.Startup.enable(PySI.Configuration.SI_ANTI_ALIASING_8x)

    PySI.Startup.logger_log(True)
    PySI.Startup.logger_set_log_output(PySI.Logger.SI_LOG_SHOW_ALL)
    PySI.Startup.logger_quench_messages_from_class("linkingmanager")
    PySI.Startup.logger_quench_messages_from_class("recognizer")
    # PySI.Startup.logger_quench_messages_from_class("mainwindow")

    # PySI.Startup.set_pen_color(PySI.Configuration.PEN_CLOLOR_BLACK)
    PySI.Startup.set_pen_color(PySI.Configuration.PEN_CLOLOR_WHITE)

    PySI.Startup.set_tangible_ip_address_and_port("132.199.128.250", 3333)
    # PySI.Startup.set_tangible_ip_address_and_port("10.61.3.117", 3333)
    # PySI.Startup.set_tangible_ip_address_and_port("127.0.0.1", 3333)

    PySI.Startup.exclude_plugins([
        # SimpleNotification.SimpleNotification.regionname
        ConveyorBelt.ConveyorBelt.regionname,
        ConveyorBeltSplitter.ConveyorBeltSplitter.regionname,
        ConveyorBeltMerger.ConveyorBeltMerger.regionname,
        Tag.Tag.regionname,
        Plot.Plot.regionname,
        Presentation.Presentation.regionname,
        Lasso.Lasso.regionname,
        Preview.Preview.regionname,
        ImageEditor.ImageEditor.regionname,
        # OpenEntry.OpenEntry.regionname,
        Terminal.Terminal.regionname,
        Undo.Undo.regionname,
        Redo.Redo.regionname,
    #     Video.Video.regionname
    ])

    CHOICE = APPLICATION

    if APPLICATION & CHOICE:
        start_application()
    elif COLOR_PICKER & CHOICE:
        add_canvas()
        add_mouse_cursor()
        add_slider([[500, 500], [500, 530], [800, 530], [800, 500]], "r")
        add_slider([[500, 600], [500, 630], [800, 630], [800, 600]], "g")
        add_slider([[500, 700], [500, 730], [800, 730], [800, 700]], "b")
        add_slider_target([[1000, 570], [1000, 670], [1100, 670], [1100, 570]])
    elif CAMERA_CALIBRATION & CHOICE:
        add_canvas()
        add_mouse_cursor()
        add_camera_calibration()
    elif BENCHMARK & CHOICE:
        add_canvas()
        add_mouse_cursor()
        add_many_regions(500)
    elif TABLE_AREA_CALIBRATION & CHOICE:
        add_canvas()
        add_mouse_cursor()

        w, h = PySI.Startup.context_dimensions()[0], PySI.Startup.context_dimensions()[1]

        PySI.Startup.create_region_by_name([[1, 1], [1, h - 1], [w + 1, h - 1], [w + 1, 1]], TableArea.regionname, {})