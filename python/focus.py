#build type:release debug
#build platform:win linux linux_arm qnx mac
#python 调用本地exe，并获取程序输出
#python 实现管道调用
#python 调用bash脚本，batch脚本
#python 进度条，日志
#python 文件操作
#字符串处理
#获取当前路径
import sys
import platform
import os
from loguru import logger

#获取当前文件绝对路径
g_cur_py_abpath=os.path.abspath(__file__)
#提取文件所在目录
g_logger_path=os.path.dirname(g_cur_py_abpath)
#路径拼接

#判断文件夹是否存在

#判断文件是否存在

#追加形式写入文件

#删除单个文件或者空目录

#删除目录下所有文件

#python 定义结构体来表示文件树形结构，再进行整体文件操作

#python 数据结构嵌套

#自判断生成目录，根据代码描述的模板(每次目录修改需要该代码，而且不容易可视化)
# 或者是已经存在的目录模板(容易修改，可读性强)

#python 递归遍历文件

#
def display_sys():
    logger.info("system type:"+platform.system())

class pre_dir:
    def __init__(self, tp_root_path, dst_root_path) -> None:
        pre_dir.tp_root_path=tp_root_path
        logger.info("setting tp_root_path:"+tp_root_path)
        logger.info("setting dst_root_path:"+dst_root_path)
    def instance_pre(self)->None:
        logger.info("start instance")
        logger.info("end instance")
    
logger.info("setting logger path:"+g_logger_path)
def main_launch():
    display_sys()
    pre = pre_dir("script\\qt_prj_dir_template_win\\project_root", os.path.dirname(os.path.abspath(__file__)))
    pre.instance_pre()
if __name__ == "__main__":
    main_launch()
    
