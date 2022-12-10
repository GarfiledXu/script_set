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

# 
def display_sys():
    jf_log.t("system type:"+platform.system())

#判断文件是否存在
#判断文件夹是否存在

#loguru
#[level][num:func][filename]
#jflog_i
#jflog_e
#jflog_d
#jflog_t
#jflog_w
# class MyLogger:
    # def __init__(self, log_file_path="cur.log"):
    #     self.jf_log = jf_log
    #     # 清空所有设置
    #     self.jf_log.remove()
    #     # 添加控制台输出的格式,sys.stdout为输出到屏幕;关于这些配置还需要自定义请移步官网查看相关参数说明
    #     self.jf_log.add(sys.stdout,
    #                     format="<green>{time:YYYYMMDD HH:mm:ss}</green> | "  # 颜色>时间
    #                            "{process.name} | "  # 进程名
    #                            "{thread.name} | "  # 进程名
    #                            "<cyan>{module}</cyan>.<cyan>{function}</cyan>"  # 模块名.方法名
    #                            ":<cyan>{line}</cyan> | "  # 行号
    #                            "<level>{level}</level>: "  # 等级
    #                            "<level>{message}</level>",  # 日志内容
    #                     )
    #     # 输出到文件的格式,注释下面的add',则关闭日志写入
    #     self.jf_log.add(log_file_path, level='d',
    #                     format='{time:YYYYMMDD HH:mm:ss} - '  # 时间
    #                            "{process.name} | "  # 进程名
    #                            "{thread.name} | "  # 进程名
    #                            '{module}.{function}:{line} - {level} -{message}',  # 模块名.方法名:行号
    #                     rotation="10 MB")


def main_launch():
    display_sys()
if __name__ == "__main__":
    main_launch()
    jf_file.update_root_dir(r'D:\CODE_SAPCE_WIN\script_center\python\qt_prj_dir_template_win\project_root', r'D:\CODE_SAPCE_WIN\script_center\test')

    
