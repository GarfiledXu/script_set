#ifndef _MY_SDK_HPP_
#define _MY_SDK_HPP_

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

#include "MyCallback.hpp"
#include "MyGbuffer.hpp"
#include "MyAdapt.hpp"

#include "ArcDriveSDK.h"
// #include "MyFlags.hpp"
using json = nlohmann::json;
#define SAFE_FREE(p) if(NULL == p){free(p);p=NULL};
#define FUNC(FUNC,TRUE)      ({ int RET=0;\
(RET=FUNC)==TRUE? RET:\
printf("in %d,%s:%s fail! return [%d]\n", __LINE__, __FUNCTION__,\
[&](std::string strFunc)->std::string{ std::string funcName(strFunc);return funcName.substr(0,funcName.find("(")); }(#FUNC).c_str(),\
 RET);RET;\
})

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   SDK_MASK define
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.25 18:15 -18:30
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MYSDK_SET_MASK_NONE 1<<0
#define MYSDK_SET_MASK_SIGNAL 1<<1
#define MYSDK_SET_MASK_DMS_DETECT_SWITCH 1<<2
#define MYSDK_SET_MASK_OMS_DETECT_SWITCH 1<<3
typedef uint32_t MYSDK_SET_MASK;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   MySdk 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.25 18:15 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MySdk {
private:

public:
//////////////////////////////////////////////////////
////                   handle 
//////////////////////////////////////////////////////
    ASD_ALG_HANDLE pAlgHandle = NULL;
//////////////////////////////////////////////////////
////                   thread 
//////////////////////////////////////////////////////
    pthread_t thrDmsDataPut;
    pthread_t thrOmsDataPut;
    pthread_t thrDms;
    pthread_t thrOms;
//////////////////////////////////////////////////////
////                   img 
//////////////////////////////////////////////////////
    tv::Mat* pDmsMatImg = nullptr;
    tv::FourccBuffer* pDmsFbImg = nullptr;
    ASVLOFFSCREEN* pDmsArcImg = nullptr;
    tv::Mat* pOmsMatImg = nullptr;
    tv::FourccBuffer* pOmsFbImg = nullptr;
    ASVLOFFSCREEN* pOmsArcImg = nullptr;
//////////////////////////////////////////////////////
////                   param 
//////////////////////////////////////////////////////
    ASD_INIT_PARAM initParam;
//////////////////////////////////////////////////////
////                   tectonics_ deconstruction
//////////////////////////////////////////////////////
    MySdk();
    ~MySdk();
//////////////////////////////////////////////////////
////                    init_uninit_version_license
//////////////////////////////////////////////////////
    int32_t InitAlgHandle(ASD_INIT_PARAM* initParam, ArcEvent pFunc);
    void UninitAlgHandle();
    const char* GetSDKVersion();
    const char* GetLicenseReqInfo(const char* VIN, const char* vehicleID);
    int32_t CheckLicense(const char* VIN, const char* license);
//////////////////////////////////////////////////////
////                    sdk_thread
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
////                    push_frame
//////////////////////////////////////////////////////
    int32_t PushDmsFrame(ASVLOFFSCREEN* pImage, int32_t speed = 0);
    int32_t PushOmsFrame(ASVLOFFSCREEN* pImage);
//////////////////////////////////////////////////////
////                    setting
//////////////////////////////////////////////////////
    //int32_t SetVehicleSignals(ASD_VEHICLE_SIGNAL_TYPE type, int32_t value);
    int32_t SetDmsDetectSwitch(ASD_DETECT_MASK mask);
    int32_t SetOmsDetectSwitch(ASD_DETECT_MASK mask);
//////////////////////////////////////////////////////
////                    faceid
//////////////////////////////////////////////////////
    int32_t FaceRegister(ASVLOFFSCREEN* pImage, ASD_CAM_MASK camType, char* pFeatureReg);
    int32_t FaceRecognition(ASVLOFFSCREEN* pImage, ASD_CAM_MASK camType, char* pFeatureRegcog);
    int32_t FaceMatch(const char* pFeatureReg, const char* pFeatureRegcog, double* similarity);
//////////////////////////////////////////////////////
////                   process 
//////////////////////////////////////////////////////
    std::string ReadTxt(const char* txtPath);
    std::vector<int32_t> Init(ASD_INIT_PARAM* pParam, ArcEvent eventFunc);
    void Uninit();
    //void AllSet(MYSDK_SET_MASK myMask, ASD_VEHICLE_SIGNAL_TYPE type, int32_t value, ASD_DETECT_MASK mask);
    void PushAdapt(SDK_MODULE enumInput, ASVLOFFSCREEN* pImage);
    void AllSet(void);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   inline_define 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.24
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
inline int32_t MySdk::InitAlgHandle(ASD_INIT_PARAM* initParam, ArcEvent pFunc) {
    return ASD_InitAlgHandle(&this->pAlgHandle, initParam, pFunc);
}
inline void MySdk::UninitAlgHandle(){
    return ASD_UninitAlgHandle(this->pAlgHandle);
}
inline const char* MySdk::GetSDKVersion() {
    return ASD_GetSDKVersion();
}
inline const char* MySdk::GetLicenseReqInfo(const char* VIN, const char* vehicleID) {
    return ASD_GetLicenseReqInfo(VIN, vehicleID);
}
inline int32_t MySdk::CheckLicense(const char* VIN, const char* license){
    return ASD_CheckLicense(VIN, license);
}
inline int32_t MySdk::PushDmsFrame(ASVLOFFSCREEN* pImage, int32_t speed){
    return ASD_PushDmsFrame(this->pAlgHandle, pImage, speed);
}
inline int32_t MySdk::PushOmsFrame(ASVLOFFSCREEN* pImage){
    return ASD_PushOmsFrame(this->pAlgHandle, pImage);
}
inline int32_t MySdk::SetDmsDetectSwitch(ASD_DETECT_MASK mask){
    return ASD_SetDmsDetectSwitch(this->pAlgHandle, mask);
}
inline int32_t MySdk::SetOmsDetectSwitch(ASD_DETECT_MASK mask){
    return ASD_SetOmsDetectSwitch(this->pAlgHandle, mask);
}
inline int32_t MySdk::FaceRegister(ASVLOFFSCREEN* pImage, ASD_CAM_MASK camType, char* pFeatureReg) {
    return ASD_FaceRegister(this->pAlgHandle, pImage, camType, pFeatureReg);
}
inline int32_t MySdk::FaceRecognition(ASVLOFFSCREEN* pImage, ASD_CAM_MASK camType, char* pFeatureRegcog) {
    return ASD_FaceRecognition(this->pAlgHandle, pImage, camType, pFeatureRegcog);
}
inline int32_t MySdk::FaceMatch(const char* pFeatureReg, const char* pFeatureRegcog, double* similarity) {
    return ASD_FaceMatch(this->pAlgHandle, pFeatureReg, pFeatureRegcog, similarity);
}


#endif