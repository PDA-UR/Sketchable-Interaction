
from PyQt5 import QtWidgets
from sketchable_interaction.environment import Environment


class SketchableInteraction(QtWidgets.QApplication):
    def __init__(self, argv):
        super(SketchableInteraction, self).__init__(argv)

        self.environment = Environment.Environment()
        self.environment.show()
