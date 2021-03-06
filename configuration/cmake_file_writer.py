
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
            fh.write('MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}\n')
            fh.write('PROJECT({0} CXX)\n'.format(options["project_name"]))
            fh.write('\n')
            fh.write('# -----------------------------------------------------------------------------\n')
            fh.write('# Prevent in-source builds.\n')
            fh.write('# -----------------------------------------------------------------------------\n')
            fh.write('IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})\n')
            fh.write('\tMESSAGE( FATAL_ERROR  "In-source build is not possible and not recommended.'
                     'Choose an empty directory for build output.")\n')
            fh.write('ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})\n')

            fh.write('\n')
            fh.write('# Be sure to avoid troubles with library paths when using old policy\n')
            fh.write('IF(COMMAND cmake_policy)\n')
            fh.write('\tCMAKE_POLICY(SET CMP0003 NEW)\n')
            fh.write('\tCMAKE_POLICY(SET CMP0048 NEW)\n')
            fh.write('ENDIF(COMMAND cmake_policy)\n')
            return fh

    def write_test_cmake_lists(self):
        pass

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
        fh.write('\tBoost_USE_SHARED_LIBS ON\n')
        fh.write('ELSE()\n')
        fh.write('\tMESSAGE( FATAL_ERROR  "Boost C++ library is required but not found.")\n')
        fh.write('ENDIF()\n')
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
        return fh

    def _write_include_files(self, fh):
        return fh

    def _write_options(self, fh):
        pass

    def _write_opencv(self, fh):
        pass

    def _write_configure_files(self, fh):
        pass


class CMakeCubicEngineWriter(CMakeFileWriter):

    def __init__(self, configuration: dict) -> None:
        super(CMakeCubicEngineWriter, self).__init__(configuration=configuration,
                                                     project_name="CubicEngine")

    def write_cmake_lists(self):

        # call super class
        #super(CMakeCubicEngineWriter, self).write_cmake_lists()

        if self.configuration["testing"]:
            self.write_test_cmake_lists()

        if self.configuration["cengine"]["rl"]["USE_RL"]:
            self.write_rl_cmake_lists()

    def write_rl_cmake_lists(self):

        with open("cubic_engine/rl/CMakeLists_test.txt", 'w', newline="\n") as fh:

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
            fh.write('SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR})\n')
            fh.write('SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR})\n')

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

            use_examples = "ON" if self.configuration["cengine"]["rl"]["BUILD_EXAMPLES"] else "OFF"
            fh.write('OPTION(BUILD_EXAMPLES "Build the examples" {0})\n'.format(use_examples))

            fh = self._write_build_option(fh=fh)

            fh.write('\n')
            fh.write('# set up include directories\n')
            fh.write('INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')
            fh.write('INCLUDE_DIRECTORIES(${NLOHMANN_JSON_INCL_DIR})\n')

            fh.write('\n')
            fh.write('# set up link directories directories\n')
            fh.write('LINK_DIRECTORIES(${BOOST_LIBRARYDIR})\n')

            fh.write('\n')
            fh.write('# collect the sources')
            fh.write('FILE(GLOB SRCS  *.cpp '
                     '${PROJECT_SRC_DIR} / worlds / *.cpp)\n')

            fh.write('ADD_LIBRARY(${LIB_NAME} SHARED ${SRCS})\n')

            fh.write('SET_TARGET_PROPERTIES({0} PROPERTIES LINKER_LANGUAGE CXX)\n'.format("cubic_rl_engine"))
            fh.write('INSTALL(TARGETS %s DESTINATION ${CMAKE_INSTALL_PREFIX})\n' % "cubic_rl_engine")
            fh.write('MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")\n')



