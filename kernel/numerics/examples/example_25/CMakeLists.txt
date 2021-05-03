cmake_minimum_required(VERSION 3.0)
PROJECT(example_27 CXX)
SET(SOURCE example_27.cpp)
SET(EXECUTABLE  example_27)

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

SET(CMAKE_CXX_FLAGS "-std=c++17 -g -pthread -fopenmp -fPIC")

INCLUDE_DIRECTORIES(/home/alex/MySoftware/blaze-3.8/install/include)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/discretization/src)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/numerics/src/)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/base/src)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/data_structs/src)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/geometry/src)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/maths/src)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/parallel/src)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/patterns/src)
INCLUDE_DIRECTORIES(/home/alex/qi3/ce_numerics/kernel/kernel/utilities/src)
INCLUDE_DIRECTORIES(/home/alex/MySoftware/trilinos/install/include)

LINK_DIRECTORIES(/home/alex/qi3/ce_numerics/install)
LINK_DIRECTORIES(/home/alex/MySoftware/gtest/install/lib)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})
LINK_DIRECTORIES(/home/alex/MySoftware/trilinos/install/lib)

ADD_EXECUTABLE(${EXECUTABLE} ${SOURCE})

TARGET_LINK_LIBRARIES(${EXECUTABLE} kernel_numerics)
TARGET_LINK_LIBRARIES(${EXECUTABLE} kernel)
TARGET_LINK_LIBRARIES(${EXECUTABLE} kernel_discretization)
TARGET_LINK_LIBRARIES(${EXECUTABLE} pthread)
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)
TARGET_LINK_LIBRARIES(${EXECUTABLE} epetra)
TARGET_LINK_LIBRARIES(${EXECUTABLE} aztecoo)
TARGET_LINK_LIBRARIES(${EXECUTABLE} amesos)
