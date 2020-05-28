from libPySI import PySI
from plugins.standard_environment_library import SIEffect


class Palette(SIEffect.SIEffect):
    regiontype = PySI.EffectType.SI_PALETTE
    regionname = PySI.EffectName.SI_STD_NAME_PALETTE

    def __init__(self, shape=PySI.PointVector(), uuid="", kwargs={}):
        super(Palette, self).__init__(shape, uuid, "", Palette.regiontype, Palette.regionname, kwargs)
        self.source = "libStdSI"
        self.qml_path = ""
        available_plugins = self.available_plugins()

        self.enable_link_emission(PySI.LinkingCapability.POSITION, self.position)

        self.disable_effect(PySI.CollisionCapability.DELETION, self.RECEPTION)
        self.as_selector = True
        self.num_selectors_per_row = int(len(available_plugins) / 3) + 1

        if len(available_plugins) % self.num_selectors_per_row == 0:
            self.num_rows = len(available_plugins) / self.num_selectors_per_row
        else:
            self.num_rows = int(len(available_plugins) / self.num_selectors_per_row) + 1

        self.x_offset = 5
        self.y_offset = 5

        self.selector_width = self.get_region_width() / self.num_selectors_per_row - self.x_offset * self.num_selectors_per_row
        self.selector_height = self.get_region_height() / self.num_rows - self.y_offset * self.num_rows

        y = -1
        x = 1

        for i in range(len(available_plugins)):
            if i % self.num_selectors_per_row:
                x += 1
            else:
                x = x - self.num_selectors_per_row - 1 if x - self.num_selectors_per_row - 1 >= 0 else 0
                y += 1

            shape = [[((self.x_offset + self.selector_width) * x) + (self.relative_x_pos() + self.x_offset), ((self.y_offset + self.selector_height) * y) + (self.relative_y_pos() + self.y_offset)],
                     [((self.x_offset + self.selector_width) * x) + (self.relative_x_pos() + self.x_offset), ((self.y_offset + self.selector_height) * y) + (self.relative_y_pos() + self.y_offset + self.selector_height)],
                     [((self.x_offset + self.selector_width) * x) + (self.relative_x_pos() + self.x_offset + self.selector_width), ((self.y_offset + self.selector_height) * y) + (self.relative_y_pos() + self.y_offset + self.selector_height)],
                     [((self.x_offset + self.selector_width) * x) + (self.relative_x_pos() + self.x_offset + self.selector_width), ((self.y_offset + self.selector_height) * y) + (self.relative_y_pos() + self.y_offset)]]

            self.create_region_via_name(shape, available_plugins[i], self.as_selector, {"parent": self._uuid})

    def position(self):
        x = self.x - self.last_x
        y = self.y - self.last_y

        self.last_x = self.x
        self.last_y = self.y

        return x, y, self.x, self.y