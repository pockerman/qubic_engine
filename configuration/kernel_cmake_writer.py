import os
from pathlib import Path
from configuration import INFO
from configuration.cmake_file_writer import CMakeFileWriter

class KernelCMakeWriter(CMakeFileWriter):

    def __init__(self, configuration):
        super(KernelCMakeWriter, self).__init__(configuration=configuration,
                                                project_name="kernel")

        self.dirs = ['base', 'data_structs', 'geometry', 'maths',
                     'parallel', 'patterns', 'utilities']

    def write_cmake_lists(self):

        print("{0} Writing CMakeLists for project {1}".format(INFO, self.project_name))
        with open("kernel/kernel/CMakeLists.txt", 'w', newline="\n") as fh:
            # call super class
            project_name = 'kernel'
            fh = self.write_basic_lists(fh=fh, **{"project_name": "kernel"})

            fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')
            for directory in self.dirs:
                fh.write('INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/%s/src/)\n' % (directory))

            fh.write("IF({0})".format("USE_DISCRETIZATION"))
            fh.write('INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})\n')
            fh.write("ELSE()\n")
            fh.write('MESSAGE(STATUS "Discretization module in not built)\n')
            fh.write("ENDIF()")

            fh.write('INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})\n')
            fh.write('\n')

            fh.write('ADD_LIBRARY({0} SHARED "")\n'.format(self.project_name))

            fh.write('\n')

            current_dir = Path(os.getcwd())
            print("Current dir {0}".format(current_dir))
            parent_dir = current_dir.parent
            print("Parent dir {0}".format(parent_dir))

            # Add source directories
            for directory in self.dirs:
                fh.write('ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/%s/src/%s/%s/)\n' % (directory, self.project_name, directory))

                src_dir = current_dir / self.project_name / self.project_name / directory / 'src' / self.project_name / directory
                with open(src_dir / 'CMakeLists.txt', 'w', newline='\n') as local_fh:
                    dir_upper = directory.upper()+'_SRCS'
                    local_fh.write('IF(COMMAND cmake_policy)\n')
                    local_fh.write('\tCMAKE_POLICY(SET CMP0076 NEW)\n')
                    local_fh.write('ENDIF(COMMAND cmake_policy)\n')
                    local_fh.write('\n')
                    local_fh.write('FILE(GLOB %s ${PROJECT_SOURCE_DIR}/%s/src/%s/%s/*.cpp '
                                   '${PROJECT_SOURCE_DIR}/%s/src/%s/%s/*/*.cpp)\n' % (dir_upper, directory, self.project_name, directory,
                                                                                      directory, self.project_name, directory))
                    local_fh.write('TARGET_SOURCES(%s PUBLIC ${%s})\n' % (self.project_name, dir_upper))

            fh.write('\n')
            fh.write('SET_TARGET_PROPERTIES({0} PROPERTIES LINKER_LANGUAGE CXX)\n'.format(self.project_name))
            fh.write('INSTALL(TARGETS %s DESTINATION ${CMAKE_INSTALL_PREFIX})\n' % self.project_name)
            fh.write('MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")\n')

        print("{0} Done...".format(INFO))


    def _write_project_variables(self, fh):
        return fh

    def _write_options(self, fh):

        if self.configuration[self.project_name]["USE_DISCRETIZATION"]:
            fh.write('OPTION({0} "Build with discretization module " ON)\n'.format("USE_DISCRETIZATION"))
        else:
            fh.write('OPTION({0} "Build with discretization module " OFF)\n'.format("USE_DISCRETIZATION"))

        return fh

    def _write_configure_files(self, fh):
        fh.write('configure_file(config.h.in ${PROJECT_SOURCE_DIR}/base/src/kernel/base/config.h @ONLY)\n')
        fh.write('configure_file(version.h.in ${PROJECT_SOURCE_DIR}/base/src/kernel/base/version.h @ONLY)\n')
        fh.write('\n')
        return fh