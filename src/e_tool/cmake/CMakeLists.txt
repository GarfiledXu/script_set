CMAKE_MINIMUM_REQUIRED(VERSION 3.4.2)
ADD_DEFINITIONS(-std=gnu++11)
add_definitions(-w)
PROJECT(JF_PROJECT)
#路径

SET(CURRENT_PROJECT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(PROJECT_ROOT_PATH ${CURRENT_PROJECT_PATH}/..)
set(root_path ${CURRENT_PROJECT_PATH}/../..)
set(for_public_path /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/FOR-PUBLIC)
if(NOT DEFINED exe_name)
MESSAGE(WARNING "exe_name not defined:" ${exe_name})
    set(exe_name SDK_EXE)
else()

    MESSAGE(WARNING "exe_name defined:" ${exe_name})
endif()

add_subdirectory(${for_public_path}/FOR_GFLAGS  gflags_binary_dir)
add_subdirectory(${for_public_path}/FOR_GTEST gtest_binary_dir)
add_subdirectory(${for_public_path}/FOR_IMG/FFMPEG ffmpeg_binary_dir)
# add_subdirectory(${for_public_path}/FOR_IMG/LIBJPEG libjpeg_binary_dir)
# add_subdirectory(${for_public_path}/FOR_IMG/OPENCV opencv_binary_dir)
add_subdirectory(${for_public_path}/FOR_IMG/TOYCV toycv_binary_dir)
add_subdirectory(${for_public_path}/FOR_LINK2DEVICE link2device_binary_dir)
add_subdirectory(${for_public_path}/FOR_MY_TOOL my_toll_binary_dir)

add_subdirectory(${PROJECT_ROOT_PATH}/src/m_sdk sdk_binary_dir)
add_subdirectory(${PROJECT_ROOT_PATH}/src/m_mysdk my_sdk_binary_dir)
add_subdirectory(${PROJECT_ROOT_PATH}/src/m_camera for_camera_binary_dir)

# FILE(GLOB SUB_MAIN_SRC_LIST ${CURRENT_PROJECT_PATH}/*.cpp)

SET(SUM_INC_PATH 
                                        ${SUB_MYSDK_INC_PATH}
                                        ${SUB_MYTOOL_INC_PATH}
                                        ${CURRENT_PROJECT_PATH}
                                        ${SUB_GFLAGS_INC_PATH}
                                        ${SUB_GTEST_INC_PATH}
                                        ${SUB_FFMPEG_INC_PATH}
                                        # ${SUB_LIBJPEG_INC_PATH}
                                        # ${SUB_OPENCV_INC_PATH}
                                        ${SUB_CAMERA_INC_PATH}
                                        ${SUB_SDK_INC_PATH}
                                        ${SUB_TOYCV_INC_PATH}
                                        ${SUB_LINK2DEVICE_INC_PATH}                       
)
SET(SUM_SRC_LIST 
                                    ${SUB_CAMERA_SRC_PATH}
                                    ${SUB_GTEST_SRC_LIST}
                                    ${SUB_MYTOOL_SRC_LIST}
                                    ${SUB_MYSDK_SRC_LIST}
                                    # ${SUB_MAIN_SRC_LIST}        
)

SET(SUM_LIB
                            ${SUB_CAMERA_LIB_NAME}  
                            
                            ${SUB_GFLAGS_LIB_NAME}
                            ${SUB_GTEST_LIB_NAEM}
                            ${SUB_FFMPEG_LIB_NAEM}
                            # ${SUB_LIBJPEG_LIB_NAEM}
                            ${SUB_TOYCV_LIB_NAME}
                            # ${OpenCV_LIBS}
                            ${SUB_FFMPEG_LIB_NAME}
                            ${SUB_LINK2DEVICE_LIB_NAEM}
                            ${SUB_SDK_LIB_NAME}     
)

# MESSAGE(WARNING "SUM_INC_PATH:" ${SUM_INC_PATH})
# MESSAGE(WARNING "SUM_SRC_LIST:" ${SUM_SRC_LIST})
# MESSAGE(WARNING "SUM_LIB:" ${SUM_LIB})
MESSAGE(WARNING "exe_name:" ${exe_name})
INCLUDE_DIRECTORIES(${SUM_INC_PATH})
ADD_EXECUTABLE(${exe_name} ${SUM_SRC_LIST})
target_link_libraries(${exe_name} ${SUM_LIB})