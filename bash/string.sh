#! /usr/bin/bash

#https://stackoverflow.com/questions/2264428/how-to-convert-a-string-to-lower-case-in-bash
source $bash_path/log.sh
source $bash_path/file.sh
source $bash_path/cmake.sh

#all to c
#all to l
#c and l exchange

function func_letter_all_to_c(){
    echo "${@^^}"
}
function func_letter_all_to_l(){
    echo "${@,,}"
}
function func_letter_exchange(){
    echo "${@~~}"
}

function func_get_file_dir(){
    local name=$(dirname $1)
    echo $name
}

function func_get_dir_parent(){
    local name=$(dirname $1)
    echo $name
}

function func_get_file_name(){
    local name=$(basename $1)
    echo $name
}

function func_get_array_itemnum(){
    if [[ $# -gt 1 ]]
    then
        log_e "parse_array_name input gt 1 param!"
        log_t "exit"
        exit 0
    fi
    local array_name=$1[@]
    local array_src=${!array_name}
    local i=0
    for it in ${array_src[@]}
    do
        i=$((i+1))
    done
    echo $i
}

function func_get_array_first(){
    if [[ -z $1 ]]
    then
        log_e "no param input func_get_array_first"
        log_t "exit"
        exit 1
    fi
    local array=$1
    for tt in ${array[*]}
    do
        echo $tt
        break
    done
}

