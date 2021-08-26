from pathlib import Path
import os
from configuration.cmake_file_writer import CMakeFileWriter
from configuration.build_libs import build_library
from configuration.build_examples import build_examples
from configuration.build_tests import build_tests
from configuration import INFO


class MLCMakeWriter(CMakeFileWriter):

    @staticmethod
    def dir_path() -> Path:
        current_dir = Path(os.getcwd())
        return current_dir / "cubic_engine" / "ml"

    @staticmethod
    def module_dirs() -> list:
        return ['instance_learning', 'neural_networks',
                'supervised_learning', 'unsupervised_learning', 'objective_functions' ]

    @staticmethod
    def module_name() -> str:
        return "cengine_ml"

    def __init__(self, configuration: dict, kernel_dir: Path, kernel_dirs: list,
                 kernel_name: str,  cengine_dir: Path, cengine_dirs: list, cengine_name: str,
                 numerics_name: str, numerics_dir: Path, numerics_dirs: list) -> None:
        super(MLCMakeWriter, self).__init__(configuration=configuration,
                                            project_name=MLCMakeWriter.module_name(),
                                            install_prefix=configuration["cengine"]["CMAKE_INSTALL_PREFIX"])

        self.kernel_dirs = kernel_dirs
        self.kernel_dir = kernel_dir
        self.kernel_name = kernel_name
        self.numerics_name = numerics_name
        self.numerics_dir = numerics_dir
        self.numerics_dirs = numerics_dirs
        self.cengine_dirs = cengine_dirs
        self.cengine_dir = cengine_dir
        self.cengine_name = cengine_name
        self.dirs = MLCMakeWriter.module_dirs()

    def write_cmake_lists(self):

        print("{0} Writing CMakeLists for project {1}".format(INFO, self.project_name))

        # set the kernel path
        current_dir = Path(os.getcwd())
        with open(current_dir / "cubic_engine" / "ml" / "CMakeLists.txt", 'w', newline="\n") as fh:

            # write the basic set variables
            fh = self.write_basic_lists(fh=fh)

            fh.write('\n')
            fh.write('ADD_LIBRARY({0} SHARED "")\n'.format(self.project_name))
            fh.write('\n')

            src_dir = current_dir / "cubic_engine" / "ml" / "src" / "cubic_engine" / "ml"
            fh.write('ADD_SUBDIRECTORY(%s)\n' % src_dir)

            with open(src_dir / 'CMakeLists.txt', 'w', newline='\n') as local_fh:
                dir_upper = 'SRCS'
                local_fh.write('IF(COMMAND cmake_policy)\n')
                local_fh.write('\tCMAKE_POLICY(SET CMP0076 NEW)\n')
                local_fh.write('ENDIF(COMMAND cmake_policy)\n')
                local_fh.write('\n')
                local_fh.write('FILE(GLOB %s ${PROJECT_SOURCE_DIR}/src/cubic_engine/ml/*.cpp '
                               '${PROJECT_SOURCE_DIR}/src/cubic_engine/ml/*/*.cpp '
                               '${PROJECT_SOURCE_DIR}/src/cubic_engine/ml/instance_learning/details/*.cpp)\n' % dir_upper)
                local_fh.write('TARGET_SOURCES(%s PUBLIC ${%s})\n' % (self.project_name, dir_upper))

            fh.write('\n')
            fh.write('SET_TARGET_PROPERTIES({0} PROPERTIES LINKER_LANGUAGE CXX)\n'.format(self.project_name))
            fh.write('INSTALL(TARGETS %s DESTINATION ${CMAKE_INSTALL_PREFIX})\n' % self.project_name)
            fh.write('MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")\n')

        if self.configuration["BUILD_LIBS"]:
            print("{0} Building {1}".format(INFO, self.project_name))
            build_library(path=MLCMakeWriter.dir_path())

        if self.configuration["cengine"]["ml"]["BUILD_TESTS"]:

            if self.configuration["zmq"]["USE_ZMQ"]:
                cmd = " export LD_LIBRARY_PATH={0}".format(self.configuration["zmq"]["ZMQ_LIB_PATH"])
                os.system(cmd)
            self._write_tests_cmake()
            print("{0} Building tests for project {1}".format(INFO, MLCMakeWriter.dir_path()))
            build_tests(path=MLCMakeWriter.dir_path() / "tests" )

        if self.configuration["cengine"]["rl"]["BUILD_EXAMPLES"]:

            if self.configuration["zmq"]["USE_ZMQ"]:
                cmd = " export LD_LIBRARY_PATH={0}".format(self.configuration["zmq"]["ZMQ_LIB_PATH"])
                os.system(cmd)

            self._write_examples_cmake()
            print("{0} Building examples for project {1}".format(INFO, MLCMakeWriter.dir_path()))
            build_examples(path=MLCMakeWriter.dir_path() / "examples")

        print("{0} Done...".format(INFO))

    def _write_configure_files(self, fh):
        # library uses kernel/config.h and cengine/config.h
        return fh

    def _write_project_variables(self, fh):
        # library uses cengine project variables
        return fh

    def _write_single_cmake(self, path: Path, directory: str, example: bool) -> None:

        with open(path / "CMakeLists.txt", "w", newline="\n") as tfh:

            # set the kernel path
            tfh.write("cmake_minimum_required(VERSION 3.0)\n")
            tfh.write("PROJECT({0} CXX)\n".format(directory))
            tfh.write("SET(SOURCE {0}.cpp)\n".format(directory))
            tfh.write("SET(EXECUTABLE  {0})\n".format(directory))

            tfh = self._find_boost(fh=tfh)
            tfh = self._find_blas(fh=tfh)
            tfh = self._find_pytorch(fh=tfh)
            tfh = self._write_build_option(fh=tfh, example=example)
            tfh = self._write_includes(fh=tfh)

            if example is False:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["testing"]["GTEST_INC_DIR"]))

            tfh.write('\n')

            link_dirs = [self.configuration["kernel"]["CMAKE_INSTALL_PREFIX"],
                         self.configuration["cengine"]["CMAKE_INSTALL_PREFIX"],
                         "${Boost_LIBRARY_DIRS}", ]

            if example is False:
                link_dirs.append(self.configuration["testing"]["GTEST_LIB_DIR"])

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                link_dirs.append(self.configuration["trilinos"]["TRILINOS_LIB_DIR"])

            if self.configuration["zmq"]["USE_ZMQ"]:
                link_dirs.append(self.configuration["zmq"]["ZMQ_LIB_PATH"])

            for link_dir in link_dirs:
                tfh.write("LINK_DIRECTORIES({0})\n".format(link_dir))

            tfh.write("\n")
            tfh.write('ADD_EXECUTABLE(%s %s)\n' % ("${EXECUTABLE}", "${SOURCE}"))
            tfh.write("\n")
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.project_name))
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.cengine_name))
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.kernel_name))
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.numerics_name))

            if example is False:
                tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} gtest)\n')
                tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} gtest_main) '
                          '# so that tests dont need to have a main\n')

            if self.configuration["pytorch"]["USE_PYTORCH"]:
                tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} ${TORCH_LIBRARIES})\n')

            if self.configuration["zmq"]["USE_ZMQ"]:
                tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} zmq)\n')

            tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} pthread)\n')
            tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)\n')

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                libs = ["epetra", "aztecoo", "amesos"]
                for lib in libs:
                    tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} %s)\n' % lib)

    def _write_include_files(self, fh):
        fh.write('INCLUDE_DIRECTORIES(%s/src/)\n' % MLCMakeWriter.dir_path())
        fh.write('INCLUDE_DIRECTORIES({0}/src/)\n'.format(self.cengine_dir))
        fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["msgpack"]["MSGPACK_INCL_DIR"]))
        fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.numerics_dir / 'src'))

        if self.configuration["zmq"]["USE_ZMQ"]:
            fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["zmq"]["ZMQ_INCLUDE_PATH"]))

        for kdir in self.kernel_dirs:
            fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.kernel_dir / kdir / 'src'))

        #for numericsdir in self.numerics_dirs:
        #    fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.numerics_dir / numericsdir / 'src'))

        return fh

    def _write_multiple_cmakes(self, path: Path, example: bool) -> None:

        # get the test directories
        working_path = path
        working_dirs = os.listdir(working_path)

        for w_dir in working_dirs:
            w_dir_path = Path(working_path / w_dir)

            if w_dir_path.is_dir():
                self._write_single_cmake(path=w_dir_path, directory=w_dir, example=example)

    def _write_examples_cmake(self) -> None:
        """
        Write the examples CMakeLists
        :return:
        """

        # get the test directories
        examples_path = MLCMakeWriter.dir_path() / "examples"
        self._write_multiple_cmakes(path=examples_path, example=True)

    def _write_tests_cmake(self) -> None:
        """
        Write the CMakeLists for tests
        """

        # get the test directories
        path = MLCMakeWriter.dir_path() / "tests"
        self._write_multiple_cmakes(path=path, example=False)
