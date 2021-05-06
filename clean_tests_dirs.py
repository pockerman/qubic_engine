from pathlib import Path
import os

from configuration import INFO
from configuration.utils import remove_directory, remove_file


def main(modules):
    current_dir = Path(os.getcwd())

    for module in modules:
        module_path = current_dir / module / "tests"

        directories = os.listdir(path=module_path)

        for directory in directories:
            example_dir = module_path / directory
            print("{0} cleaning tests {1}".format(INFO, example_dir))

            if "build" in os.listdir(path=example_dir):
                remove_directory(path=example_dir, dirname="build")

            if "CMakeLists.txt" in os.listdir(path=example_dir):
                remove_file(path=example_dir, filename="CMakeLists.txt")


if __name__ == '__main__':
    modules = ['kernel/numerics', 'kernel/discretization', 'kernel/kernel', 'cubic_engine/rl']
    main(modules=modules)
