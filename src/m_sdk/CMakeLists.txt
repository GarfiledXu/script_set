# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ////                  FOR SDK
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ////                  date:2022.02.24
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMAKE_MINIMUM_REQUIRED(VERSION 3.4.2)
ADD_DEFINITIONS(-std=gnu++11)
PROJECT(JF_PROJECT)

#子模块路径
SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(SUB_SDK_INC_PATH ${CURRENT_PROJECT_PATH}/inc)
set(SUB_SDK_LIB_PATH ${CURRENT_PROJECT_PATH}/lib)

set(SUB_SDK_LIB_NAME 
                                # ${SUB_SDK_LIB_PATH}/libArcDriveSdk.so 
                                ${SUB_SDK_LIB_PATH}/libArcDriveSdk.so
                                ${SUB_SDK_LIB_PATH}/libcdsprpc.so
                                ${SUB_SDK_LIB_PATH}/libfastrpc_pmem.so
                                ${SUB_SDK_LIB_PATH}/liblibstd.so
                                ${SUB_SDK_LIB_PATH}/libsmmu_client.so
                                #  ${SUB_SDK_LIB_PATH}/libneat_1_1_skel.so
                                # ${SUB_SDK_LIB_PATH}/libcommon_base.so           
)

set(SUB_SDK_INC_PATH ${SUB_SDK_INC_PATH} CACHE INTERNAL "xxxx")
set(SUB_SDK_LIB_PATH ${SUB_SDK_LIB_PATH} CACHE INTERNAL "xxxx")
set(SUB_SDK_LIB_NAME ${SUB_SDK_LIB_NAME} CACHE INTERNAL "xxxx")