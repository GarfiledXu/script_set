#build type:release d
#build platform:win linux linux_arm qnx mac
#python 调用本地exe，并获取程序输出
#python 实现管道调用
#python 调用bash脚本，batch脚本
#python 进度条，日志
#python 文件操作
#字符串处理
#获取当前路径

from fileinput import filename
from genericpath import isdir
import sys
import platform
import os
import shutil
import re

from jf_log import jf_log
from jf_file import jf_file
#获取当前文件绝对路径
g_cur_py_abpath=os.path.abspath(__file__)
#提取文件所在目录
g_logger_path=os.path.dirname(g_cur_py_abpath)
#路径拼接

#判断文件夹是否存在

#判断文件是否存在

#追加形式写入文件 bbbbbbbbbb b566666666666666666789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789789423errr
 
#删除单个文件或者空目录

#删除目录下所有文件

#python 定义结构体来表示文件树形结构，再进行整体文件操作

#python 数据结构嵌套

#自判断生成目录，根据代码描述的模板(每次目录修改需要该代码，而且不容易可视化)
# 或者是已经存在的目录模板(容易修改，可读性强)

#python 递归遍历文件




def display_sys():
    jf_log.t("system type:"+platform.system())


def main_launch():
    display_sys()
    #jf_file.update_root_dir(r'D:\CODE_SAPCE_WIN\script_center\python\qt_prj_dir_template_win\project_root', r'D:\CODE_SAPCE_WIN\script_center\test')
    
    
if __name__ == "__main__":
    main_launch()
    jf_file.update_root_dir(r'D:\CODE_SAPCE_WIN\script_center\python\qt_prj_dir_template_win\project_root', r'D:\CODE_SAPCE_WIN\script_center\test')
    #linux test
    # jf_file.update_root_dir(r'/home/fei/CodeSpaceWsl/script_set/python/qt_prj_dir_template_win/project_root', r'/home/fei/CodeSpaceWsl/script_set/test')
    

    
