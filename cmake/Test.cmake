#naming rule
#short name: name
#full name add prefix modification: jf_name_xxxx
#suffix modification: 
#xxx_name_


#global list
#.e.i OPENCV_MODULES_PUBLIC        
#.e.i OPENCV_MODULES_BUILD         
#.e.i OPENCV_MODULES_DISABLED_USER 
#.e.i OPENCV_MODULES_DISABLED_AUTO 
#.e.i OPENCV_MODULES_DISABLED_FORCE


macro(j_define_module VAR)
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