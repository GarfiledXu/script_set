#! /usr/bin/bash

source $bash_path/log.sh

#$1 file
#$2 bool decide if exit when file not exist, default is 1
function func_is_file_exist(){
    local file=$1
    local is_exit=$2
    if [[  -z $1 ]] || [[ ! -f $1 ]]
    then
        log_e "can't find file $file"
        if [[ -z $2 ]] 
        then
            log_i "exit"
            exit -1
        elif [[ ! -z $2 ]] && [[ $2 -eq 1 ]]
        then
            log_i "exit"
            exit -1
        fi
    fi
}

#$1 string dir path
#$2 bool decide if build dir when dir not exist, default not
#$3 bool decide if exit when dir not exist, default is
#agc can't be 2 , if agc != 3, using all default value
function func_is_dir_exist(){
    local file=$1
    local is_rebuild=0
    local is_exit=1
    if [[ ! -d $1 ]] || [[ -z $1 ]]
    then
        # echo -e "${color_green}[error] ${color_red_bold}can't find dir ${1}${color_clear}"
        log_e "can't find dir ${1}"
        if [[ $# == 3 ]]
        then
            is_rebuild=$2
            is_exit=$3
            if [[ is_rebuild -eq 1 ]]
            then
                # echo -e "$color_red_bold rebulid dir $1"
                log_t "rebuild dir ${1}"
                mkdir $1
                return 0
            fi
            if [[ is_rebuild -eq 0 ]] && [[ is_exit -eq 1 ]]
            then
                log_i "exit"
                exit -1
            fi
        else
            log_i "exit"
            exit -1
        fi 
    else
        log_i "dir exist $1"
        # echo -e "${color_green}dir $1 exist"
    fi
}

# dir path
function func_clear_dir(){
    local dir=$1
    if [[ -z $1 ]] || [[ ! -d $1 ]]
    then
        log_e "can't find dir $1"
        # echo -e "${color_green}[error] ${color_red_bold}can't find file ${1}${color_clear}" 
        exit -1
        log_i "exit"
    fi
    if [[ ! -z $1 ]] && [[ -d $1 ]]
    then
        log_i "rm dir $1"
        # echo -e "$color_red_bold rm dir $1 $color_clear"
        rm -rf $1
    fi
}

function func_update_folder(){
    local build_dir=$1
    if [[ -z $1 ]]
    then
        build_dir="build"
    fi

    if [[ ! -d $build_dir ]]
    then
        mkdir ${build_dir}
        echo ${build_dir}
        return 0
    else
        build_dir=$build_dir"1"
        func_update_folder ${build_dir}
        return 0
    fi
}

function func_add_suffix_for_eachelement {
    local i=0
    # declare -a temp_array
    local temp_array=()
    local accept_array_name=$1[@]
    for item in ${!accept_array_name}
    do
        temp_array+=("$item$2 ")
        i=$((i + 1))
    done
    echo ${temp_array[@]}
}

function func_print_array {
    local i=0
    local accept_array_name=$1[@]
    echo ">> echo array $1:"
    for item in ${!accept_array_name[@]}
    do 
        echo [$i]$item
        i=$((i+1))
    done
    echo "<< echo end"
}

function func_array_relink {
    local i=0
    local accept_src_name=$1[@]
    local accept_src=${!accept_src_name}
    local accept_des_name=$2[@]
    local accept_des=${!accept_des_name}
    
    local new_des=()
    for item in ${accept_des[@]}
    do
        # echo test:$item
       new_des+=($item)
    done
   
    for tt in ${accept_src[@]}
    do
        # echo link[$i]:$tt"==>>"${new_des[$i]}#test
        echo link[$i]">>"$tt">>"${new_des[$i]}
        ln -sf $tt ${new_des[$i]}
        i=$((i+1))
    done
}


# array_link_lib_name_1=(
#     "libopencv_calib3d.so" 
#     "libopencv_core.so" 
#     "libopencv_features2d.so" 
#     "libopencv_flann.so" 
#     "libopencv_highgui.so" 
#     "libopencv_imgcodecs.so" 
#     "libopencv_imgproc.so" 
#     "libopencv_ml.so" 
#     "libopencv_objdetect.so" 
#     "libopencv_photo.so" 
#     "libopencv_shape.so" 
#     "libopencv_stitching.so" 
#     "libopencv_superres.so" 
#     "libopencv_video.so" 
#     "libopencv_videoio.so" 
#     "libopencv_videostab.so" 
# )

# declare -a array_origin_lib_3_2_0=()
# declare -a array_link_lib_name_3_2=()

# array_link_lib_name_3_2=$( func_add_suffix_for_eachelement array_link_lib_name_1 ".3.2" )
# array_origin_lib_3_2_0=$( func_add_suffix_for_eachelement array_link_lib_name_1 ".3.2.0" )

# func_print_array array_link_lib_name_3_2
# echo " "
# func_print_array array_origin_lib_3_2_0

# func_array_relink array_origin_lib_3_2_0 array_link_lib_name_1
# echo " "
# func_array_relink array_origin_lib_3_2_0 array_link_lib_name_3_2


# func_clear_dir /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/bash/testdir

# func_is_dir_exist \
#  /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/script/bash/testdir \
#  1 \
#  0


