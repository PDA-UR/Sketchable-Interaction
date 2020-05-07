from libPySI import PySIEffect

from plugins.standard_environment_library import SIEffect


region_type = PySIEffect.EffectType.SI_PREVIEW
region_name = PySIEffect.SI_STD_NAME_PREVIEW


class Preview(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Preview, self).__init__(shape, uuid, "res/preview.png", kwargs)
        self.name = PySIEffect.SI_STD_NAME_PREVIEW
        self.region_type = PySIEffect.EffectType.SI_PREVIEW
        self.source = "libStdSI"
        self.qml_path = "plugins/standard_environment_library/preview/Preview.qml"
        self.color = PySIEffect.Color(0, 80, 80, 255)

        self.enable_effect(PySIEffect.PREVIEW, self.EMISSION, self.on_preview_enter_emit, self.on_preview_continuous_emit, self.on_preview_continuous_emit)

    def on_preview_enter_emit(self, other):
        pass

    def on_preview_continuous_emit(self, other):
        pass

    def on_preview_leave_emit(self, other):
        pass
