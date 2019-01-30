from si import SI
from plugins import Test
from PyQt5 import QtWidgets

if __name__ == '__main__':
    si = SI()

    si.add_region(Test.Test().get_instance())

    si.run()
