cmake_minimum_required(VERSION 3.6)
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

SET(KERNELLIB_VERSION_MAJOR 1)
SET(KERNELLIB_VERSION_MINOR 3)
SET(KERNELLIB_VERSION_PATCH 0)
PROJECT(KernelLib VERSION ${KERNELLIB_VERSION_MAJOR}.${KERNELLIB_VERSION_MINOR}.${KERNELLIB_VERSION_PATCH})
SET(KERNELLIB_VERSION "${KERNELLIB_VERSION_MAJOR}.${KERNELLIB_VERSION_MINOR}.${KERNELLIB_VERSION_PATCH}")
MESSAGE(STATUS "KernelLib Version ${KERNELLIB_VERSION}")

# find Boost
FIND_PACKAGE(Boost 1.65.0 REQUIRED)
IF(Boost_FOUND)

		MESSAGE( STATUS  "Found needed Boost C++ library.")
        SET(BOOST_INCLUDEDIR ${Boost_INCLUDE_DIRS})
        SET(BOOST_LIBRARYDIR ${Boost_LIBRARY_DIRS})
        SET(Boost_USE_SHARED_LIBS ON)
ELSE()
MESSAGE( FATAL_ERROR  "Boost 1.65.0 is required but not found.")
ENDIF()

# find OpenBLAS
FIND_PACKAGE(BLAS REQUIRED)

IF(NOT BLAS_FOUND)
MESSAGE( FATAL_ERROR  "Could not find OpenBLAS library.")
ELSE()
MESSAGE( STATUS  "Found needed BLAS library.")
ENDIF()

SET(BUILD_SHARED_LIBS ON)
SET(CMAKE_BUILD_TYPE "Debug")
SET(ENABLE_TESTING "ON")
SET(USE_OPENMP "ON")
SET(USE_LOG "ON")
SET(USE_FVM "ON")
SET(USE_TRILINOS "OFF")
SET(USE_TRILINOS_LONG_LONG_TYPE "OFF")
SET(USE_OPEN_CV "ON")

SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)
SET(PROJECT_INCL_DIR ${PROJECT_SOURCE_DIR}/include)
SET(PROJECT_SRC_DIR  ${PROJECT_SOURCE_DIR}/src/kernel)
SET(DATA_SET_FOLDER ${PROJECT_SOURCE_DIR}/data)
SET(PROJECT_PATH ${PROJECT_SOURCE_DIR})
SET(MAGIC_ENUM_INCL_DIR " ")
SET(BLAZE_INCL_DIR "/home/alex/MySoftware/blaze-3.8/install/include")
SET(NLOHMANN_JSON_INCL_DIR "/home/alex/MySoftware/nlohman_json/install/include")

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    SET(KERNEL_DEBUG "ON")
ENDIF()

MESSAGE(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
MESSAGE(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
MESSAGE(STATUS "C Compiler: ${CMAKE_C_COMPILER}")

configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/kernel/base/config.h @ONLY)
configure_file(version.h.in ${PROJECT_SOURCE_DIR}/include/kernel/base/version.h @ONLY)

# TODO: Fix this
# add this because config.h ends up in the build directory
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/include)


IF(USE_OPEN_CV MATCHES "ON")

    SET(OPENCV_INCL_DIR "/home/alex/MySoftware/opencv/install/include/opencv4")
    INCLUDE_DIRECTORIES(${OPENCV_INCL_DIR})
    MESSAGE(STATUS  "Using OpenCV at ${OPENCV_INCL_DIR} ")
ELSE()
    MESSAGE(STATUS  "OpenCV is not used")
ENDIF()

IF(${MAGIC_ENUM_INCL_DIR} MATCHES " ")
    MESSAGE( STATUS  "MagicEnum include directory has not been specified")
ELSE()
    INCLUDE_DIRECTORIES(${MAGIC_ENUM_INCL_DIR})
    MESSAGE(STATUS  "Using Magic enum at ${MAGIC_ENUM_INCL_DIR} ")
ENDIF()


IF( ${BLAZE_INCL_DIR} MATCHES " " )
    MESSAGE( FATAL_ERROR  "Blaze include directory has not been specified")
ELSE()
    MESSAGE( STATUS  "Using BLAZE headers at: ${BLAZE_INCL_DIR}")
ENDIF()

IF(USE_TRILINOS MATCHES "ON")
    SET(TRILINOS_INCL_DIR "/home/alex/MySoftware/trilinos/install/include")
    SET(TRILINOS_LIB_DIR "/home/alex/MySoftware/trilinos/install/lib")
    INCLUDE_DIRECTORIES(${TRILINOS_INCL_DIR})
    LINK_DIRECTORIES(${TRILINOS_LIB_DIR})
ENDIF()


IF( ENABLE_TESTING MATCHES "ON")
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
SET(PROJECT_LIB_DIR ${CMAKE_INSTALL_PREFIX})

# include dirs used by the project
INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR}) 
INCLUDE_DIRECTORIES(${BOOST_INCLUDEDIR})
INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})
INCLUDE_DIRECTORIES(${NLOHMANN_JSON_INCL_DIR})

# use the Boost link directories
LINK_DIRECTORIES(${BOOST_LIBRARYDIR})

FILE(GLOB  SRCS  ${PROJECT_SRC_DIR}/base/*.cpp
                 ${PROJECT_SRC_DIR}/parallel/threading/*.cpp
                 ${PROJECT_SRC_DIR}/parallel/utilities/*.cpp
                 ${PROJECT_SRC_DIR}/utilities/*.cpp
                 ${PROJECT_SRC_DIR}/vehicles/*.cpp
                 ${PROJECT_SRC_DIR}/maths/functions/*.cpp
                 ${PROJECT_SRC_DIR}/maths/krylov_solvers/*.cpp
                 ${PROJECT_SRC_DIR}/maths/statistics/*.cpp
                 ${PROJECT_SRC_DIR}/maths/optimization/*.cpp
				 ${PROJECT_SRC_DIR}/maths/optimization/utils/*.cpp
                 ${PROJECT_SRC_DIR}/maths/solvers/*.cpp
                 ${PROJECT_SRC_DIR}/maths/direct_solvers/*.cpp
                 ${PROJECT_SRC_DIR}/maths/*.cpp
                 ${PROJECT_SRC_DIR}/dynamics/*.cpp
                 ${PROJECT_SRC_DIR}/numerics/*.cpp
                 ${PROJECT_SRC_DIR}/numerics/fvm/*.cpp
                 ${PROJECT_SRC_DIR}/discretization/*.cpp
                 ${PROJECT_SRC_DIR}/discretization/utils/*.cpp
                 ${PROJECT_SRC_DIR}/geometry/*.cpp
                 ${PROJECT_SRC_DIR}/patterns/*.cpp
                 ${PROJECT_SRC_DIR}/models/*.cpp
				 ${PROJECT_SRC_DIR}/data_structs/*.cpp)


ADD_LIBRARY(${LIB_NAME} SHARED ${SRCS})

SET_TARGET_PROPERTIES(kernellib PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS kernellib DESTINATION ${CMAKE_INSTALL_PREFIX})

MESSAGE( STATUS  "Installation destination at: ${CMAKE_INSTALL_PREFIX}")

# Add the examples
ADD_SUBDIRECTORY(examples)

IF( ENABLE_TESTING MATCHES "ON")
ENABLE_TESTING()
ADD_SUBDIRECTORY(tests)
ENDIF()
