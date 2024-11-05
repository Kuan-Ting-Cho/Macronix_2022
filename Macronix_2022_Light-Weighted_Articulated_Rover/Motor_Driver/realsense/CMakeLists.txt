# License: Apache 2.0. See LICENSE file in root directory.
# Copyright(c) 2019 Intel Corporation. All Rights Reserved.
cmake_minimum_required(VERSION 3.5)

project(realsense)
find_package(realsense2 REQUIRED)

add_library(realsense SHARED rsdis.cpp)
set_property(TARGET realsense PROPERTY CXX_STANDARD 11)
target_link_libraries(realsense ${DEPENDENCIES} realsense2)
