import os
from pathlib import Path


def build_example(path: Path) -> None:
    # that's a test directory
    str_cmd = "cd {0}".format(path)

    if "build" in os.listdir(path=path):
        str_cmd += " && rm -rf build"
        os.system(str_cmd)
    os.system("rm {0}/CMakeLists.txt".format(path))


    #str_cmd += " && mkdir build && cd build"
    #str_cmd += " && cmake .."
    #str_cmd += " && make "

    #os.system(str_cmd)
    str_cmd = " cd {0}".format(path.parent)
    os.system(str_cmd)


def build_examples(path: Path) -> None:

    # get all the directories starting with
    # test_
    directories = os.listdir(path=path)

    for directory in directories:
        directory_path = path / directory

        if directory_path.is_dir():
            if directory.startswith("example_"):
                build_example(path=directory_path)

