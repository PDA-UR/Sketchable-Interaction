from libPySI import PySIEffect

region_name = "Test"
region_type = 0


class Test(PySIEffect):
    def __init__(self):
        self.name = region_name
        self.type = region_type

        self.cap_emit = {

        }

        self.cap_receive = {

        }

        self.cap_link_emit = {

        }

        self.cap_link_recv = {

        }