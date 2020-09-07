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
endif(COMMAND cmake_policy)

SET(CUBIC_ENGINE_VERSION_MAJOR 1)
SET(CUBIC_ENGINE_VERSION_MINOR 1)
SET(CUBIC_ENGINE_VERSION_PATCH 1)
PROJECT(CubicEngine VERSION ${CUBIC_ENGINE_VERSION_MAJOR}.${CUBIC_ENGINE_VERSION_MINOR}.${CUBIC_ENGINE_VERSION_PATCH})
SET(CUBIC_ENGINE_VERSION "${CUBIC_ENGINE_VERSION_MAJOR}.${CUBIC_ENGINE_VERSION_MINOR}.${CUBIC_ENGINE_VERSION_PATCH}")

SET(BUILD_SHARED_LIBS ON)
SET(CMAKE_BUILD_TYPE "Debug")
SET(ENABLE_TESTING "OFF")
SET(USE_OPENMP "ON")
SET(USE_LOG "ON")
SET(USE_PYTORCH "OFF")
SET(USE_TRILINOS "ON")
SET(USE_TRILINOS_LONG_LONG_TYPE "OFF")
SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/source/cubic_engine)
SET(PWD ${PROJECT_SOURCE_DIR})
SET(CMAKE_INSTALL_PREFIX ${PWD}/install)
SET(MAGIC_ENUM_INCL_DIR " ")
SET(BLAZE_INCL_DIR "/home/alex/MySoftware/blaze/blaze-3.7/install/include")
SET(NLOHMANN_JSON_INCL_DIR "/home/alex/MySoftware/nlohman_json/install/include")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(CENGINE_DEBUG "ON")
ENDIF()

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/cubic_engine/base/config.h @ONLY)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/include/cubic_engine/base/version.h @ONLY)

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

IF(USE_TRILINOS MATCHES "ON")
    SET(TRILINOS_INCL_DIR "/home/alex/MySoftware/trilinos/install/include")
    SET(TRILINOS_LIB_DIR "/home/alex/MySoftware/trilinos/install/lib")
    INCLUDE_DIRECTORIES(${TRILINOS_INCL_DIR})
    LINK_DIRECTORIES(${TRILINOS_LIB_DIR})
ENDIF()

IF( ENABLE_TESTING MATCHES "ON")
    SET(GTEST_INC_DIR "/home/david/MySoftware/gtest/install/include")
    SET(GTEST_LIB_DIR "/home/david/MySoftware/gtest/install/lib")
    INCLUDE_DIRECTORIES(${GTEST_INC_DIR})
ENDIF()

IF( ENABLE_TESTING MATCHES "ON" AND GTEST_INC_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Testing is enabled but GTest include directory has not been specified")
ENDIF()

IF( ENABLE_TESTING MATCHES "ON" AND GTEST_LIB_DIR MATCHES " " )
    MESSAGE( FATAL_ERROR  "Testing is enabled but GTest lib directory has not been specified")
ENDIF()

IF(USE_PYTORCH MATCHES "ON")
	find_package(Torch REQUIRED)
	INCLUDE_DIRECTORIES(${TORCH_INCLUDE_DIRS})
ENDIF()

IF(USE_OPENMP MATCHES "ON")

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

FILE(GLOB  SRCS  ${PROJECT_SRC_DIR}/estimation/*.cpp
                 ${PROJECT_SRC_DIR}/systems/*.cpp
                 ${PROJECT_SRC_DIR}/control/*.cpp
                 ${PROJECT_SRC_DIR}/ml/instance_learning/*.cpp
                 ${PROJECT_SRC_DIR}/ml/instance_learning/utils/*.cpp
                 ${PROJECT_SRC_DIR}/ml/supervised_learning/*.cpp
                 ${PROJECT_SRC_DIR}/ml/supervised_learning/utils/*.cpp
                 ${PROJECT_SRC_DIR}/ml/unsupervised_learning/*.cpp
                 ${PROJECT_SRC_DIR}/ml/unsupervised_learning/utils/*.cpp
                 ${PROJECT_SRC_DIR}/rl/*.cpp
                 ${PROJECT_SRC_DIR}/rl/*/*.cpp
                 ${PROJECT_SRC_DIR}/optimization/utils/*.cpp
                 ${PROJECT_SRC_DIR}/maths/*.cpp)

SET(PROJECT_LIB_DIR ${CMAKE_INSTALL_PREFIX})
ADD_LIBRARY(cubicengine SHARED ${SRCS})
SET_TARGET_PROPERTIES(cubicengine PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS cubicengine DESTINATION ${CMAKE_INSTALL_PREFIX})

# Add the examples
ADD_SUBDIRECTORY(examples)

IF (ENABLE_TESTING MATCHES "ON")
    ENABLE_TESTING()
    ADD_SUBDIRECTORY(tests)
ENDIF()
