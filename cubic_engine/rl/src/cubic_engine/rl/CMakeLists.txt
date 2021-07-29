IF(COMMAND cmake_policy)
	CMAKE_POLICY(SET CMP0076 NEW)
ENDIF(COMMAND cmake_policy)

FILE(GLOB SRCS "/home/alex/qi3/ce_rl/cubic_engine/rl/src/cubic_engine/rl/*.cpp" "/home/alex/qi3/ce_rl/cubic_engine/rl/src/cubic_engine/rl/*/*.cpp"  "/home/alex/qi3/ce_rl/cubic_engine/rl/src/cubic_engine/rl/algorithms/dp/*.cpp" "/home/alex/qi3/ce_rl/cubic_engine/rl/src/cubic_engine/rl/algorithms/mc/*.cpp" "/home/alex/qi3/ce_rl/cubic_engine/rl/src/cubic_engine/rl/algorithms/td/*.cpp")
TARGET_SOURCES(cengine_rl PUBLIC ${SRCS})
