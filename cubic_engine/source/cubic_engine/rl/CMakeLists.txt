
# grab all the sources from this dir
FILE(GLOB RL_SRCS ${PROJECT_SRC_DIR}/rl/*.cpp
	           ${PROJECT_SRC_DIR}/rl/*/*.cpp)

# set the target SRCS from this
# directory
target_sources(cubicengine PUBLIC ${RL_SRCS})




