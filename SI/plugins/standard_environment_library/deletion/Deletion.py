from libPySI import PySI

from plugins.standard_environment_library import SIEffect


region_type = PySI.EffectType.SI_DELETION
region_name = PySI.EffectName.SI_STD_NAME_DELETION
region_display_name = "Deletion"


class Deletion(SIEffect.SIEffect):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Deletion, self).__init__(shape, uuid, "res/deletion.png", kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_DELETION
        self.region_type = PySI.EffectType.SI_DELETION
        self.qml_path = "plugins/standard_environment_library/deletion/Deletion.qml"
        self.color = PySI.Color(255, 255, 0, 255)
        self.is_under_user_control = False

        self.enable_effect(PySI.CollisionCapability.DELETION, True, self.on_deletion_enter_emit, self.on_deletion_continuous_emit, self.on_deletion_enter_emit)

    def on_deletion_enter_emit(self, other):
        if other.region_type is int(PySI.EffectType.SI_DELETION):
            if self.is_under_user_control:
                other.delete()

    def on_deletion_continuous_emit(self, other):
        if other.region_type is not int(PySI.EffectType.SI_DELETION):
            if not other.is_under_user_control:
                other.delete()
