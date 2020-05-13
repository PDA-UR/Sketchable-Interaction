from libPySI import PySIEffect
from plugins.standard_environment_library import SIEffect


region_type = PySIEffect.EffectType.SI_PALETTE
region_name = PySIEffect.SI_STD_NAME_PALETTE


class Palette(SIEffect.SIEffect):
    def __init__(self, shape=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(Palette, self).__init__(shape=shape, uuid=uuid, texture_path="", kwargs=kwargs)
        self.name = PySIEffect.SI_STD_NAME_PALETTE
        self.region_type = PySIEffect.EffectType.SI_PALETTE
        self.source = "libStdSI"
        self.qml_path = ""
        available_plugins = self.available_plugins()

        self.enable_link_emission(PySIEffect.POSITION, self.position)

        self.disable_effect(PySIEffect.DELETION, self.RECEPTION)
        self.as_selector = True
        self.num_selectors_per_row = int(len(available_plugins) / 3) + 1

        if len(available_plugins) % self.num_selectors_per_row == 0:
            self.num_rows = len(available_plugins) / self.num_selectors_per_row
        else:
            self.num_rows = int(len(available_plugins) / self.num_selectors_per_row) + 1

        self.x_offset = 5
        self.y_offset = 5

        self.selector_width = self.region_width() / self.num_selectors_per_row - self.x_offset * self.num_selectors_per_row
        self.selector_height = self.region_height() / self.num_rows - self.y_offset * self.num_rows

        y = -1
        x = 1

        for i in range(len(available_plugins)):
            if i % self.num_selectors_per_row:
                x += 1
            else:
                x = x - self.num_selectors_per_row - 1 if x - self.num_selectors_per_row - 1 >= 0 else 0
                y += 1

            shape = [[((self.x_offset + self.selector_width) * x) + (self.x_pos() + self.x_offset), ((self.y_offset + self.selector_height) * y) + (self.y_pos() + self.y_offset)],
                     [((self.x_offset + self.selector_width) * x) + (self.x_pos() + self.x_offset), ((self.y_offset + self.selector_height) * y) + (self.y_pos() + self.y_offset + self.selector_height)],
                     [((self.x_offset + self.selector_width) * x) + (self.x_pos() + self.x_offset + self.selector_width), ((self.y_offset + self.selector_height) * y) + (self.y_pos() + self.y_offset + self.selector_height)],
                     [((self.x_offset + self.selector_width) * x) + (self.x_pos() + self.x_offset + self.selector_width), ((self.y_offset + self.selector_height) * y) + (self.y_pos() + self.y_offset)]]

            self.create_region(shape, available_plugins[i], self.as_selector, {"parent": self._uuid})

    def position(self):
        x = self.x - self.last_x
        y = self.y - self.last_y

        self.last_x = self.x
        self.last_y = self.y

        return x, y, self.x, self.y