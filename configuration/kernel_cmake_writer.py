import os
from pathlib import Path
from configuration import INFO
from configuration.build_libs import build_library
from configuration.build_examples import build_examples
from configuration.build_tests import build_tests
from configuration.cmake_file_writer import CMakeFileWriter


class KernelCMakeWriter(CMakeFileWriter):

    @staticmethod
    def dir_path() -> Path:
        current_dir = Path(os.getcwd())
        return current_dir / "kernel" / "kernel"

    @staticmethod
    def module_dirs() -> list:
        return ['base', 'data_structs', 'geometry', 'maths',
                'parallel', 'patterns', 'utilities']

    @staticmethod
    def module_name() -> str:
        return "kernel"

    def __init__(self, configuration: dict) -> None:
        super(KernelCMakeWriter, self).__init__(configuration=configuration,
                                                project_name="kernel",
                                                install_prefix=configuration["kernel"]["CMAKE_INSTALL_PREFIX"])

        self.dirs = KernelCMakeWriter.module_dirs()

    def write_cmake_lists(self):

        print("{0} Writing CMakeLists for project {1}".format(INFO, self.project_name))

        # set the kernel path
        current_dir = Path(os.getcwd())

        with open("kernel/kernel/CMakeLists.txt", 'w', newline="\n") as fh:

            fh = self.write_basic_lists(fh=fh)

            fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')
            for directory in self.dirs:
                fh.write('INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/%s/src/)\n' % (directory))

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                fh.write('INCLUDE_DIRECTORIES(${TRILINOS_INCL_DIR})\n')

            if self.configuration["opencv"]["USE_OPEN_CV"]:
                fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["opencv"]["OPENCV_INCL_DIR"]))

            if self.configuration["pytorch"]["USE_PYTORCH"]:
                fh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["pytorch"]["PYTORCH_INC_DIR"]))

            fh.write('INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})\n')
            fh.write('\n')
            fh.write('ADD_LIBRARY({0} SHARED "")\n'.format(self.project_name))
            fh.write('\n')

            # Add source directories we need to loop
            # over the submodules and write their CMakeLists
            for directory in self.dirs:
                fh.write('ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/%s/src/%s/%s/)\n' % (
                directory, self.project_name, directory))

                src_dir = current_dir / self.project_name / self.project_name / directory / 'src' / self.project_name / directory
                with open(src_dir / 'CMakeLists.txt', 'w', newline='\n') as local_fh:
                    dir_upper = directory.upper() + '_SRCS'
                    local_fh.write('IF(COMMAND cmake_policy)\n')
                    local_fh.write('\tCMAKE_POLICY(SET CMP0076 NEW)\n')
                    local_fh.write('ENDIF(COMMAND cmake_policy)\n')
                    local_fh.write('\n')
                    local_fh.write('FILE(GLOB %s ${PROJECT_SOURCE_DIR}/%s/src/%s/%s/*.cpp '
                                   '${PROJECT_SOURCE_DIR}/%s/src/%s/%s/*/*.cpp)\n' % (
                                   dir_upper, directory, self.project_name, directory,
                                   directory, self.project_name, directory))
                    local_fh.write('TARGET_SOURCES(%s PUBLIC ${%s})\n' % (self.project_name, dir_upper))

            fh.write('\n')
            fh.write('SET_TARGET_PROPERTIES({0} PROPERTIES LINKER_LANGUAGE CXX)\n'.format(self.project_name))
            fh.write('INSTALL(TARGETS %s DESTINATION ${CMAKE_INSTALL_PREFIX})\n' % self.project_name)
            fh.write('MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")\n')

        if self.configuration["BUILD_LIBS"]:
            print("{0} Building {1}".format(INFO, self.project_name))
            build_library(path=KernelCMakeWriter.dir_path())

        if self.configuration["kernel"]["BUILD_KERNEL_TESTS"]:
            self._write_tests_cmake()
            print("{0} Building tests for project {1}".format(INFO, KernelCMakeWriter.dir_path()))
            build_tests(path=KernelCMakeWriter.dir_path() / "tests")

        if self.configuration["kernel"]["BUILD_KERNEL_EXAMPLES"]:
            self._write_examples_cmake()
            print("{0} Building examples for project {1}".format(INFO, KernelCMakeWriter.dir_path()))
            build_examples(path=KernelCMakeWriter.dir_path() / "examples")

        print("{0} Done...".format(INFO))

    def _write_project_variables(self, fh):

        fh.write('SET(USE_DISCRETIZATION {0})\n'.format(self.configuration["kernel"]["USE_DISCRETIZATION"]))
        fh.write('SET(USE_RIGID_BODY_DYNAMICS {0})\n'.format(self.configuration["kernel"]["USE_NUMERICS"]))
        fh.write('SET(USE_NUMERICS {0})\n'.format(self.configuration["kernel"]["USE_RIGID_BODY_DYNAMICS"]))
        fh.write('SET(USE_FVM {0})\n'.format(self.configuration["kernel"]["USE_FVM"]))
        fh.write('SET(USE_FEM {0})\n'.format(self.configuration["kernel"]["USE_FEM"]))

        if self.configuration["CMAKE_BUILD_TYPE"]:
            fh.write('SET(KERNEL_DEBUG "ON")\n')

        if self.configuration["trilinos"]["USE_TRILINOS"]:
            fh.write('SET(USE_TRILINOS {0})\n'.format(self.configuration["trilinos"]["USE_TRILINOS"]))
            fh.write('SET(USE_TRILINOS_LONG_LONG_TYPE {0})\n'.format(
                self.configuration["trilinos"]["USE_TRILINOS_LONG_LONG_TYPE"]))
            fh.write('SET(TRILINOS_INCL_DIR {0})\n'.format(self.configuration["trilinos"]["TRILINOS_INCL_DIR"]))
            fh.write('SET(TRILINOS_LIB_DIR {0})\n'.format(self.configuration["trilinos"]["TRILINOS_LIB_DIR"]))

        if self.configuration["opencv"]["USE_OPEN_CV"]:
            fh.write('SET(USE_OPEN_CV {0})\n'.format(self.configuration["opencv"]["USE_OPEN_CV"]))

        if self.configuration["pytorch"]["USE_PYTORCH"]:
            fh.write('SET(USE_PYTORCH {0})\n'.format(self.configuration["pytorch"]["USE_PYTORCH"]))
            fh.write('SET(PYTORCH_INCL_DIR {0})\n'.format(self.configuration["pytorch"]["PYTORCH_INC_DIR"]))
            fh.write('SET(PYTORCH_LIB_DIR {0})\n'.format(self.configuration["pytorch"]["PYTORCH_LIB_DIR"]))

        current_dir = Path(os.getcwd())
        fh.write('SET(DATA_SET_FOLDER {0}/data)\n'.format(current_dir))
        fh.write('SET(TEST_DATA_DIR {0})\n'.format(current_dir / 'test_data'))

        return fh

    def _write_options(self, fh):
        return fh

    def _write_configure_files(self, fh):
        fh.write('configure_file(config.h.in ${PROJECT_SOURCE_DIR}/base/src/kernel/base/config.h @ONLY)\n')
        fh.write('configure_file(version.h.in ${PROJECT_SOURCE_DIR}/base/src/kernel/base/version.h @ONLY)\n')
        fh.write('\n')
        return fh

    def _write_single_cmake(self, path: Path, directory: str, example: bool) -> None:

        with open(path / "CMakeLists.txt", "w", newline="\n") as tfh:
            # set the kernel path
            current_dir = Path(os.getcwd())
            tfh.write("cmake_minimum_required(VERSION 3.0)\n")
            tfh.write("PROJECT({0} CXX)\n".format(directory))
            tfh.write("SET(SOURCE {0}.cpp)\n".format(directory))
            tfh.write("SET(EXECUTABLE  {0})\n".format(directory))

            tfh = self._find_boost(fh=tfh)
            tfh = self._find_blas(fh=tfh)
            tfh = self._write_build_option(fh=tfh, example=example)

            tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["BLAZE_INCL_DIR"]))
            tfh.write('INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})\n')

            for directory in self.dirs:
                tfh.write('INCLUDE_DIRECTORIES(%s/%s/src/)\n' % (current_dir / "kernel" / "kernel", directory))

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                tfh.write(
                    'INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["trilinos"]["TRILINOS_INCL_DIR"]))

            if self.configuration["opencv"]["USE_OPEN_CV"]:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["opencv"]["OPENCV_INCL_DIR"]))

            if self.configuration["pytorch"]["USE_PYTORCH"]:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["pytorch"]["PYTORCH_INC_DIR"]))

            if example is False:
                tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["testing"]["GTEST_INC_DIR"]))
            tfh.write('\n')

            if example:
                link_dirs = [self.configuration["kernel"]["CMAKE_INSTALL_PREFIX"],
                             "${Boost_LIBRARY_DIRS}"]
            else:
                link_dirs = [self.configuration["kernel"]["CMAKE_INSTALL_PREFIX"],
                             self.configuration["testing"]["GTEST_LIB_DIR"],
                             "${Boost_LIBRARY_DIRS}"]

            if self.configuration["trilinos"]["USE_TRILINOS"]:
                link_dirs.append(self.configuration["trilinos"]["TRILINOS_LIB_DIR"])

            if self.configuration["pytorch"]["USE_PYTORCH"]:
                link_dirs.append(self.configuration["pytorch"]["PYTORCH_LIB_DIR"])

            for link_dir in link_dirs:
                tfh.write("LINK_DIRECTORIES({0})\n".format(link_dir))

            tfh.write("\n")
            tfh.write('ADD_EXECUTABLE(%s %s)\n' % ("${EXECUTABLE}", "${SOURCE}"))
            tfh.write("\n")
            tfh.write('TARGET_LINK_LIBRARIES(%s %s)\n' % ("${EXECUTABLE}", self.project_name))

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

    def _write_examples_cmake(self) -> None:
        """
        Write the examples CMakeLists
        :return:
        """
        path = KernelCMakeWriter.dir_path() / "examples"
        self._write_multiple_cmakes(path=path, example=True)

    def _write_tests_cmake(self) -> None:
        """
        Write the CMakeLists for tests
        """

        path = KernelCMakeWriter.dir_path() / "tests"
        self._write_multiple_cmakes(path=path, example=False)
