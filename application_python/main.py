from lib.si import SI
from plugins import Test

if __name__ == '__main__':
    si = SI()

    # si.set_main_canvas(Test.Test().get_instance())

    si.add_region(Test.Test().get_instance())

    si.run()
