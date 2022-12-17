from email.policy import default
from fileinput import filename
from genericpath import isdir
import sys
import platform
import os
import shutil
import re
import subprocess
# from this import d

#命令行解析库
#进度库
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
from jf_cmake import jf_cmake, test_main
from jf_cmake import CRG
from jf_cmake import CRB



def main():
    



    
if __name__=="__main__":
    main()
    # test_main()
    