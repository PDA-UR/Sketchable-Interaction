from si import SI
from plugins import Test
from PyQt5 import QtWidgets

if __name__ == '__main__':


    si = SI()

    si.register_plugin(Test.Test().get_instance())

    si.run()
