import os

from xdg import BaseDirectory
from xdg import DesktopEntry
import subprocess


class __XDGError(Exception): pass


def xdg_query(command, parameter):
    p = subprocess.Popen(['xdg-mime', 'query', command, parameter], stdout=subprocess.PIPE, stderr=subprocess.PIPE)

    output, errors = p.communicate()

    if p.returncode or errors:
        raise __XDGError('xdg-mime returned error code %d: %s' % (p.returncode, errors.strip()))

    return output.strip()


def get_default(filename):
    try:
        desktop_filename = xdg_query('default', xdg_query('filetype', filename))
    except Exception:
        return None

    try:
        res = BaseDirectory.load_data_paths('applications', desktop_filename.decode("utf-8"))

        if not res:
            return None

        res = res.__next__()
    except StopIteration:
        return None

    if not os.path.exists(res):
        return None

    return DesktopEntry.DesktopEntry(res)