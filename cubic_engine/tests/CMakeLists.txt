cmake_minimum_required(VERSION 3.0)
ADD_SUBDIRECTORY(test_astar)
ADD_SUBDIRECTORY(test_array_stats)
ADD_SUBDIRECTORY(test_knn_classifier)
ADD_SUBDIRECTORY(test_confusion_matrix)
ADD_SUBDIRECTORY(test_pure_persuit_tracker)
ADD_SUBDIRECTORY(test_waypoint_path)
ADD_SUBDIRECTORY(test_unscented_kalman_filter)
ADD_SUBDIRECTORY(test_rrt)
INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})
LINK_DIRECTORIES(${PROJECT_LIB_DIR})


