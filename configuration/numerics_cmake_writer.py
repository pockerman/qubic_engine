import os
from pathlib import Path
from configuration import INFO
from configuration.build_libs import build_library
from configuration.build_examples import build_examples
from configuration.build_tests import build_tests
from configuration.cmake_file_writer import CMakeFileWriter


class NumericsCMakeWriter(CMakeFileWriter):

    @staticmethod
    def dir_path():
        current_dir = Path(os.getcwd())
        return current_dir / "kernel" / "numerics"

    @staticmethod
    def module_dirs():
        return ['statistics', 'direct_solvers', 'fvm', 'optimization',
                'solvers', 'krylov_solvers']

    @staticmethod
    def module_name() -> str:
        return "kernel_numerics"

    def __init__(self, configuration: dict, kernel_dirs: list, kernel_dir: Path,
                 kernel_name: str,
                 discretization_dirs: list, discretization_dir: Path, discretization_name: str) -> None:
        super(NumericsCMakeWriter, self).__init__(configuration=configuration,
                                                  project_name="kernel_numerics",
                                                  install_prefix=configuration["kernel"]["CMAKE_INSTALL_PREFIX"])

        self.kernel_dirs = kernel_dirs
        self.kernel_dir = kernel_dir
        self.kernel_name = kernel_name
        self.discretization_dirs = discretization_dirs
        self.discretization_dir = discretization_dir
        self.discretization_name = discretization_name
        self.dirs = NumericsCMakeWriter.module_dirs()

    def write_cmake_lists(self):
        """
        Write CMakeLists for the project
        """
        print("{0} Writing CMakeLists for project {1}".format(INFO, self.project_name))
        current_dir = Path(os.getcwd())

        with open("kernel/numerics/CMakeLists.txt", 'w', newline="\n") as fh:

            fh = self.write_basic_lists(fh=fh)
            fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/src/)\n')
            fh.write('INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${NLOHMANN_JSON_INCL_DIR})\n')
            fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.discretization_dir / 'src'))

            # include dirs for kernel
            for kdir in self.kernel_dirs:
                fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.kernel_dir / kdir / 'src'))

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                fh.write('INCLUDE_DIRECTORIES(${TRILINOS_INCL_DIR})\n')
                fh.write('SET(TRILINOS_LIB_DIR {0})\n'.format(self.configuration["trilinos"]["TRILINOS_LIB_DIR"]))

            # OpenCV includes
            if self.configuration["opencv"]["USE_OPEN_CV"]:
                fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["opencv"]["OPENCV_INCL_DIR"]))

            fh.write('\n')
            fh.write('ADD_LIBRARY({0} SHARED "")\n'.format(self.project_name))
            fh.write('\n')

            src_dir = current_dir / "kernel" / "numerics" / "src" / "kernel" / "numerics"
            fh.write('ADD_SUBDIRECTORY(%s)\n' % src_dir)

            # write CMakeLists in the src/ directory to collect
            # the sources for the project
            with open(src_dir / 'CMakeLists.txt', 'w', newline='\n') as local_fh:
                local_fh.write('IF(COMMAND cmake_policy)\n')
                local_fh.write('\tCMAKE_POLICY(SET CMP0076 NEW)\n')
                local_fh.write('ENDIF(COMMAND cmake_policy)\n')
                local_fh.write('\n')
                local_fh.write('FILE(GLOB SRCS %s/*.cpp '
                               '%s/*/*.cpp)\n' % (src_dir, src_dir))
                local_fh.write('TARGET_SOURCES(%s PUBLIC ${%s})\n' % (self.project_name, 'SRCS'))

            fh.write('SET_TARGET_PROPERTIES({0} PROPERTIES LINKER_LANGUAGE CXX)\n'.format(self.project_name))
            fh.write('INSTALL(TARGETS %s DESTINATION ${CMAKE_INSTALL_PREFIX})\n' % self.project_name)
            fh.write('MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")\n')

        if self.configuration["BUILD_LIBS"]:
            print("{0} Building {1}".format(INFO, self.project_name))
            build_library(path=NumericsCMakeWriter.dir_path())

        if self.configuration["kernel"]["BUILD_KERNEL_TESTS"]:
            self._write_tests_cmake()
            build_tests(path=NumericsCMakeWriter.dir_path() / "tests")

        if self.configuration["kernel"]["BUILD_KERNEL_EXAMPLES"]:
            self._write_examples_cmake()
            build_examples(path=NumericsCMakeWriter.dir_path() / "examples")

        print("{0} Done...".format(INFO))

    def _write_configure_files(self, fh):
        # Do nothing as the project will use kernel/base/config.h
        return fh

    def _write_project_variables(self, fh):

        fh.write('SET(USE_FVM {0})\n'.format(self.configuration["kernel"]["USE_FVM"]))
        fh.write('SET(USE_FEM {0})\n'.format(self.configuration["kernel"]["USE_FEM"]))

        if self.configuration["trilinos"]["USE_TRILINOS"]:
            fh.write('SET(USE_TRILINOS {0})\n'.format(self.configuration["trilinos"]["USE_TRILINOS"]))
            fh.write('SET(USE_TRILINOS_LONG_LONG_TYPE {0})\n'.format(
                self.configuration["trilinos"]["USE_TRILINOS_LONG_LONG_TYPE"]))
            fh.write('SET(TRILINOS_INCL_DIR {0})\n'.format(self.configuration["trilinos"]["TRILINOS_INCL_DIR"]))
            fh.write('SET(TRILINOS_LIB_DIR {0})\n'.format(self.configuration["trilinos"]["TRILINOS_LIB_DIR"]))

        if self.configuration["opencv"]["USE_OPEN_CV"]:
            fh.write('SET(USE_OPEN_CV {0})\n'.format(self.configuration["opencv"]["USE_OPEN_CV"]))

        current_dir = Path(os.getcwd())
        fh.write('SET(DATA_SET_FOLDER {0}/data)\n'.format(current_dir))
        fh.write('SET(TEST_DATA_DIR {0})\n'.format(current_dir / 'test_data'))

        return fh

    def _write_single_cmake(self, path: Path, directory: str, example: bool) -> None:

        with open(path / "CMakeLists.txt", "w", newline="\n") as tfh:

            tfh.write("cmake_minimum_required(VERSION 3.0)\n")
            tfh.write("PROJECT({0} CXX)\n".format(directory))
            tfh.write("SET(SOURCE {0}.cpp)\n".format(directory))
            tfh.write("SET(EXECUTABLE  {0})\n".format(directory))

            tfh = self._find_boost(fh=tfh)
            tfh = self._find_blas(fh=tfh)
            tfh = self._write_build_option(fh=tfh, example=example)

            tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["BLAZE_INCL_DIR"]))
            tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.discretization_dir / "src"))
            tfh.write('INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})\n')
            tfh.write('INCLUDE_DIRECTORIES(%s/src/)\n' % NumericsCMakeWriter.dir_path())

            # include the kernel dirs
            for kdir in self.kernel_dirs:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.kernel_dir / kdir / 'src'))

            # Trilinos includes
            if self.configuration["trilinos"]["USE_TRILINOS"]:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["trilinos"]["TRILINOS_INCL_DIR"]))

            if self.configuration["opencv"]["USE_OPEN_CV"]:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["opencv"]["OPENCV_INCL_DIR"]))

            if example is False:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["testing"]["GTEST_INC_DIR"]))
            tfh.write('\n')

            if example is False:
                link_dirs = [self.configuration["kernel"]["CMAKE_INSTALL_PREFIX"],
                             "${Boost_LIBRARY_DIRS}"]
            else:
                link_dirs = [self.configuration["kernel"]["CMAKE_INSTALL_PREFIX"],
                             self.configuration["testing"]["GTEST_LIB_DIR"],
                             "${Boost_LIBRARY_DIRS}"]

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                link_dirs.append(self.configuration["trilinos"]["TRILINOS_LIB_DIR"])

            for link_dir in link_dirs:
                tfh.write("LINK_DIRECTORIES({0})\n".format(link_dir))

            tfh.write("\n")
            tfh.write('ADD_EXECUTABLE(%s %s)\n' % ("${EXECUTABLE}", "${SOURCE}"))
            tfh.write("\n")
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.project_name))
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.kernel_name))
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.discretization_name))

            if example is False:
                tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} gtest)\n')
                tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} gtest_main) '
                          '# so that tests dont need to have a main\n')
            tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} pthread)\n')
            tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)\n')

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                libs = ["epetra", "aztecoo", "amesos"]
                for lib in libs:
                    tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} %s)\n' % lib)

    def _write_multiple_cmakes(self, path: Path, example: bool) -> None:

        # get the test directories
        working_path = path
        working_dirs = os.listdir(working_path)

        for w_dir in working_dirs:
            w_dir_path = Path(working_path / w_dir)

            if w_dir_path.is_dir():
                self._write_single_cmake(path=w_dir_path, directory=w_dir, example=example)

    def _write_examples_cmake(self):
        """
        Write the examples CMakeLists
        :return:
        """

        examples_path = NumericsCMakeWriter.dir_path() / "examples"
        self._write_multiple_cmakes(path=examples_path, example=True)

    def _write_tests_cmake(self):
        """
        Write the CMakeLists for tests
        """

        # get the test directories
        path = NumericsCMakeWriter.dir_path() / "tests"
        self._write_multiple_cmakes(path=path, example=False)
