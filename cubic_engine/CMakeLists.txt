cmake_minimum_required(VERSION 3.0)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(CubicEngine CXX) 

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

SET(CUBIC_ENGINE_VERSION_MAJOR 0)
SET(CUBIC_ENGINE_VERSION_MINOR 1)
SET(CUBIC_ENGINE_VERSION_PATCH 0)
PROJECT(CubicEngine VERSION ${CUBIC_ENGINE_VERSION_MAJOR}.${CUBIC_ENGINE_VERSION_MINOR}.${CUBIC_ENGINE_VERSION_PATCH})
SET(CUBIC_ENGINE_VERSION "${CUBIC_ENGINE_VERSION_MAJOR}.${CUBIC_ENGINE_VERSION_MINOR}.${CUBIC_ENGINE_VERSION_PATCH}")

SET(BUILD_SHARED_LIBS ON)
SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/source/cubic_engine)

#find boost. How to make this user defined?
find_package(Boost 1.65.0 REQUIRED)

IF(Boost_FOUND)
SET(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})
SET(BOOST_LIBRARYDIR ${Boost_LIBRARY_DIRS})
SET(Boost_USE_SHARED_LIBS ON)
ENDIF()

# Currently fail the build if the user does not supply the
# variables.
# TODO: How to do it better
SET(KERNEL_INCL_DIR " ")

if(${KERNEL_INCL_DIR} MATCHES " ")
    MESSAGE(FATAL_ERROR " kernel directory not set")
else()
    INCLUDE_DIRECTORIES(${KERNEL_INCL_DIR})
endif()

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

INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})

FILE(GLOB  SRCS  ${PROJECT_SRC_DIR}/estimation/*.cpp
                 ${PROJECT_SRC_DIR}/systems/*.cpp
                 ${PROJECT_SRC_DIR}/control/*.cpp)

IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES " ")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fPIC")
        SET(CMAKE_LINKER_FLAGS "-pthread")
        SET(CUBIC_ENGINE_DEBUG 1)
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
ELSEIF(CMAKE_BUILD_TYPE MATCHES "Release")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread -fPIC")
        SET(CMAKE_LINKER_FLAGS "-pthread")
        SET(CUBIC_ENGINE_DEBUG 0)
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)
ELSE()

        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fPIC")
        SET(CMAKE_LINKER_FLAGS "-pthread")
        SET(CUBIC_ENGINE_DEBUG 1)
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
ENDIF()

SET(PROJECT_LIB_DIR ${CMAKE_INSTALL_PREFIX})
ADD_LIBRARY(cubicengine SHARED ${SRCS}) 
SET_TARGET_PROPERTIES(cubicengine PROPERTIES LINKER_LANGUAGE CXX) 
INSTALL(TARGETS cubicengine DESTINATION ${CMAKE_INSTALL_PREFIX}) 

# Add the examples
ADD_SUBDIRECTORY(examples)

ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)
