
from libPySI import PySIStartup, PySIEffect
from plugins.standard_environment_library.notification import SimpleNotification
from plugins.standard_environment_library.filesystem import Directory
from plugins.standard_environment_library.cursor import Cursor

def add_canvas():
    canvas_shape = [[0, 0],
                    [0, PySIStartup.PySIStartup.context_dimensions()[1]],
                    [PySIStartup.PySIStartup.context_dimensions()[0], PySIStartup.PySIStartup.context_dimensions()[1]],
                    [PySIStartup.PySIStartup.context_dimensions()[0], 0]]

    PySIStartup.PySIStartup.create_region_by_id(canvas_shape, PySIEffect.EffectType.SI_CANVAS, {})

def add_mouse_cursor():
    mouse_shape = [[0, 0],
                   [0, Cursor.region_height],
                   [Cursor.region_width, Cursor.region_height],
                   [Cursor.region_width, 0]]

    PySIStartup.PySIStartup.create_region_by_id(mouse_shape, PySIEffect.EffectType.SI_MOUSE_CURSOR, {})

def add_simple_notification():
    x = PySIStartup.PySIStartup.context_dimensions()[0] / 2 - SimpleNotification.region_width / 2

    simple_notification_shape = [[x, 75],
                                 [x, 75 + SimpleNotification.region_height],
                                 [x + SimpleNotification.region_width, 75 + SimpleNotification.region_height],
                                 [x + SimpleNotification.region_width, 75]]

    PySIStartup.PySIStartup.create_region_by_id(simple_notification_shape, PySIEffect.EffectType.SI_NOTIFICATION, {})

def add_palette():
    palette_shape = [[PySIStartup.PySIStartup.context_dimensions()[0] - 300, 75],
                     [PySIStartup.PySIStartup.context_dimensions()[0] - 300, 475],
                     [PySIStartup.PySIStartup.context_dimensions()[0] - 100, 475],
                     [PySIStartup.PySIStartup.context_dimensions()[0] - 100, 75]]

    PySIStartup.PySIStartup.create_region_by_id(palette_shape, PySIEffect.EffectType.SI_PALETTE, {})

def add_start_directory():
    directory_path = "" # if empty, the Desktop will be used

    directory_shape = [[0, 75,],
                       [0, 75 + Directory.region_height],
                       [Directory.region_width, 75 + Directory.region_height],
                       [Directory.region_width, 75]]

    PySIStartup.PySIStartup.create_region_by_id(directory_shape, PySIEffect.EffectType.SI_DIRECTORY, {"cwd": directory_path})

def on_startup():
    add_canvas()
    add_mouse_cursor()
    add_simple_notification()
    add_palette()
    add_start_directory()