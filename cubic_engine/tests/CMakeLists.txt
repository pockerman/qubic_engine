cmake_minimum_required(VERSION 3.0)
ADD_SUBDIRECTORY(test_astar)
ADD_SUBDIRECTORY(test_array_stats)
ADD_SUBDIRECTORY(test_knn_classifier)
ADD_SUBDIRECTORY(test_confusion_matrix)
INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})
LINK_DIRECTORIES(${PROJECT_LIB_DIR})


