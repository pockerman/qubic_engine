IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0076 NEW)
ENDIF(COMMAND cmake_policy)

FILE(GLOB SRCS ${PROJECT_SOURCE_DIR}/src/cubic_engine/rl/*.cpp ${PROJECT_SOURCE_DIR}/src/cubic_engine/rl/*/*.cpp)
TARGET_SOURCES(cengine_rl PUBLIC ${SRCS})
