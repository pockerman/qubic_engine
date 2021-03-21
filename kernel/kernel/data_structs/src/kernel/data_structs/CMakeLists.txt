IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0076 NEW)
ENDIF(COMMAND cmake_policy)

FILE(GLOB DATA_STRUCTS_SRCS ${PROJECT_SOURCE_DIR}/data_structs/src/kernel/data_structs/*.cpp ${PROJECT_SOURCE_DIR}/data_structs/src/kernel/data_structs/*/*.cpp)
TARGET_SOURCES(kernel PUBLIC ${DATA_STRUCTS_SRCS})
