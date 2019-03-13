from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Region import Region

import time


class HeartBeat(QThread):
    heart_beat = pyqtSignal()

    def __init__(self, frequency, sleep=True):
        super(HeartBeat, self).__init__()
        self.is_running = True
        self.frequency = frequency
        self.sleep = sleep

    def run(self):
        while self.is_running:
            if self.sleep:
                time.sleep(1.0 / self.frequency)

            self.heart_beat.emit()


class Canvas(QMainWindow):
    def __init__(self, width, height):
        super(Canvas, self).__init__()

        self.setGeometry(0, 0, width, height)
        self.showFullScreen()

        self.widget_list = []

        Region(0, 0, self)
        Region(250, 0, self)

        self.heart_beat = HeartBeat(33.0)
        self.heart_beat.heart_beat.connect(self.update_all)
        self.heart_beat.start()

        w1, w2 = self.findChildren(Region)

        self.make_link(w1, w2, 'pos')

        self.show()

    def update_all(self):
        pass

        # handle here link stuff?

    def keyPressEvent(self, ev):
        if ev.key() == Qt.Key_Escape:
            self.close()
        elif ev.key() == Qt.Key_A:
            # spawn new widget at 0, 0 (or space where it has room)
            pass

    def make_link(self, w1, w2, attribute):
        # is the attribute a capability?
        # model complex linkage states as directional graph?
        # how to resolve cyclic graphs?

        if attribute == "pos":
            w1.posi.connect(w2.position)


    def undo_link(self, w1, w2):
        pass
