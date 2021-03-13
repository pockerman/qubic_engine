import os
from pathlib import Path

from configuration import INFO


class CMakeFileWriter(object):
    def __init__(self, configuration, project_name):
        self._configuration = configuration
        self._project_name = project_name

        if self._project_name == "CubicEngine":
            self._main_cmake = 'cubic_engine/CMakeLists_test.txt'

    @property
    def configuration(self):
        return self._configuration

    @property
    def project_name(self):
        return self._project_name

    def write_basic_lists(self, fh, **options):

            # start creating main CMakeLists
            fh.write("CMAKE_MINIMUM_REQUIRED(VERSION 3.6)\n")
            fh.write('MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")\n')
            fh.write('\n')
            fh.write('PROJECT({0} CXX)\n'.format(self._project_name))
            fh.write('\n')
            fh.write('# -----------------------------------------------------------------------------\n')
            fh.write('# Prevent in-source builds.\n')
            fh.write('# -----------------------------------------------------------------------------\n')
            fh.write('\n')
            fh.write('IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})\n')
            fh.write('\tMESSAGE( FATAL_ERROR  "In-source build is not possible. Choose an empty directory for build output.")\n')
            fh.write('ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})\n')

            fh.write('\n')
            fh.write('# Be sure to avoid troubles with library paths when using old policy\n')
            fh.write('IF(COMMAND cmake_policy)\n')
            fh.write('\tCMAKE_POLICY(SET CMP0003 NEW)\n')
            fh.write('\tCMAKE_POLICY(SET CMP0048 NEW)\n')
            fh.write('ENDIF(COMMAND cmake_policy)\n')

            fh.write('\n')
            fh.write('# default options')
            fh.write('SET(BUILD_SHARED_LIBS ON)\n')

            build_type = self.configuration["CMAKE_BUILD_TYPE"]
            fh.write('SET(CMAKE_BUILD_TYPE "{0}")\n'.format(build_type))
            fh.write('SET(CMAKE_CXX_STANDARD 17)\n')
            fh.write('SET(CMAKE_CXX_STANDARD_REQUIRED True)\n')
            fh.write('SET(PWD ${PROJECT_SOURCE_DIR})\n')
            fh.write('SET(CMAKE_INSTALL_PREFIX ${PWD}/install)\n')
            fh.write('SET(MAGIC_ENUM_INCL_DIR " ")\n')
            fh.write('SET(CMAKE_LINKER_FLAGS "-pthread")\n')

            blaze_dir = self.configuration["BLAZE_INCL_DIR"]
            fh.write('SET(BLAZE_INCL_DIR "{0}")\n'.format(blaze_dir))

            nlohmann_json_dir = self.configuration["NLOHMANN_JSON_INCL_DIR"]
            fh.write('SET(NLOHMANN_JSON_INCL_DIR "{0}")\n'.format(nlohmann_json_dir))
            fh = self._write_project_variables(fh=fh)

            fh = self._find_boost(fh=fh)
            fh = self._find_blas(fh=fh)
            fh = self._write_options(fh=fh)
            fh = self._write_configure_files(fh=fh)

            return fh

    def write_test_cmake_lists(self):
        pass

    def _write_project_variables(self, fh):
        return fh

    def _write_build_option(self, fh):

        if self.configuration["USE_OPENMP"]:
            if self.configuration["CMAKE_BUILD_TYPE"]:
                fh.write('SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fopenmp -fPIC")\n')
                fh.write('SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)\n')
            else:
                fh.write('SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread -fopenmp -fPIC")\n')
                fh.write('SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)\n')
        else:

            if self.configuration["CMAKE_BUILD_TYPE"]:
                fh.write('SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fPIC")\n')
                fh.write('SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)\n')
            else:
                fh.write('SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread -fPIC")\n')
                fh.write('SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)\n')

        return fh

    def _find_boost(self, fh):
        fh.write('\n')
        fh.write('# find Boost\n')
        fh.write('FIND_PACKAGE(Boost 1.65.0 REQUIRED)\n')
        fh.write('IF(Boost_FOUND)\n')
        fh.write('\tMESSAGE( STATUS  "Found needed Boost C++ library.")\n')
        #fh.write('\tSET(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})\n')
        #fh.write('\tSET(BOOST_LIBRARYDIR ${Boost_LIBRARY_DIRS})\n')
        fh.write('\tSET(Boost_USE_SHARED_LIBS ON)\n')
        fh.write('ELSE()\n')
        fh.write('\tMESSAGE( FATAL_ERROR  "Boost C++ library is required but not found.")\n')
        fh.write('ENDIF()\n')
        fh.write('\n')
        return fh

    def _find_blas(self, fh):
        fh.write('\n')
        fh.write('# find OpenBLAS\n')
        fh.write('FIND_PACKAGE(BLAS REQUIRED)\n')
        fh.write('IF(NOT BLAS_FOUND)\n')
        fh.write('\tMESSAGE( FATAL_ERROR  "Could not find OpenBLAS library.")\n')
        fh.write('ELSE()\n')
        fh.write('\tMESSAGE( STATUS  "Found needed BLAS library.")\n')
        fh.write('ENDIF()\n')
        fh.write('\n')
        return fh

    def _write_include_files(self, fh):
        return fh

    def _write_options(self, fh):
        return fh

    def _write_opencv(self, fh):
        pass

    def _write_configure_files(self, fh):
        return fh


class CMakeCubicEngineWriter(CMakeFileWriter):

    def __init__(self, configuration: dict) -> None:
        super(CMakeCubicEngineWriter, self).__init__(configuration=configuration,
                                                     project_name="CubicEngine")

    def write_cmake_lists(self):

        if self.configuration["testing"]:
            self.write_test_cmake_lists()

        if self.configuration["cengine"]["rl"]["USE_RL"]:

            print("{0} RL module is enabled...".format(INFO))
            self.write_rl_cmake_lists()

            if self.configuration["cengine"]["rl"]["BUILD_EXAMPLES"]:
                print("{0} RL module examples are enabled...".format(INFO))
                self.write_rl_cmake_examples()

            if self.configuration["cengine"]["rl"]["BUILD_TESTS"]:
                print("{0} RL module tests are enabled...".format(INFO))
                self.write_rl_cmake_tests()

    def write_rl_cmake_lists(self):

        with open("cubic_engine/rl/CMakeLists.txt", 'w', newline="\n") as fh:

            fh = self.write_basic_lists(fh, **{"project_name": "cubic_rl_engine"})
            fh = self._find_boost(fh=fh)
            fh = self._find_blas(fh=fh)


            # write require attributes
            # required configuration
            fh.write('\n')
            fh.write('# default options')
            fh.write('SET(BUILD_SHARED_LIBS ON)\n')

            build_type = self.configuration["CMAKE_BUILD_TYPE"]
            fh.write('SET(CMAKE_BUILD_TYPE "{0}")\n'.format(build_type))
            fh.write('SET(CMAKE_CXX_STANDARD 17)\n')
            fh.write('SET(CMAKE_CXX_STANDARD_REQUIRED True)\n')
            fh.write('SET(PWD ${PROJECT_SOURCE_DIR})\n')
            fh.write('SET(CMAKE_INSTALL_PREFIX ${PWD} / install)\n')
            fh.write('SET(MAGIC_ENUM_INCL_DIR " ")\n')
            fh.write('SET(CMAKE_LINKER_FLAGS "-pthread")\n')

            blaze_dir = self.configuration["BLAZE_INCL_DIR"]
            fh.write('SET(BLAZE_INCL_DIR "{0}")\n'.format(blaze_dir))

            nlohmann_json_dir = self.configuration["NLOHMANN_JSON_INCL_DIR"]
            fh.write('SET(NLOHMANN_JSON_INCL_DIR "{0}")\n'.format(nlohmann_json_dir))

            # we want the include dir to have dir cubic_engine/
            # so that include files have have the format
            # cubic_engine/rl/.h
            fh.write('SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/src)\n')
            fh.write('SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/src)\n')
            fh.write('MESSAGE(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")\n')

            # set OPEN_CV dir if used
            if self.configuration["opencv"]["USE_OPEN_CV"]:
                fh.write('SET(OPENCV_INCL_DIR "{0}")\n'.format(self.configuration["opencv"]['OPENCV_INCL_DIR']))

            fh.write('\n')
            fh.write('# set up options\n')
            # optional configuration
            use_openmp = "ON" if self.configuration["USE_OPENMP"] else "OFF"
            fh.write('OPTION(USE_OPENMP "Build with OpenMP support" {0})\n'.format(use_openmp))

            use_log = "ON" if self.configuration["USE_LOG"] else "OFF"
            fh.write('OPTION(USE_LOG "Use logging utilities" {0})\n'.format(use_log))

            use_pytorch = "ON" if self.configuration["USE_PYTORCH"] else "OFF"
            fh.write('OPTION(USE_PYTORCH "Build with PyTorch support" {0})\n'.format(use_pytorch))

            use_opencv = "ON" if self.configuration["opencv"]["USE_OPEN_CV"] else "OFF"
            fh.write('OPTION(USE_OPEN_CV "Build with OpenCV support" {0})\n'.format(use_opencv))
            fh.write('OPTION(BUILD_SHARED_LIBS "Build using shared libraries" ON)\n')

            fh = self._write_build_option(fh=fh)

            fh.write('\n')
            fh.write('# set up include directories\n')
            fh.write('INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${NLOHMANN_JSON_INCL_DIR})\n')

            current_dir = Path(os.getcwd())
            print("Current dir {0}".format(current_dir))
            parent_dir = current_dir.parent
            print("Parent dir {0}".format(parent_dir))

            fh.write('INCLUDE_DIRECTORIES("%s/base/src/")\n' % (current_dir / "cubic_engine/"))
            fh.write('INCLUDE_DIRECTORIES("%s/base/src/")\n' % (current_dir / "kernel"))

            if self.configuration["opencv"]["USE_OPEN_CV"]:
                fh.write('INCLUDE_DIRECTORIES(${OPENCV_INCL_DIR})\n')

            fh.write('\n')
            fh.write('# set up link directories directories\n')
            fh.write('LINK_DIRECTORIES(${BOOST_LIBRARYDIR})\n')

            fh.write('\n')
            fh.write('# collect the sources\n')
            fh.write('FILE(GLOB SRCS  ${PROJECT_SOURCE_DIR}/src/cubic_engine/rl/*.cpp \n'
                     '${PROJECT_SOURCE_DIR}/src/cubic_engine/rl/worlds/*.cpp)\n')

            fh.write('\n')
            fh.write('ADD_LIBRARY(%s SHARED ${%s})\n'% ("cubic_rl_engine", "SRCS"))
            fh.write('SET_TARGET_PROPERTIES({0} PROPERTIES LINKER_LANGUAGE CXX)\n'.format("cubic_rl_engine"))
            fh.write('INSTALL(TARGETS %s DESTINATION ${CMAKE_INSTALL_PREFIX})\n' % "cubic_rl_engine")
            fh.write('MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")\n')


    def write_rl_cmake_examples(self):

        path = Path("cubic_engine/rl/examples/")
        with open(path / "CMakeLists_org.txt", 'w', newline="\n") as fh:
            rl_examples_dirs = os.listdir(path)
            for example in rl_examples_dirs:

                if (path / example).is_dir():
                    fh.write('ADD_SUBDIRECTORY({0})\n'.format(example))

                    # write CMakeLists for each example
                    with open( path / example / "CMakeLists_org.txt", 'w', newline="\n") as exe_fh:
                        exe_fh.write('cmake_minimum_required(VERSION 3.0)\n')

                        exe_fh.write('PROJECT(Example CXX)\n')
                        exe_fh.write('SET(SOURCE exe.cpp)\n')
                        exe_fh.write('SET(EXECUTABLE {0})\n'.format(example))

                        exe_fh.write('INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})\n')
                        exe_fh.write('INCLUDE_DIRECTORIES(${KERNEL_INCL_DIR})\n')
                        exe_fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')

                        exe_fh.write('LINK_DIRECTORIES(${PROJECT_LIB_DIR})\n')
                        exe_fh.write('LINK_DIRECTORIES(${KERNEL_LIB_DIR})\n')

                        exe_fh.write('ADD_EXECUTABLE(${EXECUTABLE} ${SOURCE})\n')

                        exe_fh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} cubicengine)\n')
                        exe_fh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} kernellib)\n')
                        exe_fh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)\n')
                        exe_fh.write('TARGET_LINK_LIBRARIES(${EXECUTABLE} pthread)\n')

                        


    def write_rl_cmake_tests(self):
        pass



