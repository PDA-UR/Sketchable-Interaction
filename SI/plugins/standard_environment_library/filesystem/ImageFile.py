from libPySI import PySIEffect
import Entry


class ImageFile(Entry.Entry):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(ImageFile, self).__init__(shape, aabb, uuid, kwargs)
        self.name = PySIEffect.SI_STD_NAME_IMAGEFILE
        self.region_type = PySIEffect.EffectType.SI_IMAGE_FILE
        self.qml_path = "plugins/standard_environment_library/filesystem/ImageFile.qml"
        self.is_in_preview = False

        self.add_QML_data("img_path", self.path, PySIEffect.DataType.STRING)
        self.add_QML_data("is_visible", self.is_visible, PySIEffect.DataType.BOOL)
        self.add_QML_data("is_in_preview", self.is_in_preview, PySIEffect.DataType.BOOL)

        self.cap_recv[PySIEffect.PREVIEW] = {PySIEffect.ON_ENTER: self.on_preview_enter_recv, PySIEffect.ON_CONTINUOUS: self.on_preview_continuous_recv, PySIEffect.ON_LEAVE: self.on_preview_leave_recv}

    def on_preview_enter_recv(self):
        self.is_in_preview = True

        x = self.aabb[0].x
        y = self.aabb[0].y

        self.width = 450
        self.height = 450

        self.shape = PySIEffect.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])
        self.__notify_shape_changed__(True)

        self.add_QML_data("img_path", self.path, PySIEffect.DataType.STRING)
        self.add_QML_data("is_in_preview", self.is_in_preview, PySIEffect.DataType.BOOL)
        self.add_QML_data("container_width", self.width, PySIEffect.DataType.INT)
        self.add_QML_data("container_height", self.height, PySIEffect.DataType.INT)
        self.add_QML_data("icon_width", self.width, PySIEffect.DataType.INT)
        self.add_QML_data("icon_height", self.height, PySIEffect.DataType.INT)

    def on_preview_continuous_recv(self):
        pass

    def on_preview_leave_recv(self):
        self.is_in_preview = False

        x = self.aabb[0].x
        y = self.aabb[0].y

        self.width = self.icon_width * 2
        self.height = self.icon_height + self.text_height

        self.shape = PySIEffect.PointVector([[x, y], [x, y + self.height], [x + self.width, y + self.height], [x + self.width, y]])
        self.register_shape_change(self.RESAMPLING)

        self.add_QML_data("img_path", self.path, PySIEffect.DataType.STRING)
        self.add_QML_data("is_in_preview", self.is_in_preview, PySIEffect.DataType.BOOL)
        self.add_QML_data("container_width", self.width, PySIEffect.DataType.INT)
        self.add_QML_data("container_height", self.height, PySIEffect.DataType.INT)
        self.add_QML_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_QML_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
