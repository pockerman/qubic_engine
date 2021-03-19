import os
from pathlib import Path
from configuration import INFO
from configuration.cmake_file_writer import CMakeFileWriter


class KernelCMakeWriter(CMakeFileWriter):

    @staticmethod
    def kernel_dir_path():
        current_dir = Path(os.getcwd())
        return current_dir / "kernel" / "kernel"

    @staticmethod
    def kernel_dirs():
        return ['base', 'data_structs', 'geometry', 'maths',
                'parallel', 'patterns', 'utilities']

    def __init__(self, configuration: dict) -> None:
        super(KernelCMakeWriter, self).__init__(configuration=configuration,
                                                project_name="kernel",
                                                install_prefix=configuration["kernel"]["CMAKE_INSTALL_PREFIX"])

        self.dirs = KernelCMakeWriter.kernel_dirs()

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

        if self.configuration["kernel"]["BUILD_KERNEL_TESTS"]:
            self._write_tests_cmake()

        print("{0} Done...".format(INFO))

    def _write_project_variables(self, fh):

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

        current_dir = Path(os.getcwd())
        fh.write('SET(DATA_SET_FOLDER {0}/data)\n'.format(current_dir))

        return fh

    def _write_options(self, fh):
        return fh

    def _write_configure_files(self, fh):
        fh.write('configure_file(config.h.in ${PROJECT_SOURCE_DIR}/base/src/kernel/base/config.h @ONLY)\n')
        fh.write('configure_file(version.h.in ${PROJECT_SOURCE_DIR}/base/src/kernel/base/version.h @ONLY)\n')
        fh.write('\n')
        return fh

    def _write_tests_cmake(self):
        """
        Write the CMakeLists for tests
        """

        # set the kernel path
        current_dir = Path(os.getcwd())

        # get the test directories
        test_path = current_dir / "kernel/kernel/tests"
        test_dirs = os.listdir(test_path)

        for test_dir in test_dirs:
            test_dir_path = Path(test_path / test_dir)

            if test_dir_path.is_dir():

                with open(current_dir / "kernel/kernel/tests" / test_dir / "CMakeLists.txt", "w", newline="\n") as tfh:
                    tfh.write("cmake_minimum_required(VERSION 3.0)\n")
                    tfh.write("PROJECT({0} CXX)\n".format(test_dir))
                    tfh.write("SET(SOURCE test.cpp)\n")
                    tfh.write("SET(EXECUTABLE  {0})\n".format(test_dir))

                    tfh = self._find_boost(fh=tfh)
                    tfh = self._find_blas(fh=tfh)
                    tfh = self._write_build_option(fh=tfh)

                    tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["BLAZE_INCL_DIR"]))
                    for directory in self.dirs:
                        tfh.write('INCLUDE_DIRECTORIES(%s/%s/src/)\n' % (current_dir / "kernel" / "kernel", directory))

                    if self.configuration["trilinos"]["USE_TRILINOS"]:
                        tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["trilinos"]["TRILINOS_INCL_DIR"]))

                    if self.configuration["opencv"]["USE_OPEN_CV"]:
                        tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["opencv"]["OPENCV_INCL_DIR"]))

                    tfh.write('INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})\n')
                    tfh.write('INCLUDE_DIRECTORIES({0})\n'.format(self.configuration["testing"]["GTEST_INC_DIR"]))
                    tfh.write('\n')

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
                    tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} gtest)\n')
                    tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} gtest_main) '
                              '# so that tests dont need to have a main\n')
                    tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} pthread)\n')
                    tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)\n')

                    if self.configuration["trilinos"]["USE_TRILINOS"]:
                        libs = ["epetra", "aztecoo", "amesos"]
                        for lib in libs:
                            tfh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} %s)\n' % lib)
