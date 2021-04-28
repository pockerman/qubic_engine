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

    # make some logic tests required at the moment
    if config["kernel"]["USE_DISCRETIZATION"] is False and config["kernel"]["USE_NUMERICS"]:
        raise ValueError(" 'USE_DISCRETIZATION' is false but 'USE_NUMERICS' is true. Currently this is not supported."
                         "Turn both flags to True if you want kernel_numerics library to be built")

    # basic kernel
    kernel_cmake_writer = KernelCMakeWriter(configuration=config)
    kernel_cmake_writer.write_cmake_lists()

    if config["kernel"]["BUILD_KERNEL_TESTS"]:
        build_tests(path=KernelCMakeWriter.dir_path() / "tests")

    if config["kernel"]["BUILD_KERNEL_EXAMPLES"]:
        build_examples(path=KernelCMakeWriter.dir_path() / "examples")

    if config["kernel"]["USE_DISCRETIZATION"]:
        discretization_cmake_writer = DiscretizationCMakeWriter(configuration=config,
                                                                kernel_dirs=KernelCMakeWriter.module_dirs(),
                                                                kernel_dir=KernelCMakeWriter.dir_path(),
                                                                kernel_name=KernelCMakeWriter.module_name())
        discretization_cmake_writer.write_cmake_lists()

    if config["kernel"]["USE_NUMERICS"]:
        numerics_cmake_writer = NumericsCMakeWriter(configuration=config,
                                                    kernel_dirs=KernelCMakeWriter.module_dirs(),
                                                    kernel_dir=KernelCMakeWriter.dir_path(),
                                                    kernel_name=KernelCMakeWriter.module_name(),
                                                    discretization_dirs=DiscretizationCMakeWriter.module_dirs(),
                                                    discretization_dir=DiscretizationCMakeWriter.dir_path(),
                                                    discretization_name=DiscretizationCMakeWriter.module_name())
        numerics_cmake_writer.write_cmake_lists()

    if config["kernel"]["USE_RIGID_BODY_DYNAMICS"]:
        rd_dynamics_cmake_writer = RBDynamicsCMakeWriter(configuration=config,
                                                         kernel_dirs=KernelCMakeWriter.module_dirs(),
                                                         kernel_dir=KernelCMakeWriter.dir_path())
        rd_dynamics_cmake_writer.write_cmake_lists()

        if config["kernel"]["BUILD_KERNEL_TESTS"]:
            build_tests(path=RBDynamicsCMakeWriter.dir_path() / "tests")

        if config["kernel"]["BUILD_KERNEL_EXAMPLES"]:
            build_examples(path=RBDynamicsCMakeWriter.dir_path() / "examples")

    if config["cengine"]["build_cengine"]:

        cubic_cmake_writer = CMakeCubicEngineWriter(configuration=config,
                                                    kernel_name=KernelCMakeWriter.module_name(),
                                                    kernel_dir=KernelCMakeWriter.dir_path(),
                                                    kernel_dirs=KernelCMakeWriter.module_dirs())
        cubic_cmake_writer.write_cmake_lists()

        if config["cengine"]["rl"]["USE_RL"]:
            rl_cmake_writer = RLCMakeWriter(configuration=config,
                                            kernel_dir=KernelCMakeWriter.dir_path(),
                                            kernel_dirs=KernelCMakeWriter.module_dirs(),
                                            kernel_name=KernelCMakeWriter.module_name(),
                                            cengine_dir=CMakeCubicEngineWriter.dir_path(),
                                            cengine_dirs=CMakeCubicEngineWriter.module_dirs(),
                                            cengine_name=CMakeCubicEngineWriter.module_name())
            rl_cmake_writer.write_cmake_lists()

    print("{0} finished...".format(INFO))
