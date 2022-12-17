
CMAKE_MINIMUM_REQUIRED(VERSION 3.4.2)
# ADD_DEFINITIONS(-std=c++11)
PROJECT(${exe_name})
include(${cmake_center}/color.cmake)
include(${cmake_center}/module.cmake)
SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
FILE(GLOB SUB_MYSDK_SRC_LIST ${CURRENT_PROJECT_PATH}/*.cpp)

generate_module("cml" 
    SP_INC 
        ${CURRENT_PROJECT_PATH}
    SP_LIB 
      "NON"
    SP_SRC 
        ${SUB_MYSDK_SRC_LIST})

# print_module(internal)

