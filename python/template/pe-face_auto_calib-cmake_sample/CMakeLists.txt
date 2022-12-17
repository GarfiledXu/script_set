CMAKE_MINIMUM_REQUIRED(VERSION 3.4.2)

if(NOT DEFINED cmake_center)
    set(cmake_center "")
endif()

include(${cmake_center}/color.cmake)
include(${cmake_center}/module.cmake)

# ADD_DEFINITIONS(-std=c++11)
add_definitions(-w)

#cmake windows color
#当默认为空时，且无cml赋值，终止
define_var(exe_name "JF_EXE")
define_var(build_dir "")
PROJECT(${exe_name})
log_t("exe_name:${exe_name}")

add_executable(${exe_name})
set_property(TARGET ${exe_name} PROPERTY CXX_STANDARD 17)
target_link_options(${exe_name} PUBLIC "$<$<C_COMPILER_ID:MSVC>:-SAFESEH:NO>")
# WIN32
# _DEBUG
# _CONSOLE
# _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
# _CRT_SECURE_NO_WARNINGS
# _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
# _HAS_STD_BYTE=0
c_macro(${exe_name} _HAS_STD_BYTE 0 "ON")


add_subdirectory(D:/VS2019/repos/pe-face_auto_calib/src/m_alg ${build_dir}/m_alg)
add_module(${exe_name} arc_alg "ON")

add_subdirectory(D:/VS2019/repos/pe-face_auto_calib/src/m_common ${build_dir}/m_common)
add_module(${exe_name} jf_common "ON")

add_subdirectory(D:/VS2019/repos/pe-face_auto_calib/src/m_jsoncpp ${build_dir}/m_jsoncpp)
add_module(${exe_name} jsoncpp "ON")

add_subdirectory(D:/VS2019/repos/pe-face_auto_calib/src/m_ncnn ${build_dir}/m_ncnn)
add_module(${exe_name} ncnn "ON")

add_subdirectory(D:/VS2019/repos/pe-face_auto_calib/src/m_opencv ${build_dir}/m_opencv)
add_module(${exe_name} opencv4 "ON")

add_subdirectory(D:/VS2019/repos/pe-face_auto_calib/src/o_cml ${build_dir}/o_cml)
add_module(${exe_name} cml "ON")

add_subdirectory(D:/VS2019/repos/PUBLICRELY/MYTOOL ${build_dir}/MYTOOL)
add_module(${exe_name} public_mytool "ON")




