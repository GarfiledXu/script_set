// #include "MyFlags.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   MyFlags
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////
////                   gflags-img
//////////////////////////////////////////////
DEFINE_string(type, "", "default option: [ img video unit]");
DEFINE_string(path, "", "input path:[ dir or video or img ]");
//////////////////////////////////////////////
////                   gflags-sdk
//////////////////////////////////////////////
DEFINE_string(module, "", "default option: [ dms oms dmsfaceid omsfaceid ]");
DEFINE_string(sdk_function, "", "input sdk_function:[  ]");
DEFINE_int32(speed, 30, "input speed:[ dir or video or img ]");
//////////////////////////////////////////////
////                   gflags-license
//////////////////////////////////////////////
DEFINE_string(vehicle,"", "default option: [ EP40 ]" );
//////////////////////////////////////////////
////                   gflags-faceid
//////////////////////////////////////////////
DEFINE_string(faceid_opt, "", "input faceid_opt:[  ]");
DEFINE_int32(faceid_opt_param, 0, "input faceid_opt_param:[  ]");
//////////////////////////////////////////////
////                   gflags-camera FLAGS_dump_img
//////////////////////////////////////////////
DEFINE_string(camera_opt, "", "default option: [ alarm/faceid_register_login_match ]");

//////////////////////////////////////////////
////                   gflags-dump-img
//////////////////////////////////////////////
DEFINE_int32(dump_img, 1, "input :[  ]");


DEFINE_string(log_level, "xvidePWC", "");
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   Check Flags
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CheckStrFlags(const char* INPUT_KEY_C_STR, const char* INPUT_VALUE_C_STR, const std::vector<std::string> vecOption, bool isExit) {
    int32_t iRet = 1;
    for (std::string tempStr : vecOption) {
        iRet = iRet * strcmp(tempStr.c_str(), INPUT_VALUE_C_STR);
    }
    std::string strAllOption = "[ ";
    for (std::string strTemp : vecOption) {
        strAllOption += strTemp + " ";
    }
    strAllOption += "]";
    if(0 != iRet){
        LOG_ERROR("param error, %s is %s, should be %s\n", INPUT_KEY_C_STR, INPUT_VALUE_C_STR, strAllOption.c_str());
        if (true == isExit) {
            exit(1);
        }
    }
}