# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ////                  FOR CAMERA
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ////                  date:2022.02.28-13:35
# /////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMAKE_MINIMUM_REQUIRED(VERSION 3.4.2)
ADD_DEFINITIONS(-std=gnu++11)
PROJECT(JF_PROJECT)

#子模块路径
SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

SET(SUB_CAMERA_INC_PATH      ${CURRENT_PROJECT_PATH}/inc )
SET(SUB_CAMERA_LIB_PATH      ${CURRENT_PROJECT_PATH}/lib)
SET(SUB_CAMERA_LIB_NAME     
                                                    ${SUB_CAMERA_LIB_PATH}/libais_client.so
                                                    ${SUB_CAMERA_LIB_PATH}/liblibstd.so
                                                    ${SUB_CAMERA_LIB_PATH}/libmmap_peer.so
                                                    ${SUB_CAMERA_LIB_PATH}/libOSAbstraction.so
                                                    ${SUB_CAMERA_LIB_PATH}/libpmem_client.so
                                                    ${SUB_CAMERA_LIB_PATH}/libpmemext.so         
)


# INCLUDE_DIRECTORIES(${SUB_CAMERA_INC_PATH})
# LINK_DIRECTORIES(${SUB_CAMERA_LIB_PATH})
# LINK_LIBRARIES(${SUB_CAMERA_LIB_NAME})


#add_library(SUB_CAMERA_LIB ${SUB_CAMERA_LIB_NAME})

set(SUB_CAMERA_INC_PATH ${SUB_CAMERA_INC_PATH} CACHE INTERNAL "xxxx")
set(SUB_CAMERA_LIB_PATH ${SUB_CAMERA_LIB_PATH} CACHE INTERNAL "xxxx")
set(SUB_CAMERA_LIB_NAME ${SUB_CAMERA_LIB_NAME} CACHE INTERNAL "xxxx")
