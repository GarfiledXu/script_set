#! /usr/bin/bash

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
color_clear=$color_green_bold
#$1 tag_color
#$1 tag
#$2 string_color
#$3 string
function func_log {
    local l_tag_c=$1
    local l_tag=$2
    local l_string_c=$3
    local l_string=$4

    if [[ $# -ne 4 ]]
    then
        func_log ${color_red} "eror" ${color_red_bold} "func_log need input 4 param: tag tag_color string string_color" 
        exit 1
    fi

    echo -e "$color_clear"["$l_tag_c$l_tag$color_clear"]"$l_string_c$l_string$color_clear"
}

function log_t {
    func_log $color_yellow "extr" $color_blue "$@"
}
function log_e {
    func_log $color_red "eror" $color_red_bold "$@"
}
function log_i {
    func_log $color_blue "info" $color_blue_bold "$@"
}
function log_d {
    func_log $color_white "dbug" $color_white_bold "$@"
}

# log_t "logt"
# log_e "loge"
# log_i "logi"
# log_d "logd"