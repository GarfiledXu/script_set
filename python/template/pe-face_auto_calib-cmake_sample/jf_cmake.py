from email.policy import default
from fileinput import filename
from genericpath import isdir
import sys
import platform
import os
import shutil
import re
import subprocess
from abc import ABCMeta, abstractmethod
import copy

script_center_dir=r"D:\VS2019\repos\PUBLICRELY\script_set\python"
script_indep_dir=r"D:\VS2019\repos\pe-face_auto_calib\script\python"
sys.path.insert(0, script_center_dir)

from jf_log import jf_log
from jf_file import *

#feature:
#multi structure
#modify convenient
#modify separately
#handler could be modify repeatedly
#reference git abstruct concept

#implement thought
#instance one jf_cmake obj correspond to once build
#using dict as handler to store all of generator and build information
#provide method to operator dict handler for  rapid reuse and modification
#could establish some manager method to manage or feedback procedure for all of build life cycle


#new thought
#add modify run
#copy 

#pseudo use
#jf_cmake cb_1()
# 
class jf_cmake_base(metaclass=ABCMeta):
    pname="this is an abstract class for jf_cmake"
    
    @abstractmethod
    def generator(self):
        pass
    
    @abstractmethod
    def build(self):
        pass
    
    @abstractmethod
    def clear_all_rule(self):
        pass

    
class jf_cmake(object):
    rule_ge=dict()
    rule_bu=dict()
    
    dict_gene=dict()
    dict_gene_c=dict()
    dict_build=dict()
    dict_build_c=dict()
    dict_gene_set_default=dict()
    dict_build_set_default=dict()
    
    def __init__(self, input_CRG, input_CRB) -> None:
        super().__init__()
        self.rule_ge=copy.deepcopy(input_CRG)
        self.rule_bu=copy.deepcopy(input_CRB)
        
    def get_var_name(self, variable):
        globals_dict = globals()
        return [var_name for var_name in globals_dict if globals_dict[var_name] is variable]
    
    def add_gene(self, **kw):
        cur_dict=self.dict_gene
        cur_dict.update(kw)
        jf_log.d("add dict:")
        for k,v in cur_dict.items():
            jf_log.d("[{key}]:[{value}]".format(key=k, value=v))
    def add_gene_custom(self, **kw):
        cur_dict=self.dict_gene_c
        cur_dict.update(kw)
        jf_log.d("add dict:")
        for k,v in cur_dict.items():
            jf_log.d("[{key}]:[{value}]".format(key=k, value=v))
    def add_build(self, **kw):
        cur_dict=self.dict_build
        cur_dict.update(kw)
        jf_log.d("add dict:")
        for k,v in cur_dict.items():
            jf_log.d("[{key}]:[{value}]".format(key=k, value=v))
    def add_build_custom(self, **kw):
        cur_dict=self.dict_build_c
        cur_dict.update(kw)
        jf_log.d("add dict:")
        for k,v in cur_dict.items():
            jf_log.d("[{key}]:[{value}]".format(key=k, value=v))
            
    def set_gene_default(self, **item):
        self.dict_gene_set_default=item
    def set_build_default(self, **item):
        self.dict_build_set_default=item
        
    def del_gene(self):
        self.dict_gene=dict()
    def del_gene_custom(self):
        self.dict_gene_c=dict()
    def del_build(self):
        self.dict_build=dict()
    def del_build_custom(self):
        self.dict_gene_c=dict()
    def del_gene_set_default(self):
        self.dict_gene_set_default=dict()
    def del_build_set_default(self):
        self.dict_build_set_default=dict()

    def start_impl(self, CR, dict_tag, dict_tag_c, dict_tag_set):
        ru=CR
        dict_gene=dict_tag
        dict_gene_set_default=dict_tag_set
        list_gene=list()
        list_gene_custom=list()
        jf_log.d("{dict_name}{value}".format(dict_name="dict_gene", value=dict_gene))
        jf_log.d("{dict_name}{value}".format(dict_name="dict_gene_set_default", value=dict_gene_set_default))
        for k,v in dict_gene.items():
            if str(v) == "default":
                for ke,va in dict_gene_set_default.items():
                    if str(ke) == str(k):
                        str1= str(ru[k]["prefix"]) + va + str(ru[k]["suffix"]) + " "
                        list_gene.append(str1)
                        jf_log.d("list_gene add default:{str}".format(str=str1))
            else:
                if not str(k) in ru:
                    jf_log.e("error! no key:{Key}, of dict:{dict}".format(Key=k, dict=ru))
                elif not str(v) in ru[str(k)]:
                    jf_log.e("error! no key:{Key}, of dict:{dict}".format(Key=v, dict=ru[str(k)]))
                else:
                    list_gene.append(str(ru[k][v]))
                    jf_log.d("list_gene add form:{str}".format(str=str(ru[k][v])) )
        dict_gene_c=dict_tag_c
        for k,v in dict_gene_c.items():
            list_gene_custom.append("-D{key}={value}".format(key=str(k),value=str(v)))
        for it in list_gene:
            jf_log.d("list_build:{value}".format(value=it))
        for it in list_gene_custom:
            jf_log.d("list_gene_custom:{value}".format(value=it))
        list_all=list_gene+list_gene_custom
        str_all="cmake "
        jf_log.t("run cmd:list")
        for it in list(set(list_all)):
            str_all+=" "+it+" "
            jf_log.i(it)
        str_all = re.sub(r' +', ' ', str_all)
        jf_log.t("run cmd str:{str}".format(str=str_all))
        run_cmd_and_print(str_all)
    def start_generator(self):
        self.start_impl(self.rule_ge, self.dict_gene, self.dict_gene_c, self.dict_gene_set_default)
        
    def start_build(self):
        self.start_impl(self.rule_bu, self.dict_build, self.dict_build_c, self.dict_build_set_default)

    def start_clean(self, *dir_key):
        dir_key=list(set(dir_key))
        #parse store dict
        dict_gene=self.dict_gene
        dict_gene_c=self.dict_gene
        dict_build=self.dict_gene
        dict_build_c=self.dict_gene
        dict_gene_set_default=self.dict_gene_set_default
        dict_build_set_default=self.dict_build_set_default
        list_all_dir=[dict_gene, dict_gene_c, dict_build, dict_build_c, dict_gene_set_default, dict_build_set_default]
        list_pending=list()
        serial=int()
        for list_t in list_all_dir:
            for k,v in list_t.items():
                for dst_key in dir_key:
                    if k==dst_key and v != "default":
                        serial=serial+1
                        list_pending.append(v)
                        jf_log.d("add clean item:[{sei},{key}]:[{value}]".format(sei=serial, key=str(k), value=v))
        list_pending=list(set(list_pending))
        for clean in list_pending:
            jf_log.i("start_cmake_clean")
            jf_file.clean_all_under_dir(clean)
            jf_log.i("end clean build dir:%s" %clean)
    def print(self):
        pass
def test_main():
    # jf_cmake make1(CRG, CRB)
    make1=jf_cmake(CRG, CRB)
    make1.add_gene(
        is_vs="2019", 
        arch="win32",
        #build_type="d", 
        toolchain="null",
        src_dir="default",
        build_dir="default",
        exe_out_dir="default",
    )
    make1.set_gene_default(
        src_dir=r"D:\VS2019\repos\pe-face_auto_calib",
        build_dir=r"D:\VS2019\repos\pe-face_auto_calib\build",
        exe_out_dir=r"D:\VS2019\repos\pe-face_auto_calib\out\bin",
    )
    make1.add_gene_custom(
        build_dir=r"D:\VS2019\repos\pe-face_auto_calib\build",
        exe_name="FACE_TOOO",
        cmake_center=r"D:\VS2019\repos\PUBLICRELY\script_set\cmake"
    )
    make1.add_build(
        build_dir="default",
        build_type="d",
    )
    make1.add_build_custom(
        
    )
    make1.set_build_default(
        build_dir=""r"D:\VS2019\repos\pe-face_auto_calib\build",
    )
    make1.start_clean("build_dir")
    make1.start_generator()
    make1.start_build()
    
    jf_log.t("end of main")


CRG=dict(
    is_vs={
        "2019":"-G=\"Visual Studio 16 2019\"",
        "2022":"-G=\"Visual Studio 17 2022\"",
        "2017":"-G=\"Visual Studio 15 2017\"",
        "null":"",
        "default":"",
    },
    build_type={
        "d":"-DCMAKE_BUILD_TYPE=Debug",
        "r":"-DCMAKE_BUILD_TYPE=Realse",
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
)

    
if __name__=="__main__":
    test_main()