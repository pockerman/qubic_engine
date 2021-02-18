
# grab all the sources from this dir
FILE(GLOB SRCS ${PROJECT_SRC_DIR}/ml/*.cpp
	       ${PROJECT_SRC_DIR}/ml/*/*.cpp
	       ${PROJECT_SRC_DIR}/ml/instance_learning/utils/*.cpp
	       ${PROJECT_SRC_DIR}/ml/supervised_learning/utils/*.cpp
	       ${PROJECT_SRC_DIR}/ml/unsupervised_learning/utils/*.cpp)

# set the target SRCS from this
# directory
target_sources(cubicengine PUBLIC ${SRCS})




