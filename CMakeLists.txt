cmake_minimum_required(VERSION 2.8.9)
project(cloudrone)

find_package(catkin REQUIRED roscpp message_generation sensor_msgs image_transport cv_bridge)

find_package(Qt5Core)
find_package(Qt5Sql)

set(CMAKE_AUTOMOC ON)
set(CMAKE_BUILD_TYPE RelWithDebugInfo)

SET(CMAKE_CXX_FLAGS "-std=c++0x -fPIC")

add_service_files(DIRECTORY server/srv)
add_message_files(DIRECTORY server/msg)
generate_messages(DEPENDENCIES std_msgs sensor_msgs)

catkin_package(INCLUDE_DIRS server/include)
include_directories(server)
include_directories(server/include)

#worker_node

file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS
  server/include/worker_node/*.h)
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS
  server/src/worker_node/*.cpp
)

add_executable(worker_node ${QT_SOURCES} ${QT_MOC_HPP})
add_dependencies(worker_node ${PROJECT_NAME}_generate_messages_cpp)
qt5_use_modules(worker_node Core Sql)
target_link_libraries(worker_node ${catkin_LIBRARIES} ${QT_LIBRARIES})

#marker_detection_node

add_executable(marker_detection_node server/src/marker_detection_node/marker_detection_node.cpp)
add_dependencies(marker_detection_node ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(marker_detection_node ${catkin_LIBRARIES})

#control_node

file(GLOB CONTROL_NODE_SOURCES server/src/control_node/*.cpp)

add_executable(control_node ${CONTROL_NODE_SOURCES})
add_dependencies(control_node ${PROJECT_NAME}_generate_messages_cpp)
target_link_libraries(control_node ${catkin_LIBRARIES} ${OpenCV_LIBS})