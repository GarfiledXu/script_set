# FATAL_ERROR：cmake出错，停止编译和生成(信息红色)
# SEND_ERROR：cmake出错，继续编译，但是停止生成(信息红色)
# WARNING：cmake警告，继续编译(信息红色)
# AUTHOR_WARNING：开发者警告，继续编译(信息红色)
# DEPRECATION：如果使用set方法设置CMAKE_ERROR_DEPRECATED为true(不区分大小写)，编译出错，否则继续编译
# (none) or NOTICE：不设置mode，默认是NOTICE模式，不影响编译和生成，用于打印消息(信息白色)
# STATUS：编译时状态信息，左边以--开头(信息白色)
# DEBUG：针对开发人员的调试信息(信息白色)
# TRACE：日志级别的临时信息(信息白色)

#if cmd defined
#if set default
macro(jf_set_co_va VAR)
    if(NOT DEFINED ${VAR})
        if("x${VARN}" STREQUAL "x")
            message(FATAL_ERROR "cmd not defined and you are not set default value for ${VAR}")
        else()
            set(${VAR} ${ARGN})
        endif()
    else()
        if("${VAR}" STREQUAL "")
        message(FATAL_ERROR "cmd defined but no value for ${VAR}")
        endif()
    endif()
endmacro()


# stable & safe duplicates removal macro
macro(_list_unique __lst)
  if(${__lst})
    list(REMOVE_DUPLICATES ${__lst})
  endif()
endmacro()