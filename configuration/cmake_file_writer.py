
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

    def write_cmake_lists(self):
        with open(self._main_cmake, 'w', newline="\n") as fh:
            # start creating main CMakeLists
            fh.write("CMAKE_MINIMUM_REQUIRED(VERSION 3.6)\n")
            fh.write('MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}\n')
            fh.write('PROJECT({0} CXX)\n'.format(self._project_name))
            fh.write('# -----------------------------------------------------------------------------\n')
            fh.write('# Prevent in-source builds.\n')
            fh.write('# -----------------------------------------------------------------------------\n')
            fh.write('IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})\n')
            fh.write('\tMESSAGE( FATAL_ERROR  "In-source build is not possible and not recommended.'
                     'Choose an empty directory for build output.")\n')
            fh.write('ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})\n')
            fh.write('# Be sure to avoid troubles with library paths when using old policy\n')
            fh.write('IF(COMMAND cmake_policy)\n')
            fh.write('\tCMAKE_POLICY(SET CMP0003 NEW)\n')

    def write_test_cmake_lists(self):
        pass


class CMakeCubicEngineWriter(CMakeFileWriter):

    def __init__(self, configuration: dict) -> None:
        super(CMakeCubicEngineWriter, self).__init__(configuration=configuration,
                                                     project_name="CubicEngine")

    def write_cmake_lists(self):

        # call super class
        super(CMakeCubicEngineWriter, self).write_cmake_lists()

        if self.configuration["testing"]:
            self.write_test_cmake_lists()

        if self.configuration["cengine"]["USE_RL"]:
            self.write_rl_cmake_lists()

    def write_rl_cmake_lists(self):

        with open("cubic_engine/source/cubic_engine/rl/CMakeLists.txt", 'w', newline="\n") as fh:
            pass
