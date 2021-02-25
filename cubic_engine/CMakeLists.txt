cmake_minimum_required(VERSION 3.6)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(CubicEngine CXX)

# -----------------------------------------------------------------------------
# Prevent in-source builds.
# -----------------------------------------------------------------------------

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message( FATAL_ERROR  "In-source build is not possible and not recommended. Choose an empty directory for build output.")
endif(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Be sure to avoid troubles with library paths when using old policy
if(COMMAND cmake_policy)
    CMAKE_POLICY(SET CMP0003 NEW)
    CMAKE_POLICY(SET CMP0048 NEW)
    CMAKE_POLICY(SET CMP0065 NEW)
endif(COMMAND cmake_policy)

SET(CUBIC_ENGINE_VERSION_MAJOR 1)
SET(CUBIC_ENGINE_VERSION_MINOR 3)
SET(CUBIC_ENGINE_VERSION_PATCH 0)
PROJECT(CubicEngine VERSION ${CUBIC_ENGINE_VERSION_MAJOR}.${CUBIC_ENGINE_VERSION_MINOR}.${CUBIC_ENGINE_VERSION_PATCH})
SET(CUBIC_ENGINE_VERSION "${CUBIC_ENGINE_VERSION_MAJOR}.${CUBIC_ENGINE_VERSION_MINOR}.${CUBIC_ENGINE_VERSION_PATCH}")

# required configuration
SET(BUILD_SHARED_LIBS ON)
SET(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# optional configuration
OPTION(ENABLE_TESTING "Build tests" ON)
OPTION(USE_OPENMP "Build with OpenMP support" ON)
OPTION(USE_LOG "Use logging utilities" ON)
OPTION(USE_PYTORCH "Build with PyTorch support" OFF)
OPTION(USE_TRILINOS "Build with Trillinos support" OFF)
OPTION(USE_TRILINOS_LONG_LONG_TYPE "Specify the integer type that Trilinos has been compiled" OFF)
OPTION(USE_OPEN_CV "Build with OpenCV support" ON)
OPTION(USE_RL "Build with RL framework support" ON)
OPTION(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# project include directory
SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/include)

# project source directory
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/source/cubic_engine)

# where the library is installed
#SET(PROJECT_LIB_DIR ${CMAKE_INSTALL_PREFIX})

# project working directory
SET(PWD ${PROJECT_SOURCE_DIR})
SET(CMAKE_INSTALL_PREFIX ${PWD}/install)

SET(MAGIC_ENUM_INCL_DIR " ")
SET(BLAZE_INCL_DIR "/home/alex/MySoftware/blaze-3.8/install/include")
SET(NLOHMANN_JSON_INCL_DIR "/home/alex/MySoftware/nlohman_json/install/include")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CENGINE_DEBUG "ON")
ENDIF()

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/cubic_engine/base/config.h @ONLY)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/include/cubic_engine/base/version.h @ONLY)

IF(USE_OPEN_CV)

    SET(OPENCV_INCL_DIR "/home/alex/MySoftware/opencv/install/include/opencv4")
    SET(OPENCV_LIB_DIR "/home/alex/MySoftware/opencv/install/lib")
    INCLUDE_DIRECTORIES(${OPENCV_INCL_DIR})
    LINK_DIRECTORIES(${OPENCV_LIB_DIR})
    MESSAGE(STATUS  "Using OpenCV include files at ${OPENCV_INCL_DIR} ")
    MESSAGE(STATUS  "Using OpenCV libraries at ${OPENCV_LIB_DIR} ")
ELSE()
    MESSAGE(STATUS  "OpenCV will not be used")
ENDIF()


# TODO: Fix this
# add this because config.h ends up in the build directory
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)

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
IF(CENGINE_DEBUG MATCHES "ON")
    SET(KERNEL_LIB_DIR  ${PROJECT_SOURCE_DIR}/../kernel/install/lib/dbg)
ELSE()
    SET(KERNEL_LIB_DIR ${PROJECT_SOURCE_DIR}/../kernel/install/lib/opt)
ENDIF()

SET(KERNEL_INCL_DIR ${PROJECT_SOURCE_DIR}/../kernel/include)

if(${KERNEL_INCL_DIR} MATCHES " ")
    MESSAGE(FATAL_ERROR " kernel directory not set")
else()
    INCLUDE_DIRECTORIES(${KERNEL_INCL_DIR})
endif()

IF(${MAGIC_ENUM_INCL_DIR} MATCHES " ")
    MESSAGE(STATUS  "MagicEnum include directory has not been specified")
ELSE()
    INCLUDE_DIRECTORIES(${MAGIC_ENUM_INCL_DIR})
ENDIF()


if(${BLAZE_INCL_DIR} MATCHES " ")
    MESSAGE(FATAL_ERROR " Blaze directory not set")
else()
    INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})
endif()

IF(USE_TRILINOS)
    SET(TRILINOS_INCL_DIR "/home/alex/MySoftware/trilinos/install/include")
    SET(TRILINOS_LIB_DIR "/home/alex/MySoftware/trilinos/install/lib")
    INCLUDE_DIRECTORIES(${TRILINOS_INCL_DIR})
    LINK_DIRECTORIES(${TRILINOS_LIB_DIR})
ENDIF()

IF( ENABLE_TESTING)
    SET(GTEST_INC_DIR "/home/alex/MySoftware/gtest/install/include")
    SET(GTEST_LIB_DIR "/home/alex/MySoftware/gtest/install/lib")
    INCLUDE_DIRECTORIES(${GTEST_INC_DIR})
    LINK_DIRECTORIES(${GTEST_LIB_DIR})
ENDIF()

IF( ENABLE_TESTING MATCHES "ON" AND GTEST_INC_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Testing is enabled but GTest include directory has not been specified")
ENDIF()

IF( ENABLE_TESTING MATCHES "ON" AND GTEST_LIB_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Testing is enabled but GTest lib directory has not been specified")
ENDIF()

IF(USE_PYTORCH)
	find_package(Torch REQUIRED)
	INCLUDE_DIRECTORIES(${TORCH_INCLUDE_DIRS})
ELSE()
        MESSAGE(STATUS  "Building without PyTorch support")
ENDIF()

IF(USE_OPENMP)

    IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES " ")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fopenmp -fPIC")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
    ELSE()
        SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread -fopenmp -fPIC")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)
    ENDIF()
ELSE()

    IF(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES " ")
        SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fPIC")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)
    ELSE()
        SET(CMAKE_CXX_FLAGS "-std=c++17 -O2 -pthread -fPIC")
        SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/opt)
    ENDIF()
ENDIF()

SET(CMAKE_LINKER_FLAGS "-pthread")

INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})

# define the library name
ADD_LIBRARY(cubicengine SHARED "")
SET_TARGET_PROPERTIES(cubicengine PROPERTIES LINKER_LANGUAGE CXX)

# Add source directories
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/estimation)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/control)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/ml)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/maths)


IF(USE_RL)
    ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/rl)
ELSE()
    MESSAGE(STATUS  "RL framework will not be built")
ENDIF()

TARGET_INCLUDE_DIRECTORIES(cubicengine INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
INSTALL(TARGETS cubicengine DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add the examples
ADD_SUBDIRECTORY(examples)

IF (ENABLE_TESTING)
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(tests)
ENDIF()
