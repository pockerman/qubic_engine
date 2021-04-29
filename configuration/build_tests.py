import os
from pathlib import Path
from configuration import INFO
from configuration.utils import remove_directory


def build_test(path: Path) -> None:
    # that's a test directory
    str_cmd = "cd {0}".format(path)

    if "build" in os.listdir(path=path):
        remove_directory(path=path, dirname="build")
        cmd = " cd {0}".format(path.parent)
        os.system(cmd)

    str_cmd += " && mkdir build && cd build"
    str_cmd += " && cmake .."
    str_cmd += " && make "
    os.system(str_cmd)

    str_cmd = " cd {0}".format(path.parent)
    os.system(str_cmd)


def build_tests(path: Path) -> None:

    # get all the directories starting with
    # test_
    directories = os.listdir(path=path)

    for directory in directories:
        directory_path = path / directory

        if directory_path.is_dir():
            if directory.startswith("test_"):
                print("{0} Building test {1}".format(INFO, directory_path))
                build_test(path=directory_path)

