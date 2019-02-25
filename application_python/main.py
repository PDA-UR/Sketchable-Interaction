from lib.si import SI
from plugins import Test

if __name__ == '__main__':
    si = SI()

    si.add_region(Test.Test().get_instance())

    si.run()
