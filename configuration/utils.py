from pathlib import Path
import os


def remove_directory(path: Path, dirname: str) -> None:
    """
    Remove the directory in the Path
    """
    str_cmd = "cd {0}".format(path)
    str_cmd += " && rm -rf {0}".format(dirname)
    os.system(str_cmd)


def remove_file(path: Path, filename: str) -> None:
    os.system("rm {0}/{1}".format(path, filename))