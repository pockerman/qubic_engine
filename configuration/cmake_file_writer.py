import os
from pathlib import Path

from configuration import INFO


class CMakeFileWriter(object):
    def __init__(self, configuration, project_name, install_prefix):
        self._configuration = configuration
        self._project_name = project_name
        self._install_prefix = install_prefix


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
            fh.write('SET(CMAKE_CXX_COMPILER {0})\n'.format(self.configuration["CMAKE_CXX_COMPILER"]))
            fh.write('SET(CMAKE_CXX_STANDARD 17)\n')
            fh.write('SET(CMAKE_CXX_STANDARD_REQUIRED True)\n')
            fh.write('SET(CMAKE_C_COMPILER {0})\n'.format(self.configuration["CMAKE_C_COMPILER"]))
            fh.write('SET(PWD ${PROJECT_SOURCE_DIR})\n')
            fh.write('SET(CMAKE_INSTALL_PREFIX {0})\n'.format(self._install_prefix))
            fh.write('SET(MAGIC_ENUM_INCL_DIR " ")\n')
            fh.write('SET(CMAKE_LINKER_FLAGS "-pthread")\n')
            fh.write('SET(USE_OPENMP {0})\n'.format(self.configuration["USE_OPENMP"]))
            fh.write('SET(USE_LOG {0})\n'.format(self.configuration["USE_LOG"]))

            blaze_dir = self.configuration["BLAZE_INCL_DIR"]
            fh.write('SET(BLAZE_INCL_DIR "{0}")\n'.format(blaze_dir))

            nlohmann_json_dir = self.configuration["NLOHMANN_JSON_INCL_DIR"]
            fh.write('SET(NLOHMANN_JSON_INCL_DIR "{0}")\n'.format(nlohmann_json_dir))
            fh = self._write_project_variables(fh=fh)

            fh = self._find_boost(fh=fh)
            fh = self._find_blas(fh=fh)
            fh = self._write_options(fh=fh)
            fh = self._write_configure_files(fh=fh)

            fh.write('MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")\n')
            fh.write('MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")\n')
            fh.write('MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")\n')
            fh.write('\n')
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

        fh.write("\n")
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
        return fh

    def _write_configure_files(self, fh):
        return fh






