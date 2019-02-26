from si.si import SI
from si.SIMouseRegionDefault import SIMouseRegionDefault

if __name__ == '__main__':
    si = SI()

    si.add_region(SIMouseRegionDefault().get_instance())

    si.run()
