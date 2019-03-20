from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from PyQt5.QtCore import *

from Capabiliy import Capability

from Region import Region


class CompressionRegion(Region):
    def __init__(self, x, y, parent=None):
        super(CompressionRegion, self).__init__(x, y, parent)

    @staticmethod
    def from_linking_graph(region):
        # from region get whole graph
        # compress into x compression regions based on graph composition

        # if len compression regions == 1 return the region
        # else return a tuple of compression regions

        pass

