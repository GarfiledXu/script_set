#ifndef _MY_PROCESS_HPP_
#define _MY_PROCESS_HPP_
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
// #include "MyFlags.hpp"
#include "MySdk.hpp"
#include "MyAdapt.hpp"
#include "MyCallback.hpp"
#include "MyGbuffer.hpp"

// #include "MyCamera.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_decode_img
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.22
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SaveUYVY(unsigned char* buf, int width, int height,
      char* filename);
void DecodeMp4PushFrame(SDK_MODULE enumSdkModule, const std::string& strVideoPath, G_SyncInfo& L_SyncInfo, void* pSdkPro);
void DecodeJpgPushFrame(SDK_MODULE enumSdkModule, const std::string& strJpgPath, void* pSdkPro);
long DecodeMp4ForFrameNb(const std::string& pathVideo);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_thread_function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.22
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* G_DmsImg_DataThread(void* pSdkPro);
void* G_OmsImg_DataThread(void* pSdkPro) ;
void* G_DmsMp4_DataThread(void* pSdkPro);
void* G_OmsMp4_DataThread(void* pSdkPro);
void* G_DmsUnit_DataThread(void* pSdkPro);
void* G_OmsUnit_DataThread(void* pSdkPro);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_thread_camera_pushFrame
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.28
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* G_DmsCamera_DataThread(void* pmySdk);
void* G_OmsCamera_DataThread(void* pmySdk);
void* G_DmsDumpImg(void* agv);
void* G_OmsDumpImg(void* agv);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_thread_manage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.22
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void G_CheckDataThreadCounter(DataThreadCounter& threadCounter);
void G_RiseDataPthread(RUN_MODULE enumRunModule, SDK_MODULE enumSdkModule, DataThreadCounter& threadCounter, MySdk& mySdk);
void G_ReleaseDataPthread(DataThreadCounter& threadCounter, MySdk& algPro);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_test_faceid
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date: 2022.02.25.22:30 -  23:00 2022.02.26.17:40-18:17
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   FLAGS:FLAGS_faceid_opt FLAGS_path FLAGS_module FALGS_faceid_opt_param
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class TestFaceid {
private:
      /*data*/
public:
      TestFaceid(/*args*/);
      ~TestFaceid();
       void Run(const std::string& strModule, const std::string& strFaceidOpt, const std::string& strPath);
// /*ONLY DMS*/      
//       void Register(SDK_MODULE enumSdkModule, const std::string& pathImg, int32_t faceID, MySdk& mySdk);
// /*ONLY DMS*/     
//       void Login(SDK_MODULE enumSdkModule, const std::string& pathImg, MySdk& mySdk);
// /*ONLY OOMS*/     
//       void AgeGender(SDK_MODULE enumSdkModule, const std::string& pathImg, MySdk& mySdk);
// /*ONLY OOMS*/     
//       void LivingThs(SDK_MODULE enumSdkModule, const std::string& pathImg, MySdk& mySdk);
// /*ONLY DMS*/     
//       void DeleteAllFace(SDK_MODULE enumSdkModule, MySdk& mySdk);
// /*ONLY DMS*/     
//       void DeleteFaceID(SDK_MODULE enumSdkModule, int32_t iDeleteFaceID, MySdk& mySdk);
// /*ONLY DMS*/     
//       void GetFaceNum(SDK_MODULE enumSdkModule, MySdk& mySdk);
// /*ONLY DMS*/     
//       void GetRegFaces(SDK_MODULE enumSdkModule, MySdk& mySdk);
      void Register_Recog_Match(SDK_MODULE enumInput, const std::string& pathRegister, const std::string& pathRecognition, MySdk& mySdk);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_run_module
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.23.17:00 -  17:30  2022.02.25.22:00 -  22:30
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   FLAGS:FLAGS_module FLAGS_type FLAGS_path
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class RunModule {
private:
      /*data*/
public:
      RunModule(/* args */);
      ~RunModule();
      void Run(const std::string& strModule, const std::string& strType, const std::string& strPath);
      void RunImg(SDK_MODULE enumSdkModule, const std::string& strPath, MySdk& mySdk);
      void RunVideo(SDK_MODULE enumSdkModule, const std::string& strPath, MySdk& mySdk);
      void RunUnit();
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_run_camera
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.28 13:43-
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   FLAGS:FLAGS_module(oms/dms/dms_oms) FLAGS_type(img/video/camera) 
////                                FLAGS_camera_opt(faceid_register_login_match / alarm) FLAGS_speed<int>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class TestCamera {
// private:
//       /*data*/
//       pthread_t dmsCaptureId = 0, omsCaptureId = 0, cameraCheckId = 0;
// public:
//       TestCamera();
//       ~TestCamera();
//       void Run(const std::string& strModule, const std::string& strCameraOpt);
//       int QcameraInit(MYSDK_SET_MASK inputSdkMask);
//       int QcameraUnInit(MYSDK_SET_MASK inputSdkMask);
//       int GetCameraArcImg(MYSDK_SET_MASK inputSdkMask, ASVLOFFSCREEN& arcImg);
//       int DmsOmsAlarm(MYSDK_SET_MASK inputSdkMask, MySdk& mySdk);
//       int FaceidRegisterLoginMatch(MYSDK_SET_MASK inputSdkMask, MySdk& mySdk);
//       int DumpImg(void);

// };


#endif