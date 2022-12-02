#!usr/bin/bash

# set timeout 1
color_blue_bold="\033[1;34m"
color_purpl_bold="\033[1;35m"
color_red_bold="\033[1;31m"
color_green_bold="\033[1;36m"
color_yellow_bold="\033[1;33m"
color_white_bold="\033[1;37m"

color_purpl="\033[35m"
color_blue="\033[34m"
color_green="\033[36m"
color_yellow="\033[33m"
color_red="\033[31m"
color_white="\033[37m"

color_clear="\033[0m"

time_now=$(date +"%Y-%m-%d %H:%M:%S")


# spawn su root
# expect "Password:"
# send "xjf2613\r"
current_path="$( cd "$( dirname "$0" )" && pwd )"
parent_path="$( cd ${current_path}/../..  && pwd )"
parent_name=$(basename ${parent_path})
version_string=${parent_name##*^}
project_string1=${parent_name%^*}
project_string2=${project_string1#*-}
# dir_exe="../exe"
dir_src_sub=${current_path}/..
dir_cmake=${dir_src_sub}/cmake
dir_exe=${dir_src_sub}/../exe
dir_exe_api_test=${dir_exe}/API_TEST
dir_exe_test_tool=${dir_exe}/TEST_TOOL
dir_exe_zbak=${dir_exe}/ZBAK
dir_cmake_build=${dir_cmake}/build
# dir_cmake_build="./build"
list_dir_exe=(${dir_exe} ${dir_exe_api_test} ${dir_exe_test_tool} ${dir_exe_zbak} ${dir_cmake_build})

# if [[ -d ${dir_cmake_build} ]]; then rm -rf ${dir_cmake_build}; fi
# if [[ -d ${dir_exe_api_test} ]]; then cp -rf ${dir_exe_api_test} ${dir_exe_zbak};rm -rf ${dir_exe_api_test} ; fi



# pause
# start build
if [[ $1 == "test" || $1 == "TEST" ]]
then
    new_exe=${dir_exe_api_test}/SDK_TEST_API_${version_string}
    type=TEST
    if [[ -d ${dir_exe_api_test} ]]; then cp -rf ${dir_exe_api_test} ${dir_exe_zbak};rm -rf ${dir_exe_api_test} ; fi
elif [[ $1 == "tool" || $1 == "TOOL" ]]
then 
    new_exe=${dir_exe_test_tool}/SDK_TEST_TOOL_${version_string}
    type=TOOL
    if [[ -d ${dir_exe_test_tool} ]]; then cp -rf ${dir_exe_test_tool} ${dir_exe_zbak};rm -rf ${dir_exe_test_tool} ; fi
else
   echo -e "${color_green}[error] ${color_red_bold}you input [$1], should type [test/TEST] or [tool/TOOL]${color_clear}"
   exit
fi

function func_dir_exist(){
    if [[ ! -d $1 ]]
    then 
        echo -n dir $1 not exist! 
        echo [ create ]
        mkdir $1
    else
        echo dir $1 exsit
    fi
}
#$1 file
#$2 bool decide if exit when file not exist, default is 1
function func_is_file_exist(){
    if [[  -z $1 ]] || [[ ! -f $1 ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}can't find file ${1}${color_clear}"
        if [[ -z $2 ]] 
        then
            echo -e "$color_red_bold exit $color_clear"
            exit -1
        elif [[ ! -z $2 ]] && [[ $2 -eq 1 ]]
        then
            echo -e "$color_red_bold exit $color_clear"
            exit -1
        fi
    fi
}
    
#$1 string dir path
#$2 bool decide if build dir when dir not exist, default not
#$3 bool decide if exit when dir not exist, default is
#agc can't be 2 , if agc != 3, using all default value
function func_is_dir_exist(){
    local va_2=0
    local va_3=1
    if [[ ! -d $1 ]] || [[ -z $1 ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}can't find dir ${1}${color_clear}"

        if [[ $# == 3 ]]
        then
            va_2=$2
            va_3=$3
            if [[ va_2 -eq 1 ]]
            then
                echo -e "$color_red_bold rebulid dir $1"
                mkdir $1
                return 0
            fi
            if [[ va_2 -eq 0 ]] && [[ va_3 -eq 1 ]]
            then
                exit -1
            fi
        else
            exit -1
        fi 
    else
        echo -e "${color_green}dir $1 exist"
    fi
}
# dir path
function func_clear_dir(){
    if [[ -z $1 ]] || [[ ! -d $1 ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}can't find file ${1}${color_clear}" 
        exit -1
    fi
    if [[ ! -z $1 ]] && [[ -d $1 ]]
    then
    echo -e "$color_red_bold rm dir $1 $color_clear"
        rm -rf $1
    fi
}


#toolchain path
#source path
#build path
#generate binary path
#cmake src path
#exename
#step1:clean build dir
#step2:
function func_run_cmake(){
    func_is_file_exist $1
    func_is_file_exist $2
    func_is_dir_exist $3
    func_is_dir_exist $4
    func_is_dir_exist $5
    
    local toolchain_dir=$1
    local qnx_source_dir=$2
    local build_dir=$3
    local generate_dir=$4
    local cmake_src_dir=$5
    local exe_name=$6

    if [[ -z $6 ]]
    then
        exe_name=default
    fi
    echo exe_name:$exe_name
    func_clear_dir $build_dir
    source $qnx_source_dir
    cmake -S=$cmake_src_dir \
    -B=$build_dir --toolchain=$toolchain_dir \
    -DEXECUTABLE_OUTPUT_PATH=$generate_dir \
    -Dexe_name=$exe_name

    cmake --build $build_dir
}

for item in ${list_dir_exe[@]}
do
    func_dir_exist ${item}
done



func_run_cmake \
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/FOR-PUBLIC/FOR_QNX_SDP/qnx.toolchain.cmake \
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/FOR-PUBLIC/FOR_QNX_SDP/qnx700_sdp/qnxsdp-env.sh \
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/build \
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/out/bin \
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40 \
SDK_TOOL_${version_string}

