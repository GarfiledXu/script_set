from fileinput import filename
from genericpath import isdir
import sys
import platform
import os
import shutil
import re
import subprocess

# from loguru import logger


#x64_staitc
#x86_static
#x64_dynamic
#x86_dynamic

#select platform
# cmake -G "Visual Studio 16 2019" -A Win32
# cmake -G "Visual Studio 16 2019" -A x64
# cmake -G "Visual Studio 16 2019" -A ARM
# cmake -G "Visual Studio 16 2019" -A ARM64

#sepcify build type
# cmake --build {DIR} --config Release

script_center_dir=r"D:\VS2019\repos\PUBLICRELY\script_set\python"
script_indep_dir=r"D:\VS2019\repos\pe-face_auto_calib\script\python"
cur_script_dir=script_center_dir;
sys.path.insert(0, cur_script_dir);
from jf_log import jf_log
from jf_file import *


cmake_generator_platform="cmake -G Visual Studio 16 2019 -A x64"
cmake_build_type="Release"
cmake_script_center_dir=r"D:\VS2019\repos\PUBLICRELY\script_set\cmake"
cmake_build_dir=r"D:\VS2019\repos\pe-face_auto_calib\build\x64_release"
cmake_build_exe_out_dir=r"D:\VS2019\repos\pe-face_auto_calib\out\bin\x64_release"
cmake_src_dir=r"D:\VS2019\repos\pe-face_auto_calib"
cmake_tool_chain=""

def start_cmake_clean():
    jf_log.i("start_cmake_clean")
    jf_file.clean_all_under_dir(cmake_build_dir)
    jf_log.i("end clean build dir:%s" %cmake_build_dir)
    
def start_cmake_generate()->None:
    jf_log.i("start_cmake_generate ")
    cmd_str="cmake -S={src_dir} -B={generator_build_dir}  -DEXECUTABLE_OUTPUT_PATH={exe_out_dir}".format(src_dir=cmake_src_dir, generator_build_dir=cmake_build_dir, exe_out_dir=cmake_build_exe_out_dir)
    cmd_tool_chain="--toolchain={path}".format(cmake_tool_chain)
    cmd_exe_name=" -Dexe_name={name} ".format(name="FAEC_TOOL")
    jf_log.t(cmd_str+cmd_exe_name)
    run_cmd_and_print(cmd_str+cmd_exe_name)
    os.system("pause")
    jf_log.i("end_cmake_generate ")

def start_cmake_build()->None:
    jf_log.i("start_cmake_build ")
    cmd_str="cmake --build {build_dir} --config {build_type}".format(build_dir=cmake_build_dir, build_type=cmake_build_type)
    run_cmd_and_print(cmd_str)
    jf_log.t(cmd_str)
    os.system("pause")
    jf_log.i("end_cmake_generate ")

if __name__=="__main__":
    start_cmake_clean()
    start_cmake_generate()
    start_cmake_build()