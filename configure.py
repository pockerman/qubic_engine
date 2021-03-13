import json
from configuration import INFO
from configuration.cmake_file_writer import CMakeCubicEngineWriter
from configuration.kernel_cmake_writer import KernelCMakeWriter


def read_json(filename):

    with open(filename) as json_file:
        json_input = json.load(json_file)
        return json_input


if __name__ == '__main__':
    print("{0} Generating configuration scripts...".format(INFO))
    config = read_json(filename="config_opts.json")
    kernel_cmake_writer = KernelCMakeWriter(configuration=config)
    kernel_cmake_writer.write_cmake_lists()
    #cubic_camke_writer = CMakeCubicEngineWriter(configuration=config)
    #cubic_camke_writer.write_cmake_lists()
    print("{0} finished...".format(INFO))
