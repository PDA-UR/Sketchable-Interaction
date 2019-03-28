from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Region import Region
from Capabiliy import Capability


class MoveableRegion(Region):
    posi = pyqtSignal(int, int, object)

    def __init__(self, x, y, parent=None):
        super(MoveableRegion, self).__init__(x, y, parent)

        self.add_capability(Capability.POSITION, "posi", "position")

        self.movable = False
        self.oldpos = QPoint()

        self.label = QLabel()
        self.label.setParent(self)

        self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")
        self.label.setGeometry(15, -self.height() / 2 + 15, self.width(), self.height())
        self.label.show()

    def __repr__(self):
        return "MoveableRegion: " + str(int(self.winId()))

    def position(self, x, y, uid):
        if Capability.POSITION in self.link_events.keys():
            if uid != self.link_events[Capability.POSITION]:

                # actually required from app programmer, rest is safeguards against infinite recursion
                self.move(self.x() + x, self.y() + y)
                self.label.setText("pos: (" + str(self.pos().x()) + ", " + str(self.pos().y()) + ")")
                ######################################################################################

                self.add_link_event(Capability.POSITION, uid)
                eval('self.' + self.get_capability_signal(Capability.POSITION)).emit(x, y, uid)
                self.update_visualization_handles()

        self.add_link_event(Capability.POSITION, uid)
