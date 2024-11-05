cmake_minimum_required(VERSION 3.5)

set(DYNAMIXEL_PROJECT_NAME Dynamixel_Motor)
set(DYNAMIXEL_BUILD_NAME Dynamixel_Motor)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
set(CMAKE_BUILD_TYPE Debug)

project(${DYNAMIXEL_PROJECT_NAME})
add_subdirectory(MotorUnion)
add_subdirectory(XIMU)
add_subdirectory(realsense)

include_directories(${DYNAMIXEL_BUILD_NAME} ${MotorUnion_SOURCE_DIR})
include_directories(${DYNAMIXEL_BUILD_NAME} ${XIMU_SOURCE_DIR})
include_directories(${DYNAMIXEL_BUILD_NAME} ${realsense_SOURCE_DIR})

#link_directories(${XIMU_SOURCE_DIR})

add_executable(${DYNAMIXEL_BUILD_NAME} example.cpp)
target_link_libraries(${DYNAMIXEL_BUILD_NAME} MotorUnion)
target_link_libraries(${DYNAMIXEL_BUILD_NAME} XIMU)
target_link_libraries(${DYNAMIXEL_BUILD_NAME} realsense)
