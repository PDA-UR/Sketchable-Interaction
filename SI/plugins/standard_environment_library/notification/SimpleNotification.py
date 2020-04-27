from libPySI import PySIEffect

from SI.plugins.standard_environment_library import SIEffect


class SimpleNotification(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(SimpleNotification, self).__init__(shape, aabb, uuid, "", kwargs)
        self.name = PySIEffect.SI_STD_NAME_SIMPLE_NOTIFICATION
        self.region_type = PySIEffect.EffectType.SI_NOTIFICATION
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/notification/SimpleNotification.qml"
        self.color = PySIEffect.Color(255, 255, 255, 255)
        self.message = "Hello World"

        self.width = 800
        self.height = 75

        self.add_QML_data("rect_width", self.width, PySIEffect.DataType.INT)
        self.add_QML_data("rect_height", self.height, PySIEffect.DataType.INT)
        self.add_QML_data("message", self.message, PySIEffect.DataType.STRING)

    def update_message(self, message):
        self.message = message
        self.add_QML_data("message", self.message, PySIEffect.DataType.STRING)
