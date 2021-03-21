IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0076 NEW)
ENDIF(COMMAND cmake_policy)

FILE(GLOB GEOMETRY_SRCS ${PROJECT_SOURCE_DIR}/geometry/src/kernel/geometry/*.cpp ${PROJECT_SOURCE_DIR}/geometry/src/kernel/geometry/*/*.cpp)
TARGET_SOURCES(kernel PUBLIC ${GEOMETRY_SRCS})
