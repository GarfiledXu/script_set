from email.policy import default
from fileinput import filename
from genericpath import isdir
import sys
import platform
import os
import shutil
import re
import subprocess
from this import d


script_center_dir=r"D:\VS2019\repos\PUBLICRELY\script_set\python"
script_indep_dir=r"D:\VS2019\repos\pe-face_auto_calib\script\python"
sys.path.insert(0, script_center_dir)

from jf_log import jf_log
from jf_file import *


def start_cmake_clean(input_path):
    jf_log.i("start_cmake_clean")
    jf_file.clean_all_under_dir(input_path)
    jf_log.i("end clean build dir:%s" %input_path)
    
def logic_filter():
    pass

def dict_parser():
    pass

#自定义cmake变量规则的映射
def getting_cmake_var(**kw):
    dict_new=dict()
    for k, v in kw.items():
        dict_new[k]=" -D{key}={value} ".format(key=k,value=v)
    return dict_new

def get_var_name(variable):
    globals_dict = globals()
    return [var_name for var_name in globals_dict if globals_dict[var_name] is variable]

def to_CRG(*args):
    out_str=str()
    for item in args:
        out_str=out_str +" "+ str(item) + " "
    return out_str
            
def to_CR(CR, **kw):
    out_str=str()
    for k,v in kw.items():
        out_str+=" "+str(CR.get(str(k)).get(str(v))) + " "
    return out_str
def set_CR_default(CR, CR_key, default_value):
    str_pre=str()
    str_suf=str()
    str_default=str()
    if not CR_key in CR.keys():
        jf_log.e("no key:{key}".format(key=CR_key))
    if not "default" in CR[CR_key].keys():
        jf_log.e("no key:default")
    if "prefix" in CR[CR_key].keys():
        str_pre=str(CR[CR_key]["prefix"])
    if "suffix" in CR[CR_key].keys():
        str_suf=str(CR[CR_key]["suffix"])
    str_default =" " + str_pre + default_value + str_suf + " "
    CR[CR_key]["default"]=str_default
    jf_log.i("set CR:{CR}[{key}]={value}".format(CR=get_var_name(CR), key=CR_key, value=str_default))
    return default_value

def set_CR_cmake_var(CR, **kw):
    for k,v in kw.items():
        CR["custom_add"]["add"]+=" -D{key}={value} ".format(key=str(k),value=str(v))

def generator_CR(CR, **kw):
    internal_str=str()
    for k,v in kw.items():
        internal_str+=" "+str(CR.get(str(k)).get(str(v))) + " "
    cmd="cmake " +  internal_str + " " + str(CR["custom_add"]["add"])
    CR["final_cmd"]=cmd
    return cmd

def run_CR(CR):
    cmd_key="final_cmd"
    if not   cmd_key   in CR:
        jf_log.e("error! no key {key}, in CR:{CR}".format(key=cmd_key,CR=get_var_name(CR)))
    else:
        run_cmd_and_print(CR[cmd_key])



CRG=dict(
    is_vs={
        "2019":"-G=\"Visual Studio 16 2019\"",
        "2022":"-G=\"Visual Studio 17 2022\"",
        "2017":"-G=\"Visual Studio 15 2017\"",
        "null":"",
        "default":"",
    },
    build_type={
        "d":getting_cmake_var(CMAKE_BUILD_TYPE="Debug")["CMAKE_BUILD_TYPE"],
        "r":getting_cmake_var(CMAKE_BUILD_TYPE="Realse")["CMAKE_BUILD_TYPE"],
        "default":"",
        "null":"",
    },
    arch={
        "win32":"-A=Win32 ",
        "x64":"-A=x64",       
        "default":"",
        "null":"",
    },
    toolchain={
        "null":"",
        "default":"",
    },
    src_dir={
        "null":"",
        "default":"",
        "prefix":"-S=",
        "suffix":""
    },
    build_dir={
        "null":"",
        "default":"",
        "prefix":"-B=",
        "suffix":""
    },
    exe_out_dir={
        "null":"",
        "default":"",
        "prefix":"-DEXECUTABLE_OUTPUT_PATH=",
        "suffix":" ",
    },
    custom_add={
        "null":"",
        "add":"",
    },
    final_cmd={
        "str":""
    }
)

CRB=dict(
    build_type={
        "d":"--config Debug", 
        "r":"--config Release", 
        "null":"", 
        "default":"",
    },
    build_dir={
        "null":"",
        "default":"",
        "prefix":"--build ",
        "suffix":""
    },
    custom_add={
        "null":"",
        "add":"",
    },
    final_cmd={
        "str":""
    }
  
)

def jf_main():
    set_CR_default(CRG, "src_dir",  r"D:\VS2019\repos\pe-face_auto_calib")
    str_build_dir = set_CR_default(CRG, "build_dir", r"D:\VS2019\repos\pe-face_auto_calib\build")
    set_CR_default(CRG, "exe_out_dir", r"D:\VS2019\repos\pe-face_auto_calib\out\bin")
    set_CR_default(CRB, "build_dir", r"D:\VS2019\repos\pe-face_auto_calib\build")
    
    set_CR_cmake_var(CRG, 
                    #  sys_name="windows", 
                    build_dir=str_build_dir,
                     exe_name="FACE_TOOO",
                     cmake_center=r"D:\VS2019\repos\PUBLICRELY\script_set\cmake"
                     )
    str_generator=generator_CR(CRG, 
               is_vs="2019", 
               arch="win32",
            #    build_type="d", 
               toolchain="null",
               src_dir="default",
               build_dir="default",
               exe_out_dir="default",
               custom_add="add"
               )
    str_build=generator_CR(CRB,
                build_dir="default",
                build_type="d",
                custom_add="add"
                )
    
    jf_log.t("will run cmd:{cmd}".format(cmd=str_generator))
    start_cmake_clean(str_build_dir)
    run_CR(CRG)
    jf_log.t("will run cmd:{cmd}".format(cmd=str_build))
    run_CR(CRB)
    
if __name__=="__main__":
    jf_main()