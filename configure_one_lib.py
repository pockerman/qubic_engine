import json
from configuration import INFO
from configuration.cengine_cmake_writer import CMakeCubicEngineWriter
from configuration.kernel_cmake_writer import KernelCMakeWriter
from configuration.numerics_cmake_writer import NumericsCMakeWriter
from configuration.discretization_cmake_writer import DiscretizationCMakeWriter
from configuration.rigid_body_dynamics_cmake_writer import RBDynamicsCMakeWriter
from configuration.cengine_rl_cmake_writer import RLCMakeWriter
from configuration.build_tests import build_tests
from configuration.build_examples import build_examples


def read_json(filename):

    with open(filename) as json_file:
        json_input = json.load(json_file)
        return json_input


if __name__ == '__main__':
    print("{0} Generating configuration scripts...".format(INFO))
    config = read_json(filename="config_opts.json")

    rl_cmake_writer = NumericsCMakeWriter(configuration=config,
                                          kernel_dirs=KernelCMakeWriter.module_dirs(),
                                          kernel_dir=KernelCMakeWriter.dir_path(),
                                          kernel_name=KernelCMakeWriter.module_name(),
                                          discretization_dirs=DiscretizationCMakeWriter.module_dirs(),
                                          discretization_dir=DiscretizationCMakeWriter.dir_path(),
                                          discretization_name=DiscretizationCMakeWriter.module_name())
    rl_cmake_writer.write_cmake_lists()
    print("{0} finished...".format(INFO))
