from libPySI import PySIEffect, PySICapability
import Entry
import subprocess
import psutil
import time

from SI.plugins.standard_environment_library.filesystem import __OpenEntryHelper as ofh


class TextFile(Entry.Entry):
    def __init__(self, shape=PySIEffect.PointVector(), aabb=PySIEffect.PointVector(), uuid="", kwargs={}):
        super(TextFile, self).__init__(shape, aabb, uuid, kwargs)
        self.name = "stdSITextFile"
        self.region_type = PySIEffect.EffectType.SI_TEXT_FILE
        self.qml_path = "plugins/standard_environment_library/filesystem/TextFile.qml"

        # require here different set of vars for qml to handle application rendering?

        self.special_apps = {
            "soffice.bin": "libreoffice",
        }

        self.pid = 0
        self.process_name = ""
        self.process_app_name = ""
        self.process_create_time = 0.0
        self.process = None
        self.process_winid = ""
        self.process_fetch_winid_sleep_time_ms = 1 / 30
        self.process_fetch_winid_timeout_ms = 500
        self.process_fetch_winid_iterations = self.process_fetch_winid_timeout_ms / self.process_fetch_winid_sleep_time_ms / 1000

        self.add_data("icon_width", self.icon_width, PySIEffect.DataType.INT)
        self.add_data("icon_height", self.icon_height, PySIEffect.DataType.INT)
        self.add_data("text_height", self.text_height, PySIEffect.DataType.INT)
        self.add_data("img_path", "res/file_icon.png", PySIEffect.DataType.STRING)
        self.add_data("color", self.text_color, PySIEffect.DataType.STRING)
        self.add_data("name", self.filename, PySIEffect.DataType.STRING)

        self.cap_recv["OPEN_ENTRY"] = {"on_enter": self.on_open_entry_enter_recv, "on_continuous": self.on_open_entry_continuous_recv, "on_leave": self.on_open_entry_leave_recv}

    def on_open_entry_enter_recv(self):
        return 0

    def on_open_entry_continuous_recv(self):
        if not self.is_child and not self.is_open_entry_capability_blocked and not self.is_under_user_control:
            if self.process is None:
                cmd = ofh.get_default(self.path).getExec()[0:-3].split(" ") + [self.path]

                self.process = subprocess.Popen(cmd, stdout=subprocess.PIPE)

                app = cmd[0] if cmd[0] != "libreoffice" else "soffice.bin"
                l = [p.info for p in psutil.process_iter(attrs=['pid', 'name', 'create_time']) if app in p.info['name']][0]

                self.pid = str(l["pid"])
                self.process_name = l["name"]
                self.process_app_name = app
                self.process_create_time = l["create_time"]

                processes = []
                for i in range(self.process_fetch_winid_iterations):
                    try:
                        processes = subprocess.check_output("wmctrl -lp | grep -i '" + self.filename + "'", shell=True).decode("utf-8").split("\n")
                        if len(processes):
                            print(i)
                            break
                    except:
                        pass

                    time.sleep(self.process_fetch_winid_sleep_time_ms)

                if len(processes) > 0:
                    temp = processes[0].split(" ")

                    for i in range(len(temp) - 1, -1, -1):
                        if temp[i] == '':
                            del temp[i]

                    winid, pid = temp[:3][::2]

                    self.process_winid = winid

                    if self.process_winid != "":
                        self.embed_file_standard_appliation_into_context(int(self.process_winid, 0))
                    else:
                        error = "Could not determine Window ID of requested application {0} for file {1}!".format(app, self.path)
                        self.on_open_entry_leave_recv()
                        raise Exception(error)

            self.is_open_entry_capability_blocked = True

        return 0

    def on_open_entry_leave_recv(self):
        if self.process is not None and self.is_open_entry_capability_blocked and self.process_winid is not "":
            if self.process_app_name in self.special_apps:
                subprocess.Popen(["wmctrl", "-c", self.special_apps[self.process_name]])
            else:
                self.process.terminate()

            self.destroy_embedded_window(int(self.process_winid, 0))

        self.pid = ""
        self.process_app_name = ""
        self.process_name = ""
        self.process_create_time = 0.0
        self.process = None
        self.process_winid = ""
        self.is_open_entry_capability_blocked = False

        return 0