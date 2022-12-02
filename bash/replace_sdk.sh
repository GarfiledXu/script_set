#! /usr/bin/bash

#move current file to contrast dir
#decompress current compress package
#check same name file if exist
#cp new file to target folder and contrast dir

#pretreatment
#package name rule
#target file name and relate path 
#target folder 
bash_path=/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/bash
source $bash_path/log.sh
source $bash_path/file.sh
source $bash_path/string.sh
function parse_array_name(){
    if [[ $# -gt 1 ]]
    then
        log_e "parse_array_name input gt 1 param!"
        log_t "exit"
        exit 0
    fi
    local array_name=$1[@]
    local array_src=${!array_name}
    echo ${array_src[*]}
}

#check source file is file
#check target file dir is exist

function move_file_list(){
    local temp_source_list=$(parse_array_name $1)
    local temp_destination_list=$(parse_array_name $2)
    local source_list=()
    local destination_list=()
    for tt in ${temp_source_list[@]}
    do
        # log_i test:tt:${tt}
        source_list+=($tt)
        # log_i "test:source_list:${source_list[*]}"
    done

    for ttt in ${temp_destination_list[@]}
    do
        # log_i test:ttt:${ttt}
        destination_list+=($ttt)
        # log_i "test:destination_list:${destination_list[*]}"
    done
    
    # local source_list=()
    # for item in ${temp_source_list}
    # do
    #     # echo test:$item
    #    source_list+=($item)
    # done

    local source_nm=$(func_get_array_itemnum source_list)
    local destination_nm=$(func_get_array_itemnum destination_list)

    # log_t "source_nm:$source_nm"
    # log_t "destination_nm:$destination_nm"
    

    if [[ ${source_nm} -ne ${destination_nm} ]]
    then
        log_e "source list num:$source_nm not equal destination list num:${destination_nm}!"
        log_t "exit"
        exit 1
    fi

    for source_it in ${source_list[@]}
    do
        # log_e $source_it
        func_is_file_exist $source_it
    done

    for destination_it in ${destination_list[@]}
    do
        # log_e $destination_it
        local file_dir=$(func_get_file_dir $destination_it)
        # log_e $file_dir
        func_is_dir_exist $file_dir
    done

    local i=0
    for sourcet in ${source_list[@]}
    do
        # log_e $it
        # log_t "loop: $i"
        func_is_file_exist ${source_list[$i]}
        # log_e "source:${source_list[$i]}"
        # log_e "destination_list:${destination_list[$i]}"
        mv -f ${source_list[i]} ${destination_list[$i]}
        func_is_file_exist ${destination_list[$i]}
        i=$((i+1))
    done
}

function cp_file_list(){
    local temp_source_list=$(parse_array_name $1)
    local temp_destination_list=$(parse_array_name $2)
    local source_list=()
    local destination_list=()
    for tt in ${temp_source_list[@]}
    do
        # log_i test:tt:${tt}
        source_list+=($tt)
        # log_i "test:source_list:${source_list[*]}"
    done

    for ttt in ${temp_destination_list[@]}
    do
        # log_i test:ttt:${ttt}
        destination_list+=($ttt)
        # log_i "test:destination_list:${destination_list[*]}"
    done
    
    # local source_list=()
    # for item in ${temp_source_list}
    # do
    #     # echo test:$item
    #    source_list+=($item)
    # done

    local source_nm=$(func_get_array_itemnum source_list)
    local destination_nm=$(func_get_array_itemnum destination_list)

    # log_t "source_nm:$source_nm"
    # log_t "destination_nm:$destination_nm"
    

    if [[ ${source_nm} -ne ${destination_nm} ]]
    then
        log_e "source list num:$source_nm not equal destination list num:${destination_nm}!"
        log_t "exit"
        exit 1
    fi

    for source_it in ${source_list[@]}
    do
        # log_e $source_it
        func_is_file_exist $source_it
    done

    for destination_it in ${destination_list[@]}
    do
        # log_e $destination_it
        local file_dir=$(func_get_file_dir $destination_it)
        # log_e $file_dir
        func_is_dir_exist $file_dir
    done

    local i=0
    for sourcet in ${source_list[@]}
    do
        # log_e $it
        # log_t "loop: $i"
        func_is_file_exist ${source_list[$i]}
        # log_e "source:${source_list[$i]}"
        # log_e "destination_list:${destination_list[$i]}"
        rm -f ${destination_list[$i]} 
         cp -f ${source_list[i]} ${destination_list[$i]}
        func_is_file_exist ${destination_list[$i]}
        i=$((i+1))
    done
}

function cp_file(){
    if [[ $# -ne 2 ]]
    then
        log_e "need input 2 param"
        log_t "exit"
        exit 1
    fi
    local src_file=$1
    local des_file=$2
    func_is_file_exist $src_file
    func_is_dir_exist  $(func_get_file_dir $des_file)
    rm -f $des_file 
    cp -f $src_file $des_file
}
function mv_file(){
    if [[ $# -ne 2 ]]
    then
        log_e "need input 2 param"
        log_t "exit"
        exit 1
    fi
    local src_file=$1
    local des_file=$2
    func_is_file_exist $src_file
    func_is_dir_exist  $(func_get_file_dir $des_file)
    mv -f $src_file $des_file
}
# list_1="
#     /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/bash/argv.sh
#     /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/bash/cmake.sh
# "
# list_2="
#     /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/test/argv1.sh
#     /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/test/cmake2.sh
# "
# move_file_list list_1 list_2
# move_file_list list_2 list_1
# cp_file_list list_1 list_2

#move list cp list
#clear dir mk dir
#decompress
#get file path from package dir
#cp list

 pkg_root="/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk/package"
 bak="$pkg_root/bak"
 contrast_dir="$pkg_root/contrast"
if [[ -d $bak ]]
then
    local parent_dir=$(parent_dir $bak)
    if [[ ! -d $parent_dir ]]
    then
        log_e "error input bak parent dir not exist!"
        log_t "exit"
        exit 1
    else
        mkdir $bak
    fi
fi

if [[ -d $contrast_dir ]]
then
    rm -rf $contrast_dir/*
else
    mkdir $contrast_dir
fi

inc_lib_bak_name=inc_lib_bak
 current_mv_dir=$(func_update_folder "$pkg_root/$inc_lib_bak_name")
#  current_mv_dir=$pkg_root/$current_mv_dir_name
 src_list="
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk/inc/ArcDriveSDK.h
/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk/lib/libArcDriveSdk.so
"
 des_list="
    ${current_mv_dir}/ArcDriveSDK.h
    ${current_mv_dir}/libArcDriveSdk.so
"
if [[ ! -d $current_mv_dir ]]
then
    mkdir $current_mv_dir
fi

# contrast_src_list="
# /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk/inc/ArcDriveSDK.h
# "
# contrast_des_list="
#     ${contrast_dir}/old.h
# "


#get version get file name by find to get an latest file  | find  grep sort
#decompress
#mv

# find $pkg_root -name "*_ARCSOFT_DriveSDK_*_QNX_AARCH64LE*.zip" | sort -n -r

pkg_file=$(func_get_array_first $(find $pkg_root -name "*_ARCSOFT_DriveSDK_*_QNX_AARCH64LE*.zip" | sort -n -r))
unzip_dir=$pkg_root/temp_dir
if [[ -d $unzip_dir ]]
then
    rm -rf $unzip_dir/*
fi
func_is_dir_exist $unzip_dir 1 0

unzip -d $unzip_dir $pkg_file
# pkg_dir=${pkg_file%.*}
pkg_dir=$unzip_dir
func_is_dir_exist $pkg_dir

pkg_inc_file=$(func_get_array_first $(find $pkg_dir -name "ArcDriveSDK.h" | sort -n -r))
pkg_lib_file=$(func_get_array_first $(find $pkg_dir -name "libArcDriveSdk.so" | sort -n -r))


# log_t "pkg_inc_file:$pkg_inc_file"
# log_t "pkg_lib_file:$pkg_lib_file"
des_sdk_root_dir="/home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src/m_sdk"
des_sdk_inc_dir=$des_sdk_root_dir/inc
des_sdk_lib_dir=$des_sdk_root_dir/lib
des_sdk_inc_file=$(func_get_array_first $(find $des_sdk_inc_dir -name "ArcDriveSDK.h" | sort -n -r))
des_sdk_lib_file=$(func_get_array_first $(find $des_sdk_lib_dir -name "libArcDriveSdk.so" | sort -n -r))

cp_unzip_file_src_list="$pkg_inc_file $pkg_lib_file"
cp_unzip_file_des_list="$des_sdk_inc_dir/$(func_get_file_name $pkg_inc_file) $des_sdk_lib_dir/$(func_get_file_name $pkg_lib_file)"





#transform contrast
#inc lib bak
#update inc lib
#1
log_t "start transform contrast fold content"
cp_file $pkg_inc_file $contrast_dir/$(func_get_file_name $pkg_inc_file)
cp_file $des_sdk_inc_file $contrast_dir/old.h

#2
log_t "start inc lib bak"
cp_file_list src_list des_list

#3
# log_t "cp_unzip_file_src_list:"
# log_t "$cp_unzip_file_src_list"
# log_t "cp_unzip_file_des_list:"
# log_t "$cp_unzip_file_des_list"
log_t "start update inc lib"
cp_file_list cp_unzip_file_src_list cp_unzip_file_des_list