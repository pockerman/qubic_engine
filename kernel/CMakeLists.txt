cmake_minimum_required(VERSION 3.0)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(kernellib CXX)

#-----------------------------------------------------------------------------
# Prevent in-source builds.
#-----------------------------------------------------------------------------

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message( FATAL_ERROR  "In-source build is not possible and not recommended. Choose an empty directory for build output.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Be sure to avoid troubles with library paths when using old policy
if(COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 NEW)
    CMAKE_POLICY(SET CMP0048 NEW)
endif(COMMAND cmake_policy)

# set the name of the library to link against
SET(LIB_NAME "kernellib")

SET(KERNELLIB_VERSION_MAJOR 0)
SET(KERNELLIB_VERSION_MINOR 1)
SET(KERNELLIB_VERSION_PATCH 0)
PROJECT(KernelLib VERSION ${KERNELLIB_VERSION_MAJOR}.${KERNELLIB_VERSION_MINOR}.${KERNELLIB_VERSION_PATCH})
SET(KERNELLIB_VERSION "${KERNELLIB_VERSION_MAJOR}.${KERNELLIB_VERSION_MINOR}.${KERNELLIB_VERSION_PATCH}")

# find Boost
FIND_PACKAGE(Boost 1.65.0 REQUIRED)
IF(Boost_FOUND)
        SET(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})
        SET(BOOST_LIBRARYDIR ${Boost_LIBRARY_DIRS})
        SET(Boost_USE_SHARED_LIBS ON)
ENDIF()

SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/src/kernel)

SET(BLAZE_INCL_DIR " ")

IF( BLAZE_INCL_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Blaze include directory has not been specified")
ENDIF()

SET(ENABLE_TESTING ON)

IF( ENABLE_TESTING MATCHES "ON")
    SET(GTEST_INC_DIR " ")
    SET(GTEST_LIB_DIR " ")
ENDIF()

IF( ENABLE_TESTING MATCHES "ON" AND GTEST_INC_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Testing is enabled but GTest include directory has not been specified")
ENDIF()

IF( ENABLE_TESTING MATCHES "ON" AND GTEST_LIB_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Testing is enabled but GTest lib directory has not been specified")
ENDIF()

SET(USE_OPENMP ON)

IF(USE_OPENMP MATCHES "ON")

    IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES " ")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fopenmp")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
    ELSE()
        SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread -fopenmp")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)
    ENDIF()
ELSE()

    IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES " ")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
    ELSE()
        SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)
    ENDIF()
ENDIF()

SET(CMAKE_LINKER_FLAGS "-pthread")


SET(PROJECT_LIB_DIR ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "INSTALL DIR ${CMAKE_INSTALL_PREFIX}")

# include dirs used by the project
INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR}) 
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})
INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})
INCLUDE_DIRECTORIES(${GTEST_INC_DIR})

# use the Boost link directories
LINK_DIRECTORIES(${BOOST_LIBRARYDIR})

FILE(GLOB  SRCS  ${PROJECT_SRC_DIR}/base/*.cpp
                 ${PROJECT_SRC_DIR}/parallel/threading/*.cpp
                 ${PROJECT_SRC_DIR}/parallel/utilities/*.cpp
                 ${PROJECT_SRC_DIR}/utilities/*.cpp
                 ${PROJECT_SRC_DIR}/vehicles/*.cpp
                 ${PROJECT_SRC_DIR}/maths/functions/*.cpp)


ADD_LIBRARY(${LIB_NAME} SHARED ${SRCS})

SET_TARGET_PROPERTIES(kernellib PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS kernellib DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add the examples
ADD_SUBDIRECTORY(examples)

IF( ENABLE_TESTING MATCHES "ON")
ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)
ENDIF()
