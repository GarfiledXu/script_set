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
    list_generator=list()
    list_generator_custom=list()
    list_build=list()
    list_build_custom=list()
    list_clean_key=list()
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
        jf_log.i("add dict:")
        for k,v in cur_dict.items():
            jf_log.i("[{key}]:[{value}]".format(key=k, value=v))
            
        
    def set_gene_default(self, **item):
        self.dict_gene_set_default=item
        # for k,v in item.items():
        #     CR_key=k
        #     CR=self.rule_ge
        #     default_value=v
        #     str_pre=str()
        #     str_suf=str()
        #     str_default=str()
        #     if not CR_key in CR.keys():
        #         jf_log.e("no key:{key}".format(key=CR_key))
        #     if not "default" in CR[CR_key].keys():
        #         jf_log.e("no key:default")
        #     if "prefix" in CR[CR_key].keys():
        #         str_pre=str(CR[CR_key]["prefix"])
        #     if "suffix" in CR[CR_key].keys():
        #         str_suf=str(CR[CR_key]["suffix"])
        #     str_default =" " + str_pre + default_value + str_suf + " "
        #     CR[CR_key]["default"]=str_default
        #     jf_log.i("set CR:{CR}[{key}]={value}".format(CR=self.get_var_name(CR), key=CR_key, value=str_default))
    def set_build_default(self, **item):
        self.dict_build_set_default=item
        # for k,v in item.items():
        #     CR_key=k
        #     CR=self.rule_bu
        #     default_value=v
        #     str_pre=str()
        #     str_suf=str()
        #     str_default=str()
        #     if not CR_key in CR.keys():
        #         jf_log.e("no key:{key}".format(key=CR_key))
        #     if not "default" in CR[CR_key].keys():
        #         jf_log.e("no key:default")
        #     if "prefix" in CR[CR_key].keys():
        #         str_pre=str(CR[CR_key]["prefix"])
        #     if "suffix" in CR[CR_key].keys():
        #         str_suf=str(CR[CR_key]["suffix"])
        #     str_default =" " + str_pre + default_value + str_suf + " "
        #     CR[CR_key]["default"]=str_default
        #     jf_log.i("set CR:{CR}[{key}]={value}".format(CR=self.get_var_name(CR), key=CR_key, value=str_default))
    def del_gene(self):
        self.list_generator=list()
        # self.str_generator=str()

    def add_gene_custom(self, **kw):
        cur_dict=self.dict_gene_c
        cur_dict.update(kw)
        jf_log.i("add dict:")
        for k,v in cur_dict.items():
            jf_log.i("[{key}]:[{value}]".format(key=k, value=v))
        
    def del_gene_custom(self):
        self.list_generator_custom=list()
        # self.str_generator_custom=str()
        
    #create 
    #key match success, repalce
    #key match fail, add
    def modify_gene(self, **kw):
        
        for k,v in kw.items():
            new_str=str()
            if not str(k) in self.rule_ge:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(key=k, dict=self.rule_ge))
            elif not str(v) in self.rule_ge[str(k)]:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(key=v, dict=self.rule_ge[str(k)]))
            else:
                new_str=str(self.rule_ge[k][v])
            for i in range(len(self.list_generator)):
                if re.match(r"-.{key}.s".format(key=k), self.list_generator[i], 0) != None:
                    self.list_generator[i]=new_str
                else:
                    self.list_generator.append(new_str)
        
    def modify_gene_custom(self, **kw):
        for k,v in kw.items():
            new_str="-D{key}={value}".format(key=k, value=v)
            for i in range(len(self.list_generator_custom)):
                if re.match(r"-D{key}.".format(key=k), self.list_generator_custom[i], 0) != None:
                    self.list_generator_custom[i]=new_str
                else:
                    self.list_generator_custom.append(new_str)

    def add_build(self, **kw):
        cur_dict=self.dict_build
        cur_dict.update(kw)
        jf_log.i("add dict:")
        for k,v in cur_dict.items():
            jf_log.i("[{key}]:[{value}]".format(key=k, value=v))
       
        
    def del_build(self):
        self.list_build=list()
    def add_build_custom(self, **kw):
        cur_dict=self.dict_build_c
        cur_dict.update(kw)
        jf_log.i("add dict:")
        for k,v in cur_dict.items():
            jf_log.i("[{key}]:[{value}]".format(key=k, value=v))
            
       
        
    def del_build_custom(self):
        self.list_build_custom=list()

    def modify_build(self, **kw):
        for k,v in kw.items():
            new_str=str()
            if not str(k) in self.rule_bu:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(key=k, dict=self.rule_bu))
            elif not str(v) in self.rule_bu[str(k)]:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(key=v, dict=self.rule_bu[str(k)]))
            else:
                new_str=str(self.rule_bu[k][v])
            for i in range(len(self.list_build)):
                if re.match(r"-.{key}.s".format(key=k), self.list_build[i], 0) != None:
                    self.list_build[i]=new_str
                else:
                    self.list_build.append(new_str)
        
    def modify_build_custom(self, **kw):
        for k,v in kw.items():
            new_str="-D{key}={value}".format(key=k, value=v)
            for i in range(len(self.list_build_custom)):
                if re.match(r"-D{key}.".format(key=k), self.list_build_custom[i], 0) != None:
                    self.list_build_custom[i]=new_str
                else:
                    self.list_build_custom.append(new_str)
    
    def start_generator(self):
        for k,v in self.dict_gene.items():
            if not str(k) in self.rule_ge:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(key=k, dict=self.rule_ge))
            elif not str(v) in self.rule_ge[str(k)]:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(key=v, dict=self.rule_ge[str(k)]))
            else:
                self.list_generator.append(str(self.rule_ge[k][v]))
                # self.str_generator+=" "+str(self.rule_ge[k][v]) + " "
        for k,v in self.dict_gene_c.items():
            self.list_generator_custom.append("-D{key}={value}".format(key=str(k),value=str(v)))
            # self.str_generator_custom+=" "+"-D{key}={value}".format(key=str(k),value=str(v))
        # clean_key=self.list_clean_key
        # self.internal_clean(clean_key)
        
        str_cmd="cmake"
        str_cmd_custom=str()
        for it in self.list_generator:
           str_cmd+=" "+it+" "
        for it in self.list_generator_custom:
            str_cmd_custom+=" " + it + " "
        str_cmd+=str_cmd_custom
        jf_log.t("run cmd:{cmd}".format(cmd=str_cmd))
        run_cmd_and_print(str_cmd)
        
    def start_build(self):
        for k,v in self.dict_build.items():
            if not str(k) in self.rule_bu:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(Key=k, dict=self.rule_bu))
            elif not str(v) in self.rule_bu[str(k)]:
                jf_log.e("error! no key:{Key}, of dict:{dict}".format(Key=v, dict=self.rule_bu[str(k)]))
            else:
                self.list_build.append(str(self.rule_bu[k][v]))
        for k,v in self.dict_build_c.items():
            self.list_build_custom.append("-D{key}={value}".format(key=str(k),value=str(v)))
            
        str_cmd="cmake"
        str_cmd_custom=str()
        for it in self.list_build:
            str_cmd+=" "+it+" "
        for it in self.list_build_custom:
            str_cmd_custom+=" " + it + " "
        str_cmd+=str_cmd_custom
        jf_log.t("run cmd:{cmd}".format(cmd=str_cmd))
        run_cmd_and_print(str_cmd)

        
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
                        jf_log.i("add clean item:[{sei},{key}]:[{value}]".format(sei=serial, key=str(k), value=v))
        list_pending=list(set(list_pending))
        for clean in list_pending:
            jf_log.i("start_cmake_clean")
            jf_file.clean_all_under_dir(clean)
            jf_log.i("end clean build dir:%s" %clean)
   
def test_main():
    cmake1=jf_cmake(CRG, CRB)      
    
    cmake1.add_gene(   
        is_vs="2019", 
        arch="x64",
        #build_type="d", 
        toolchain="null",
        src_dir="default",
        build_dir="default",
        exe_out_dir="default",
    )
    cmake1.add_gene(
        arch="win32",
    )
    cmake1.set_gene_default(
        src_dir=r"D:\VS2019\repos\pe-face_auto_calib",
        build_dir=r"D:\VS2019\repos\pe-face_auto_calib\build",
        exe_out_dir=r"D:\VS2019\repos\pe-face_auto_calib\out\bin",
    )
    cmake1.add_gene_custom(
        build_dir=r"D:\VS2019\repos\pe-face_auto_calib\build",
        exe_name="FACE_TOOO",
        cmake_center=r"D:\VS2019\repos\PUBLICRELY\script_set\cmake"
    )
    
    cmake1.add_build(
        build_dir="default",
        build_type="d",
    )
    cmake1.set_build_default(
        build_dir=""r"D:\VS2019\repos\pe-face_auto_calib\build",
    )
    cmake1.add_build_custom(
        
    )
    cmake1.start_clean("build_dir", "build_dir")
    # cmake1.start_generator()
    # cmake1.start_build()
    
    
# def start_cmake_clean(input_path):
#     jf_log.i("start_cmake_clean")
#     jf_file.clean_all_under_dir(input_path)
#     jf_log.i("end clean build dir:%s" %input_path)
    
# def logic_filter():
#     pass

# def dict_parser():
#     pass

# #自定义cmake变量规则的映射
# def getting_cmake_var(**kw):
#     dict_new=dict()
#     for k, v in kw.items():
#         dict_new[k]=" -D{key}={value} ".format(key=k,value=v)
#     return dict_new

# def get_var_name(variable):
#     globals_dict = globals()
#     return [var_name for var_name in globals_dict if globals_dict[var_name] is variable]

# def to_CRG(*args):
#     out_str=str()
#     for item in args:
#         out_str=out_str +" "+ str(item) + " "
#     return out_str
            
# def to_CR(CR, **kw):
#     out_str=str()
#     for k,v in kw.items():
#         out_str+=" "+str(CR.get(str(k)).get(str(v))) + " "
#     return out_str
# def set_CR_default(CR, CR_key, default_value):
#     str_pre=str()
#     str_suf=str()
#     str_default=str()
#     if not CR_key in CR.keys():
#         jf_log.e("no key:{key}".format(key=CR_key))
#     if not "default" in CR[CR_key].keys():
#         jf_log.e("no key:default")
#     if "prefix" in CR[CR_key].keys():
#         str_pre=str(CR[CR_key]["prefix"])
#     if "suffix" in CR[CR_key].keys():
#         str_suf=str(CR[CR_key]["suffix"])
#     str_default =" " + str_pre + default_value + str_suf + " "
#     CR[CR_key]["default"]=str_default
#     jf_log.i("set CR:{CR}[{key}]={value}".format(CR=get_var_name(CR), key=CR_key, value=str_default))
#     return default_value

# def set_CR_cmake_var(CR, **kw):
#     for k,v in kw.items():
#         CR["custom_add"]["add"]+=" -D{key}={value} ".format(key=str(k),value=str(v))

# def generator_CR(CR, **kw):
#     internal_str=str()
#     for k,v in kw.items():
#         internal_str+=" "+str(CR.get(str(k)).get(str(v))) + " "
#     cmd="cmake " +  internal_str + " " + str(CR["custom_add"]["add"])
#     CR["final_cmd"]=cmd
#     return cmd

# def run_CR(CR):
#     cmd_key="final_cmd"
#     if not   cmd_key   in CR:
#         jf_log.e("error! no key {key}, in CR:{CR}".format(key=cmd_key,CR=get_var_name(CR)))
#     else:
#         run_cmd_and_print(CR[cmd_key])




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
# def jf_main():
#     set_CR_default(CRG, "src_dir",  r"D:\VS2019\repos\pe-face_auto_calib")
#     str_build_dir = set_CR_default(CRG, "build_dir", r"D:\VS2019\repos\pe-face_auto_calib\build")
#     set_CR_default(CRG, "exe_out_dir", r"D:\VS2019\repos\pe-face_auto_calib\out\bin")
#     set_CR_default(CRB, "build_dir", r"D:\VS2019\repos\pe-face_auto_calib\build")
    
#     set_CR_cmake_var(CRG, 
#                     #  sys_name="windows", 
#                     build_dir=str_build_dir,
#                      exe_name="FACE_TOOO",
#                      cmake_center=r"D:\VS2019\repos\PUBLICRELY\script_set\cmake"
#                      )
#     str_generator=generator_CR(CRG, 
#                is_vs="2019", 
#                arch="win32",
#             #    build_type="d", 
#                toolchain="null",
#                src_dir="default",
#                build_dir="default",
#                exe_out_dir="default",
#                custom_add="add"
#                )
#     str_build=generator_CR(CRB,
#                 build_dir="default",
#                 build_type="d",
#                 custom_add="add"
#                 )
    
#     jf_log.t("will run cmd:{cmd}".format(cmd=str_generator))
#     start_cmake_clean(str_build_dir)
#     run_CR(CRG)
#     jf_log.t("will run cmd:{cmd}".format(cmd=str_build))
#     run_CR(CRB)
    
if __name__=="__main__":
    # jf_main()
    test_main()