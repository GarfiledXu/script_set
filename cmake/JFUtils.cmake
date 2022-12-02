#macro[generate variable]
#argv:variable, default value, not predefined using default info/using predefined value
#check if predefined
#no predefined and then set value by default /if no default set, terminal
#output info
macro(jf_set_gvariable var_name)

if (NOT DEFINED var_name)
set(var_name )
endif()
endmacro()


#macro[generate sourcecode mcaro]
#方式1：直接通过add_defined
#方式2：通过configure file进行
#最后生成控制变量，以及默认值，提供给外层控制

#cache variable 使用定义前需要消除unset normal变量，normal变量优先级更高

#generate sub module global info

#reference sub module global info
#与add subdirectory一起打包，同样生成控制变量以及默认值，提供给外层控制

#源文件内容修改
#编译信息记录：文件缓存信息，每次构建都自增信息，最后写入源文件某个关联模块的宏，而源文件将检查宏存在，若是则打印(或者直接打印)，这样就能回溯并确认运行程序的编译信息
#args:编译日期+时间，ID自增，项目目录
#方式：通过文件交互，源文件读取目标文件, 但本质上并没有写入源文件，编译加入
#方式：通过源文件生成，cmakelist生成源文件，再项目引入生成的源文件，编译时加入


#color
#message([<mode>] "message")
#<ERROR|WARNING|NOTICE|STATUS|VERBOSE|DEBUG|TRACE>
set(M_I )
