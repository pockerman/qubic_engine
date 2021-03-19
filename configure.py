import json
from configuration import INFO
from configuration.cmake_file_writer import CMakeCubicEngineWriter
from configuration.kernel_cmake_writer import KernelCMakeWriter
from configuration.numerics_cmake_writer import NumericsCMakeWriter
from configuration.discretization_cmake_writer import DiscretizationCMakeWriter
from configuration.rigid_body_dynamics_cmake_writer import RBDynamicsCMakeWriter

def read_json(filename):

    with open(filename) as json_file:
        json_input = json.load(json_file)
        return json_input


if __name__ == '__main__':
    print("{0} Generating configuration scripts...".format(INFO))
    config = read_json(filename="config_opts.json")

    # make some logic tests required at the moment
    if config["kernel"]["USE_DISCRETIZATION"] is False and config["kernel"]["USE_NUMERICS"]:
        raise ValueError(" 'USE_DISCRETIZATION' is false but 'USE_NUMERICS' is true. Currently this is not supported."
                         "Turn both flags to True if you want kernel_numerics library to be built")

    # basic kernel
    kernel_cmake_writer = KernelCMakeWriter(configuration=config)
    kernel_cmake_writer.write_cmake_lists()

    if config["kernel"]["USE_DISCRETIZATION"]:
        discretization_cmake_writer = DiscretizationCMakeWriter(configuration=config,
                                                                kernel_dirs=KernelCMakeWriter.kernel_dirs(),
                                                                kernel_dir=KernelCMakeWriter.kernel_dir_path())
        discretization_cmake_writer.write_cmake_lists()

    if config["kernel"]["USE_NUMERICS"]:
        numerics_cmake_writer = NumericsCMakeWriter(configuration=config,
                                                    kernel_dirs=KernelCMakeWriter.kernel_dirs(),
                                                    kernel_dir=KernelCMakeWriter.kernel_dir_path())
        numerics_cmake_writer.write_cmake_lists()

    if config["kernel"]["USE_RIGID_BODY_DYNAMICS"]:
        rd_dynamics_cmake_writer = RBDynamicsCMakeWriter(configuration=config,
                                                         kernel_dirs=KernelCMakeWriter.kernel_dirs(),
                                                         kernel_dir=KernelCMakeWriter.kernel_dir_path())
        rd_dynamics_cmake_writer.write_cmake_lists()

    #cubic_camke_writer = CMakeCubicEngineWriter(configuration=config)
    #cubic_camke_writer.write_cmake_lists()
    print("{0} finished...".format(INFO))
