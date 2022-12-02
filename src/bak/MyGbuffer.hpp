#ifndef _MY_G_BUFFER_HPP_
#define _MY_G_BUFFER_HPP_ 
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

#include "MyAdapt.hpp"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   GBuffer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.25 18:11
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern pthread_mutex_t mtx_dms_faceid_putdata;
extern pthread_mutex_t  mtx_main_dmsput_data;
extern pthread_mutex_t  mtx_main_oomsput_data ;

extern pthread_mutex_t mtx_dmsput_dms_callbackinfo ;
extern pthread_mutex_t mtx_oomsput_ooms_callbackinfo;

extern std::string g_defaultImg_dms;
extern std::string g_defaultImg_ooms;

extern G_SyncInfo g_Sync_Main2Putdata_DMS;
extern G_SyncInfo g_Sync_Main2Putdata_OOMS;
extern G_SyncInfo g_Sync_Putdata2Callback_DMS;
extern G_SyncInfo g_Sync_Putdata2Callback_OOMS;

extern DataThreadCounter G_threadCounter;

//2022-02-28
#define DMS_CAMERA_WIDTH 1600
#define DMS_CAMERA_HEIGHT 1300
#define OMS_CAMERA_WIDTH 1920
#define OMS_CAMERA_HEIGHT 1080
#define DMS_BUFF_LENGTH DMS_CAMERA_WIDTH*DMS_CAMERA_HEIGHT*2
#define OMS_BUFF_LENGTH OMS_CAMERA_WIDTH*OMS_CAMERA_HEIGHT*2
extern pthread_mutex_t  mtx_camera_pushdata_dms;
extern pthread_mutex_t  mtx_camera_pushdata_oms;
extern unsigned char G_CameraImgBuff_Dms[DMS_BUFF_LENGTH];
extern unsigned char G_CameraImgBuff_Oms[OMS_BUFF_LENGTH];
#endif  
