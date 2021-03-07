import json
from configuration import INFO
from configuration.cmake_file_writer import CMakeCubicEngineWriter


def read_json(filename):

    with open(filename) as json_file:
        json_input = json.load(json_file)
        return json_input


if __name__ == '__main__':
    print("{0} Generating configuration scripts...".format(INFO))
    config = read_json(filename="config_opts.json")
    cubic_camke_writer = CMakeCubicEngineWriter(configuration=config)
    cubic_camke_writer.write_cmake_lists()
    print("{0} finished...".format(INFO))
