from si.si import MainInteractionSourceRegion, SIInteractionSourceType


class SIMouseRegionDefault(MainInteractionSourceRegion):
    def __init__(self):
        super(SIMouseRegionDefault, self).__init__(self.on_enter, self.on_continuous, self.on_leave, source_type=SIInteractionSourceType.MOUSE.value)

    def on_enter(self, uuid):
        print("Mouse Default enter")

        return 0

    def on_continuous(self, uuid):
        print("Mouse Default continuous")

        return 0

    def on_leave(self, uuid):
        print("Mouse Default leave")

        return 0
