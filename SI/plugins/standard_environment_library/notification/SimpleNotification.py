from libPySI import PySI

from plugins.standard_environment_library import SIEffect


region_type = PySI.EffectType.SI_NOTIFICATION
region_name = PySI.EffectName.SI_STD_NAME_SIMPLE_NOTIFICATION
region_width = 800
region_height = 75


class SimpleNotification(SIEffect.SIEffect):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(SimpleNotification, self).__init__(shape, uuid, "", kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_SIMPLE_NOTIFICATION
        self.region_type = PySI.EffectType.SI_NOTIFICATION
        self.source = "libstdSI"
        self.qml_path = "plugins/standard_environment_library/notification/SimpleNotification.qml"
        self.color = PySI.Color(255, 255, 255, 255)
        self.message = "Hello World"

        self.width = region_width
        self.height = region_height

        self.add_QML_data("containerwidth", self.width, PySI.DataType.INT)
        self.add_QML_data("containerheight", self.height, PySI.DataType.INT)
        self.add_QML_data("text", self.message, PySI.DataType.STRING)

        self.disable_effect(PySI.CollisionCapability.DELETION, self.RECEPTION)
        self.disable_effect(PySI.CollisionCapability.MOVE, self.RECEPTION)

    def update_message(self, message):
        self.message = message
        self.add_QML_data("message", self.message, PySI.DataType.STRING)
