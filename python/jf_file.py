from genericpath import isdir
from pickle import NONE
import sys
import platform
import os
import shutil
import re
import subprocess
from jf_log import jf_log


def replace_root_path_1(src_root, dst_root, src_path):
    relate_str = os.path.abspath(src_path).replace(os.path.abspath(src_root), '')
    return os.path.abspath(dst_root) + relate_str
def replace_root_path_2(src_root, dst_root, src_dir, src_name):
    src_path = os.path.join(os.path.abspath(src_dir), src_name)
    relate_str = os.path.abspath(src_path).replace(os.path.abspath(src_root), '')
    return os.path.abspath(dst_root) + relate_str

def update_template_dir(src_root_path, dst_root_path):
    i=0
    jf_log.d("src_root_path:" + src_root_path)
    jf_log.d("dst_root_path:" + dst_root_path)
    for root, dir_list, file_list in os.walk(src_root_path, topdown = True):
        i = i + 1
        jf_log.d("iterate time: %d, root: %s" % (i, root))
        dest_1path= replace_root_path_1(src_root_path, dst_root_path, root)
        jf_log.i("root dst_path:"+dest_1path)
        jf_log.i("root src_path:"+root)
        for d in dir_list:
            dst_path=replace_root_path_2(src_root_path, dst_root_path, root, d)
            jf_log.i("dir_dst_path:"+dst_path)
            jf_log.i("dir_src_path:"+os.path.join(root, d))
            # jf_log.i("relate path:" + relate_path)
            if not os.path.isdir(dst_path):
               os.mkdir(dst_path)            
        for f in file_list:
            dst_path = dst_path=replace_root_path_2(src_root_path, dst_root_path, root, f)
            jf_log.i("file_dst_path:"+dst_path)
            jf_log.i("file_src_path:"+os.path.join(root, f))
            if not os.path.isfile(dst_path):
                shutil.copyfile(os.path.join(root, f), dst_path)
                
def clean_all_under_dir(dst_dir):
    for root, dirs, files in os.walk(dst_dir, topdown=False):
        for name in files:
            os.remove(os.path.join(root, name))
        for name in dirs:
            os.rmdir(os.path.join(root, name))
            
def run_cmd_and_print(dst_str):
    res = subprocess.Popen(dst_str)
    res.wait()
    # print("", res.stdout.read()) 
class jf_file_:
    def __init__(self)->None:
        jf_file_.version="0.1"
    def update_root_dir(self, src_root_path, dst_root_path)->None:
        update_template_dir(src_root_path, dst_root_path)
    def clean_all_under_dir(self, dst_dir)->None:
        clean_all_under_dir(dst_dir)

jf_file = jf_file_()