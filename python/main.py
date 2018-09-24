import sys
from sketchable_interaction import SketchableInteraction


if __name__ == '__main__':
    SI = SketchableInteraction.SketchableInteraction(sys.argv)

    sys.exit(SI.exec_())