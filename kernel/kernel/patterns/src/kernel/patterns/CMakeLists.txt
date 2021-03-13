IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0076 NEW)
ENDIF(COMMAND cmake_policy)

FILE(GLOB PATTERNS_SRCS ${PROJECT_SOURCE_DIR}/patterns/src/kernel/patterns/*.cpp ${PROJECT_SOURCE_DIR}/patterns/src/kernel/patterns/*/*.cpp)
TARGET_SOURCES(kernel PUBLIC ${PATTERNS_SRCS})
