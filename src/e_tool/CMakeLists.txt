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
# SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

# set(SUB_main_M_INC_PATH ${CURRENT_PROJECT_PATH})


#  FILE(GLOB SUB_main_M_SRC_LIST ${CURRENT_PROJECT_PATH}/*.cpp)
# set(SUB_main_M_INC_PATH ${SUB_main_M_INC_PATH} CACHE INTERNAL "xxxx")
# set(SUB_main_M_SRC_LIST ${SUB_main_M_SRC_LIST} CACHE INTERNAL "xxxx")



set(module_tag main_M)
set(sub_inc_path SUB_${module_tag}_INC_PATH)
set(sub_src_list SUB_${module_tag}_SRC_LIST)
#子模块路径
SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(${sub_inc_path} ${CURRENT_PROJECT_PATH})


FILE(GLOB ${sub_src_list} ${CURRENT_PROJECT_PATH}/*.cpp)
set(${sub_inc_path} ${${sub_inc_path}} CACHE INTERNAL "xxxx")
set(${sub_src_list} ${${sub_src_list}} CACHE INTERNAL "xxxx")