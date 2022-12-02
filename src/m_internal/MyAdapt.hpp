#ifndef _MY_ADAPT_HPP_
#define _MY_ADAPT_HPP_
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

#include "utils.h"
#include "jftools.hpp"
#include "json.hpp"
#include "MyEasyLog.hpp"

#include "ArcDriveSDK.h"
#include "EnumAdaptMacro.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   my enum
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.02.25 17:57
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum {
    RUN_MODULE_INVALID = -1,
    RUN_MODULE_MP4 = 0,
    RUN_MODULE_IMG = 1,
    RUN_MODULE_UNIT = 2,
    RUN_MODULE_DEFAULT = 3,
}RUN_MODULE;
typedef enum {
    SDK_MODULE_INVALID = -1,
    SDK_MODULE_DMS = 0,
    SDK_MODULE_OMS = 1,
    SDK_MODULE_DMS_FACEID = 2,
    SDK_MODULE_OMS_FACEID = 3,
}SDK_MODULE;
typedef enum {
    SDK_PUTDATA_ENABLE_INVALID = -1,
    SDK_PUTDATA_ENABLE_DMS = 0,
    SDK_PUTDATA_ENABLE_OMS = 1,
    SDK_PUTDATA_ENABLE_DMS_OMS = 2,
}SDK_PUTDATA_ENABLE;
typedef enum {
    INPUT_SOURCE_PROCESS_ENABLE_INVALID = -1,
    INPUT_SOURCE_PROCESS_ENABLE_ENABLE = 0,
    INPUT_SOURCE_PROCESS_ENABLE_DISABLE = 1,
}INPUT_SOURCE_PROCESS_ENABLE;
typedef enum {
    SYNC_TYPE_INVALID = -1,
    SYNC_TYPE_MAIN_TO_DATAPUT = 0,
    SYNC_TYPE_DATAPUT_TO_CALLBACK = 1,
}SYNC_TYPE;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   sync struct
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   date:2022.01.06 14:00  2022.02.22 15:15
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct G_SyncImg {//必须独立出来，用于增改扩展
    std::string imgPath;
}G_SyncImg, * pG_SyncImg;
typedef struct G_SyncUnit {
    long idxFrame = -1;
    long nbFrame = -1;
    std::string videoPath;
    std::string framePath;
}G_SyncUnit, * pG_SyncUnit;
typedef struct G_SyncMp4 {
    long idxFrame = -1;
    long nbFrame = -1;
    std::string videoPath;
}G_SyncMp4, * pG_SyncMp4;
typedef struct G_SyncInfo {
    RUN_MODULE enumRunModule = RUN_MODULE_INVALID;//switch, decide to start run
    SDK_MODULE enumSdkModule = SDK_MODULE_INVALID;
    SYNC_TYPE enumSyncType = SYNC_TYPE_INVALID;//same struct, but define two
    G_SyncImg g_SyncImg;
    G_SyncMp4 g_SyncMp4;
    G_SyncUnit g_SyncUnit;
}G_SyncInfo, * pG_SyncInfo;
typedef struct DataThreadCounter {
      int dmsImgCounter = 0;
      int dmsMp4Counter = 0;
      int dmsUnitCounter = 0;
      int omsImgCounter = 0;
      int omsMp4Counter = 0;
      int omsUnitCounter = 0;
      // int dmsRunTypeCounter = 0;
      // int omsRunTypeCounter = 0;
}DataThreadCounter, * pDataThreadCounter;




/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                  EnumCvt
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                  date:2022.01.06 14:00  2022.02.22 15:07-15:28 2022.02.25 17:50-18:00
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class EnumCvt
{
private:
    /* data */
public:
    EnumCvt(/* args */);
    ~EnumCvt();

//////////////////////////////////////////////////////
////                  sdk enum
//////////////////////////////////////////////////////
    DECLARE_CLASS_MEM_ENUM_CVT(SYNC_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(SDK_MODULE);
    DECLARE_CLASS_MEM_ENUM_CVT(RUN_MODULE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_EVENT_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_ALARM_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_DISTRACT_LEVEL);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_DROWSY_LEVEL);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_DANGER_ACTION);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_ACTION_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_EXPRESSION_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_GAZE_AREA);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_GESTURE_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_BABY_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_RESIDUE_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_PET_TYPE);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_SEAT_ID);
    DECLARE_CLASS_MEM_ENUM_CVT(ASD_PASSENGER_GENDER);
};



/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                  SetGSync
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                  date:2022.02.22 16:00
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SetGSync {
private:
    G_SyncInfo* pG_SyncInfo = NULL;
public:
    SetGSync(/* args */);
    SetGSync(G_SyncInfo& g_SyncInfo);
    ~SetGSync();
    void InitAll(G_SyncInfo& Main2Putdata_DMS, G_SyncInfo& Main2Putdata_OOMS, G_SyncInfo& Putdata2Callback_DMS, G_SyncInfo& Putdata2Callback_OOMS);
    void Reset(SDK_MODULE enumSdkModule, SYNC_TYPE enumSyncType);
    void Set(G_SyncMp4& l_SycnMp4);
    void Set(G_SyncImg& l_SycnImg);
    void Set(G_SyncUnit& l_SycnUnit);
};




#endif