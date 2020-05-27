
from libPySI import PySIStartup, PySI
from plugins.standard_environment_library.notification import SimpleNotification
from plugins.standard_environment_library.filesystem import Directory
from plugins.standard_environment_library.cursor import Cursor

def add_canvas():
    canvas_shape = [[0, 0],
                    [0, PySIStartup.PySIStartup.context_dimensions()[1]],
                    [PySIStartup.PySIStartup.context_dimensions()[0], PySIStartup.PySIStartup.context_dimensions()[1]],
                    [PySIStartup.PySIStartup.context_dimensions()[0], 0]]

    PySIStartup.PySIStartup.create_region_by_id(canvas_shape, PySI.EffectType.SI_CANVAS, {})

def add_mouse_cursor():
    mouse_shape = [[0, 0],
                   [0, Cursor.region_height],
                   [Cursor.region_width, Cursor.region_height],
                   [Cursor.region_width, 0]]

    PySIStartup.PySIStartup.create_region_by_id(mouse_shape, PySI.EffectType.SI_MOUSE_CURSOR, {})

def add_simple_notification():
    x = PySIStartup.PySIStartup.context_dimensions()[0] / 2 - SimpleNotification.region_width / 2

    simple_notification_shape = [[x, 75],
                                 [x, 75 + SimpleNotification.region_height],
                                 [x + SimpleNotification.region_width, 75 + SimpleNotification.region_height],
                                 [x + SimpleNotification.region_width, 75]]

    PySIStartup.PySIStartup.create_region_by_id(simple_notification_shape, PySI.EffectType.SI_NOTIFICATION, {})

def add_palette():
    palette_shape = [[PySIStartup.PySIStartup.context_dimensions()[0] - 300, 75],
                     [PySIStartup.PySIStartup.context_dimensions()[0] - 300, 475],
                     [PySIStartup.PySIStartup.context_dimensions()[0] - 100, 475],
                     [PySIStartup.PySIStartup.context_dimensions()[0] - 100, 75]]

    PySIStartup.PySIStartup.create_region_by_id(palette_shape, PySI.EffectType.SI_PALETTE, {})

def add_start_directory():
    directory_path = "" # if empty, the Desktop will be used

    directory_shape = [[0, 75],
                       [0, 75 + Directory.region_height],
                       [Directory.region_width, 75 + Directory.region_height],
                       [Directory.region_width, 75]]

    kwargs = {"cwd": directory_path, "parent": ""}
    PySIStartup.PySIStartup.create_region_by_id(directory_shape, PySI.EffectType.SI_DIRECTORY, kwargs)

import math

## Author: RW
def add_many_regions(num = 100, area_width= 1200, area_height=600):
    left = (PySIStartup.PySIStartup.context_dimensions()[0] - area_width) // 2
    top = (PySIStartup.PySIStartup.context_dimensions()[1] - area_height) // 2
    num_h = math.ceil(math.sqrt(num / (area_width/area_height)))
    num_w = math.floor(num/num_h)
    # print(f"creating region matrix: {num_w}, {num_h}")
    for i in range(num):
        row = i // num_w
        col = i % num_w
        # print(f"Creating region {i}: row {row}, col {col}")
        x = col * (area_width // num_w) + left
        y = row * (area_height // num_h) + top
        w = int(area_width // num_w * 0.9)
        h = int(area_height // num_h * 0.9)
        r_shape = [[x,y], [x, y+h], [x+w, y+h], [x+w, y]]
        PySIStartup.PySIStartup.create_region_by_id(r_shape, PySI.EffectType.SI_DELETION, {})

def on_startup():
    PySIStartup.PySIStartup.logger_log(True)
    PySIStartup.PySIStartup.logger_set_log_output(PySIStartup.SI_LOG_SHOW_ALL)
    PySIStartup.PySIStartup.logger_quench_messages_from_class("linkingmanager")

    add_canvas()
    add_mouse_cursor()
    add_simple_notification()
    add_palette()
    add_start_directory()

    add_many_regions(100)
