
# grab all the sources from this dir
FILE(GLOB SRCS ${PROJECT_SRC_DIR}/maths/*.cpp)

# set the target SRCS from this
# directory
target_sources(cubicengine PUBLIC ${SRCS})




