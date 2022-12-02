#ifndef _MY_CALLBACK_HPP_ 
#define _MY_CALLBACK_HPP_ 
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h> 
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "gtest/gtest.h"
#include "gflags/gflags.h"
#include "toy/mat.hpp"
#include "toy/fourcc_cvt.hpp"
#include "toy/imread.hpp"
#include "toy/imwrite.hpp"
extern "C" {
    #include "libavcodec/avcodec.h"
    #include "libavformat/avformat.h"
    #include "libswscale/swscale.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/avassert.h"
    #include "libavutil/imgutils.h"
    #include "libavutil/intreadwrite.h"
}

#include "utils.h"
#include "jftools.hpp"
#include "json.hpp"
#include "MyEasyLog.hpp"

#include "MySdk.hpp"
#include "MyGbuffer.hpp"
#include "MyAdapt.hpp"

#include "ArcDriveSDK.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   MyCB
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.25 18:09-18:12
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CB_MergeRet(const std::string& pathSave, json& jsRet);
void CB_MergeImgInfo(const G_SyncInfo& L_SyncInfo, json& jsImginfo);
void CB_JSON(ASD_EVENT_TYPE eventType, void* msg, int32_t msgLen);
void CB_MsgPro_ASD_OMS_RESULT(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave);
void CB_MsgPro_ASD_DMS_RESULT(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave);
void CB_MsgPro_ASD_DMS_TIMER_RESULT(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave);
void CB_MsgPro_ASD_DMS_AGE_GENDER(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave);
#endif