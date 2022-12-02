#!/usr/bin/bash
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


function func_putfile(){
    local local_path=
    local qnx_path=$1
    local android_path=$2
    local ip=$3
    if [[ -z $qnx_path ]] || [[ -z $android_path ]] || [[ -z $ip ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}3 param can't be empty ${1}${color_clear}"
    fi
    shell_str="su busybox ftpput -u root $ip $qnx_path $android_path"
    adb shell $shell_str
}



function func_login(){
    local ip=$1
    if [[ -z $ip ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}1 param can't be empty ${1}${color_clear}"
    fi
    busybox telnet $ip
}
function func_get_exe_name(){
    local exe_path=$1
    if [[ -z $exe_path ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}1 param can't be empty ${1}${color_clear}"
    fi
    local exe_name=${exe_path##*/}
    echo $exe_name
}

function func_push_file(){
    local local_file=$1
    local android_path=$2
    local qnx_path=$3
    local ip=$4
    local exe_name=$( func_get_exe_name $local_file )
    if [[ -z $local_file ]] || [[ -z $android_path ]] || [[ -z $qnx_path ]] || [[ -z $ip ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}4 param can't be empty ${1}${color_clear}"
    fi
    local str_adb_cm="adb.exe push $local_file $android_path"
    local str_adb_shell_cm=" busybox ftpput -u root --pass=sv2655888 $ip $qnx_path/$exe_name $android_path/$exe_name"
    $str_adb_cm
    adb.exe shell $str_adb_shell_cm
}
function func_get_file(){
    local local_path=$1
    local android_path=$2
    local qnx_file=$3
    local ip=$4
    local exe_name=$( func_get_exe_name $qnx_file )
    # echo exe_naem:$exe_name
    # echo local_path:$local_path
    # echo android_path:$android_path
    # echo qnx_file:$qnx_file
    if [[ -z $local_path ]] || [[ -z $android_path ]] || [[ -z $qnx_file ]] || [[ -z $ip ]]
    then
        echo -e "${color_green}[error] ${color_red_bold}4 param can't be empty ${1}${color_clear}"
    fi
    local str_adb_cm="adb.exe pull  $android_path/$exe_name  $local_path"
    local str_adb_shell_cm=" busybox ftpget  -u root $ip  $android_path/$exe_name $qnx_file"
    # echo -e "${color_green} $str_adb_shell_cm ${color_clear}"
    adb.exe shell $str_adb_shell_cm
    $str_adb_cm
}

local_root_path_2=/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/out/temp_dir
qnx_root_path_2=/var/xjf2613/
android_root_path_2=/data/xjf2613
if [[ -z $1 ]] 
then
    echo -e "${color_green}[error] ${color_red_bold}1 param can't be empty ${1}${color_clear}"
elif [[ $1 == pushexe ]]
    then
        echo -e "${color_green}[start pushexe] ${color_clear}"
        func_push_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/out/bin/SDK_EXE" \
         $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pushtool ]]
    then
        echo -e "${color_green}[start pushexe] ${color_clear}"
        func_push_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/exe/TEST_TOOL/SDK_TEST_TOOL_1.0.401235219.59" \
         $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pushlib ]]
    then
        echo -e "${color_green}[start pushexe] ${color_clear}"
        func_push_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk/lib/libArcDriveSdk.so" \
       $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pushlicense ]]
    then
    echo -e "${color_green}[start pushlicense] ${color_clear}"
    func_push_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/out/temp_dir/license.dat" \
         $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pulltxt ]]
    then
    echo -e "${color_green}[start pushlicense] ${color_clear}"
    func_get_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/exe/TEST_TOOL/" \
        "/data/xby9951" \
        "/var/xby9951/mem_recorder.txt" \
        "192.168.8.1"
elif [[ $1 == pushcfg ]]
    then
    echo -e "${color_green}[start pushcfg] ${color_clear}"
        func_push_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk/switch_set.json" \
         $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pushtar ]]
    then
    echo -e "${color_green}[start pushtar] ${color_clear}"
        func_push_file ${local_root_path_2}/EP40-TOOL.tar \
        $android_root_path_2 \
        ${qnx_root_path_2}/bin \
        "192.168.8.1"
elif [[ $1 == pushimg ]]
    then
    echo -e "${color_green}[start pushimg] ${color_clear}"
        func_push_file ${local_root_path_2}/image.tar \
        $android_root_path_2 \
        ${qnx_root_path_2}/image \
        "192.168.8.1"
elif [[ $1 == pushneat ]]
    then
    echo -e "${color_green}[start pushimg] ${color_clear}"
        func_push_file ${local_root_path_2}/libneat_1_1_skel.so \
        $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pusharccfg ]]
    then
    echo -e "${color_green}[start pushimg] ${color_clear}"
        func_push_file ${local_root_path_2}/arc_cfg.json \
        $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
elif [[ $1 == pullresult ]]
    then
    echo -e "${color_green}[start pullresult] ${color_clear}"
    func_get_file ${local_root_path_2} \
        $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL/RESULT/oms_alarm_result.txt \
        "192.168.8.1"
elif [[ $1 == pushtaskcfg ]]
    then
    echo -e "${color_green}[start pushexe] ${color_clear}"
        func_push_file "/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/out/temp_dir/task_cfg.json" \
         $android_root_path_2 \
        ${qnx_root_path_2}/bin/EP40-TOOL \
        "192.168.8.1"
else 
    echo -e "${color_green}[error] ${color_red_bold}input error [${1}]${color_clear}"
fi
