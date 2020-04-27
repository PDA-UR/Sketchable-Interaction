from libPySI import PySIEffect

from SI.plugins.standard_environment_library import SIEffect


class Deletion(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Deletion, self).__init__(shape, aabb, uuid, "res/deletion.png", kwargs)
        self.name = PySIEffect.SI_STD_NAME_DELETION
        self.region_type = PySIEffect.EffectType.SI_DELETION
        self.qml_path = "plugins/standard_environment_library/deletion/Deletion.qml"
        self.color = PySIEffect.Color(255, 255, 0, 255)
        self.is_under_user_control = False

        self.enable_effect(PySIEffect.DELETION, True, self.on_deletion_enter_emit, self.on_deletion_continuous_emit, self.on_deletion_enter_emit)

    def on_deletion_enter_emit(self, other):
        if other.region_type is int(PySIEffect.EffectType.SI_DELETION):
            if self.is_under_user_control:
                other.__signal_deletion__()

    def on_deletion_continuous_emit(self, other):
        if other.region_type is not int(PySIEffect.EffectType.SI_DELETION):
            if not other.is_under_user_control:
                other.__signal_deletion__()
