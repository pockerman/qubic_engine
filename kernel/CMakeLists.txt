CMAKE_MINIMUM_REQUIRED(VERSION 3.6)
MESSAGE(STATUS "Using CMake ${CMAKE_VERSION}")

PROJECT(kernel CXX)

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
ADD_LIBRARY(%s SHARED ""
SET_TARGET_PROPERTIES(%s PROPERTIES LINKER_LANGUAGE CXX)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/base/src/kernel/base/)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/data_structs/src/kernel/data_structs/)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/geometry/src/kernel/geometry/)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/maths/src/kernel/maths/)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/parallel/src/kernel/parallel/)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/patterns/src/kernel/patterns/)
ADD_SUBDIRECTORY(${PROJECT_SRC_DIR}/utilities/src/kernel/utilities/)

ADD_LIBRARY(kernel SHARED ${SRCS})
SET_TARGET_PROPERTIES(kernel PROPERTIES LINKER_LANGUAGE CXX)
INSTALL(TARGETS kernel DESTINATION ${CMAKE_INSTALL_PREFIX})
MESSAGE(STATUS "Installation destination at: ${CMAKE_INSTALL_PREFIX}")
