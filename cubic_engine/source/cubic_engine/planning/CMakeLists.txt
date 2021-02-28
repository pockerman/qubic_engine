
# grab all the sources from this dir
FILE(GLOB PLANNING_SRCS ${PROJECT_SRC_DIR}/planning/*.cpp)

# set the target SRCS from this
# directory
TARGET_SOURCES(cubicengine PUBLIC ${PLANNING_SRCS})




