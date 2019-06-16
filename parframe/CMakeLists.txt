cmake_minimum_required(VERSION 3.0)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(ParallelFrameLib CXX)

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
SET(LIB_NAME "parframelib")

SET(PARFRAMELIB_VERSION_MAJOR 0)
SET(PARFRAMELIB_VERSION_MINOR 1)
SET(PARFRAMELIB_VERSION_PATCH 0)
PROJECT(ParallelFrameLib VERSION ${PARFRAMELIB_VERSION_MAJOR}.${PARFRAMELIB_VERSION_MINOR}.${PARFRAMELIB_VERSION_PATCH})
SET(PARFRAMELIB_VERSION "${PARFRAMELIB_VERSION_MAJOR}.${PARFRAMELIB_VERSION_MINOR}.${PARFRAMELIB_VERSION_PATCH}")

# find Boost
FIND_PACKAGE(Boost 1.61.0 REQUIRED)
IF(Boost_FOUND)
        SET(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})
        SET(BOOST_LIBRARYDIR ${Boost_LIBRARY_DIRS})
        SET(Boost_USE_SHARED_LIBS ON)
ENDIF()

SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/source/parframe)

# Currently fail the build if the user does not supply the
# variables.
# TODO: How to do it better
SET(BLAZE_INCL_DIR " ")

if(${BLAZE_INCL_DIR} MATCHES " ")
    MESSAGE(FATAL_ERROR " Blaze directory not set")
else()
    INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})
endif()

SET(GTEST_INC_DIR " ")
SET(GTEST_LIB_DIR " ")

if(${GTEST_INC_DIR} MATCHES " " OR ${GTEST_LIB_DIR} MATCHES " ")
    MESSAGE(FATAL_ERROR " GTest not configured")
else()
    INCLUDE_DIRECTORIES(${GTEST_INC_DIR})
endif()

IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES " ")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread")
        SET(CMAKE_LINKER_FLAGS "-pthread")
        SET(CUBIC_ENGINE_DEBUG 1)
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
ELSEIF(CMAKE_BUILD_TYPE MATCHES "Release")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread")
        SET(CMAKE_LINKER_FLAGS "-pthread")
        SET(CUBIC_ENGINE_DEBUG 0)
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)
ELSE()

     SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread")
     SET(CMAKE_LINKER_FLAGS "-pthread")
     SET(CUBIC_ENGINE_DEBUG 1)
     SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
ENDIF()

SET(PROJECT_LIB_DIR ${CMAKE_INSTALL_PREFIX})

# include dirs used by the project
INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR}) 
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})

# use the Boost link directories
LINK_DIRECTORIES(${BOOST_LIBRARYDIR})

# add a shared library to link the app
# TODO: do this better as this way is not scalable
FILE(GLOB  SRCS  ${PROJECT_SRC_DIR}/base/*.cpp
                 ${PROJECT_SRC_DIR}/executors/*.cpp
                 ${PROJECT_SRC_DIR}/utilities/*.cpp
                 ${PROJECT_SRC_DIR}/data_structs/*.cpp)
                 
ADD_LIBRARY(${LIB_NAME} SHARED {SRCS})

SET_TARGET_PROPERTIES(parframelib PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS parframelib DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add the examples
ADD_SUBDIRECTORY(examples)

ENABLE_TESTING()
#ADD_SUBDIRECTORY(tests/test_max_element)
#ADD_SUBDIRECTORY(tests/test_range_1d)
ADD_SUBDIRECTORY(tests/test_stoppable_task)
ADD_SUBDIRECTORY(tests/test_thread_pool)
