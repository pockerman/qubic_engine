cmake_minimum_required(VERSION 3.0)

PROJECT(Example CXX)
SET(SOURCE exe.cpp)
SET(EXECUTABLE  exe_7)
SET(Chrono_DIR "/home/david/MyProjects/cubic_engine/chrono_lib/chrono/install/lib64/cmake")

find_package(Chrono
             COMPONENTS Irrlicht
             CONFIG)

if (NOT Chrono_FOUND)
    message("=====================================================")
    message("ERROR: ")
    message("Could not find Chrono or one of its required modules")
    message("=====================================================")
    return()
endif()

INCLUDE_DIRECTORIES(${PROJECT_INCL_DIR})
INCLUDE_DIRECTORIES(${BLAZE_INCL_DIR})
INCLUDE_DIRECTORIES(${CHRONO_INCLUDE_DIRS})

LINK_DIRECTORIES(${PROJECT_LIB_DIR})
LINK_DIRECTORIES(${KERNEL_LIB_DIR})

ADD_EXECUTABLE(${EXECUTABLE} ${SOURCE})

TARGET_LINK_LIBRARIES(${EXECUTABLE} kernellib)
TARGET_LINK_LIBRARIES(${EXECUTABLE} openblas)
TARGET_LINK_LIBRARIES(${EXECUTABLE} pthread)

TARGET_LINK_LIBRARIES(${EXECUTABLE} ${CHRONO_LIBRARIES}  ChronoEngine
                      ChronoEngine_irrlicht
                      ChronoEngine_vehicle
                      ChronoModels_vehicle)
