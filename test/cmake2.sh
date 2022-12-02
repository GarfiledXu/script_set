#! /usr/bin/bash

source $bash_path/log.sh
source $bash_path/file.sh

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
    if [[ $# -ne 6 ]]
    then 
        log_e "need input 6 param: toolchain_dir qnx_source_dir build_dir generate_dir cmake_src_dir exe_name"
        log_t "exit"
        exit 1
    fi
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

# func_run_cmake \
# /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/FOR-PUBLIC/FOR_QNX_SDP/qnx.toolchain.cmake \
# /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/FOR-PUBLIC/FOR_QNX_SDP/qnx700_sdp/qnxsdp-env.sh \
# /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src-test/cmake/build \
# /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/exe/TEST_TOOL \
# /home/xjf2613/code-space/QNX/qnx-sdk-project-20220228/pe-hezhong_ep40/src-test/cmake \
# SDK_TEST_API_${version_string}
