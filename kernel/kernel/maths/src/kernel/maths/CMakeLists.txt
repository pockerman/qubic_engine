IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0076 NEW)
ENDIF(COMMAND cmake_policy)

FILE(GLOB MATHS_SRCS ${PROJECT_SOURCE_DIR}/maths/src/kernel/maths/*.cpp ${PROJECT_SOURCE_DIR}/maths/src/kernel/maths/*/*.cpp)
TARGET_SOURCES(kernel PUBLIC ${MATHS_SRCS})
