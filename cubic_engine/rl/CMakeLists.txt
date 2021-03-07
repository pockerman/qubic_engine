CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(cubic_rl_engine CXX)

# -----------------------------------------------------------------------------
# Prevent in-source builds.
# -----------------------------------------------------------------------------

IF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
	MESSAGE( FATAL_ERROR  "In-source build is not possible. Choose an empty directory for build output.")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# Be sure to avoid troubles with library paths when using old policy
IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0003 NEW)
	CMAKE_POLICY(SET CMP0048 NEW)
ENDIF(COMMAND cmake_policy)

# find Boost
FIND_PACKAGE(Boost 1.65.0 REQUIRED)
IF(Boost_FOUND)
	MESSAGE( STATUS  "Found needed Boost C++ library.")
	SET(Boost_USE_SHARED_LIBS ON)
ELSE()
	MESSAGE( FATAL_ERROR  "Boost C++ library is required but not found.")
ENDIF()

# find OpenBLAS
FIND_PACKAGE(BLAS REQUIRED)
IF(NOT BLAS_FOUND)
	MESSAGE( FATAL_ERROR  "Could not find OpenBLAS library.")
ELSE()
	MESSAGE( STATUS  "Found needed BLAS library.")
ENDIF()

# default optionsSET(BUILD_SHARED_LIBS ON)
SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
SET(PWD ${PROJECT_SOURCE_DIR})
SET(CMAKE_INSTALL_PREFIX ${PWD} / install)
SET(MAGIC_ENUM_INCL_DIR " ")
SET(CMAKE_LINKER_FLAGS "-pthread")
SET(BLAZE_INCL_DIR "/home/alex/MySoftware/blaze-3.8/install/include")
SET(NLOHMANN_JSON_INCL_DIR "/home/alex/MySoftware/nlohman_json/install/include")
SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/src)
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/src)
MESSAGE(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
SET(OPENCV_INCL_DIR "/home/alex/MySoftware/opencv/install/include/opencv4")

# set up options
OPTION(USE_OPENMP "Build with OpenMP support" ON)
OPTION(USE_LOG "Use logging utilities" ON)
OPTION(USE_PYTORCH "Build with PyTorch support" OFF)
OPTION(USE_OPEN_CV "Build with OpenCV support" ON)
OPTION(BUILD_SHARED_LIBS "Build using shared libraries" ON)
SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fopenmp -fPIC")
SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install/lib/dbg)

# set up include directories
INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})
INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})
INCLUDE_DIRECTORIES(${NLOHMANN_JSON_INCL_DIR})
INCLUDE_DIRECTORIES("/home/alex/qi3/ce_new_build/cubic_engine/base/src/")
INCLUDE_DIRECTORIES("/home/alex/qi3/ce_new_build/kernel/base/src/")
INCLUDE_DIRECTORIES(${OPENCV_INCL_DIR})

# set up link directories directories
LINK_DIRECTORIES(${BOOST_LIBRARYDIR})

# collect the sources
FILE(GLOB SRCS  ${PROJECT_SOURCE_DIR}/src/cubic_engine/rl/*.cpp 
${PROJECT_SOURCE_DIR}/src/cubic_engine/rl/worlds/*.cpp)

ADD_LIBRARY(cubic_rl_engine SHARED ${SRCS})
SET_TARGET_PROPERTIES(cubic_rl_engine PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS cubic_rl_engine DESTINATION ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")
