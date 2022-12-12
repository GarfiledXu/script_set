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



#装饰器编码基本思路
#不管装饰器本身要不要带参数，最终的效果都是，装饰器函数需要返回一个接受目标被包装函数为参数的函数
#即decorator(input_func)，在装饰器带参数的场景下，第一层形参为装饰器自用函数，第二层函数形参就该为decorator(input_func)
#函数形式，如此在内层嵌套的函数依次回溯到最外层时，最终返回的函数对象格式就是decorator(input_func)
#需要包装目标函数参数，就定义一层嵌套函数，将新定义的函数形参作为生成参数传入
def decorator_test(decorator_tag):
    def decorator_out(in_func):
        def decorator_imp(*args, **kw):
            print("behind run, tag:" + decorator_tag)
            return in_func(*args, **kw)
        return decorator_imp
    return decorator_out


def decorator_test2():
    def decorator_in(input_func):
        def decorator_imp(*args, **kw):
            print("behind run")
            return input_func(*args, **kw)
        return decorator_imp
    return decorator_in

def decorator_test3(input_func):
    def func_pkg_args(*args, ** kw):
        print("behind run")
        return input_func(*args, ** kw)
    return func_pkg_args

def decorator_test4(second_input):
#  test_variable_parameter=decorator_test(in_func, tag)
@decorator_test("hello-tag")
def test_variable_parameter(*args, **kw)->None:
    jf_log.i(args)
    jf_log.i(kw)
    
@decorator_test2()
def test_variable_parameter2(*args, **kw)->None:
    jf_log.i(args)
    jf_log.i(kw)

@decorator_test3
def test_variable_parameter_3(*args, **kw)->None:
    jf_log.i(args)
    jf_log.i(kw)
# def cmake_cfg():
def display_sys():
    jf_log.t("system type:"+platform.system())


def main_launch():
    display_sys()
    test_variable_parameter("param1", 4, "param3", xjf=2323, dfjdkf="df")
    test_variable_parameter2("param1", 4, "param3", xjf=2323, dfjdkf="df")
    test_variable_parameter_3("param1", 4, "param3", xjf=2323, dfjdkf="df")
if __name__ == "__main__":
    main_launch()
    #linux test
    # jf_file.update_root_dir(r'/home/fei/CodeSpaceWsl/script_set/python/qt_prj_dir_template_win/project_root', r'/home/fei/CodeSpaceWsl/script_set/test')
    # jf_file.update_root_dir(r'D:\CODE_SAPCE_WIN\script_center\python\qt_prj_dir_template_win\project_root', r'D:\CODE_SAPCE_WIN\script_center\test')
    

    
