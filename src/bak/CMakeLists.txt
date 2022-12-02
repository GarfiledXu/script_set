#最低版本号
#编译器标准
#项目名称
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ////                  FOR MY SDK CMAKELISTS
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMAKE_MINIMUM_REQUIRED(VERSION 3.4.2)
ADD_DEFINITIONS(-std=gnu++11)
PROJECT(JF_PROJECT)

#子模块路径
SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(SUB_MYSDK_INC_PATH ${CURRENT_PROJECT_PATH})


FILE(GLOB SUB_MYSDK_SRC_LIST ${CURRENT_PROJECT_PATH}/*.cpp)
set(SUB_MYSDK_INC_PATH ${SUB_MYSDK_INC_PATH} CACHE INTERNAL "xxxx")
set(SUB_MYSDK_SRC_LIST ${SUB_MYSDK_SRC_LIST} CACHE INTERNAL "xxxx")


