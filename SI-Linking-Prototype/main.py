from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Canvas import Canvas

import sys

if __name__ == '__main__':
    app = QApplication(sys.argv)
    w = QMainWindow()

    canvas = Canvas(1920, 1080)


    sys.exit(app.exec_())