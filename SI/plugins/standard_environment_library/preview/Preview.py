from libPySI import PySI

from plugins.standard_environment_library import SIEffect


region_type = PySI.EffectType.SI_PREVIEW
region_name = PySI.EffectName.SI_STD_NAME_PREVIEW
region_display_name = "Preview File"


class Preview(SIEffect.SIEffect):
    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Preview, self).__init__(shape, uuid, "res/preview.png", kwargs)
        self.name = PySI.EffectName.SI_STD_NAME_PREVIEW
        self.region_type = PySI.EffectType.SI_PREVIEW
        self.source = "libStdSI"
        self.qml_path = "plugins/standard_environment_library/preview/Preview.qml"
        self.color = PySI.Color(0, 80, 80, 255)

        self.enable_effect(PySI.CollisionCapability.PREVIEW, self.EMISSION, self.on_preview_enter_emit, self.on_preview_continuous_emit, self.on_preview_continuous_emit)

    def on_preview_enter_emit(self, other):
        pass

    def on_preview_continuous_emit(self, other):
        pass

    def on_preview_leave_emit(self, other):
        pass
