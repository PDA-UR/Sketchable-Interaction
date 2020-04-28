from libPySI import PySIEffect

from SI.plugins.standard_environment_library import SIEffect


class Container(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Container, self).__init__(shape, uuid, self.TEXTURE_PATH_NONE, kwargs)
        self.name = PySIEffect.SI_STD_NAME_CONTAINER
        self.region_type = PySIEffect.EffectType.SI_EXTERNAL_APPLICATION_CONTAINER
        self.source = "libStdSI"
        self.color = PySIEffect.Color(255, 0, 0, 255)

        self.disable_effect(PySIEffect.DELETION, self.RECEPTION)
        self.disable_effect(PySIEffect.MOVE, self.RECEPTION)
        self.disable_link_reception(PySIEffect.POSITION)
        self.enable_link_reception(PySIEffect.GEOMETRY, PySIEffect.GEOMETRY, self.set_geometry_from_geometry)

    def set_geometry_from_geometry(self, abs_x, abs_y, width, height):
        if self.width == width and self.height == height:
            self.x = abs_x
            self.y = abs_y

            self.width = width
            self.height = height
        else:
            self.shape = PySIEffect.PointVector([[abs_x, abs_y], [abs_x, abs_y + height], [abs_x + width, abs_y + height], [abs_x + width, abs_y]])
            self.__notify_shape_changed__(True)
            self.has_data_changed = True
            self.x = abs_x
            self.y = abs_y
            self.width = width
            self.height = height
