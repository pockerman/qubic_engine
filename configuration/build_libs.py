import os
from pathlib import Path

from configuration.utils import remove_directory


def build_library(path: Path) -> None:

    if "build" in os.listdir(path=path):
        remove_directory(path=path, dirname="build")
        cmd = " cd {0}".format(path.parent)
        os.system(cmd)

    # that's the library directory
    str_cmd = "cd {0}".format(path)
    str_cmd += " && mkdir build && cd build"
    str_cmd += " && cmake .."
    str_cmd += " && make install"
    os.system(str_cmd)

    str_cmd = " cd {0}".format(path.parent)
    os.system(str_cmd)