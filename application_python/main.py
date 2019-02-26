from lib.si import SI, MainInteractionSourceRegion

if __name__ == '__main__':
    si = SI()

    si.add_region(MainInteractionSourceRegion().get_instance())

    si.run()
