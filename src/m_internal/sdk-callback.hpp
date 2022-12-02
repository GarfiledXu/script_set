#ifndef SDK_CALLBACK_HPP
#define SDK_CALLBACK_HPP
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


template<typename T>
void ParseEvent(void* msg) {
    LOG_ERROR("error no match function!\n");
    return;
}

template<>
void ParseEvent<ASD_DMS_RESULT>(void* msg) {
    ASD_DMS_RESULT* pMsgRet = reinterpret_cast<ASD_DMS_RESULT*>(msg);
    EnumCvt enumCvt;
    json jsRoot;
    json jsImginfo;
    json jsItem;
    int32_t infoCount = 0;
    
    jsRoot["event_type"] = "ASD_EVENT_DMS";
    if ((pMsgRet->alarmType == ASD_ALARM_TYPE_CALL) || (pMsgRet->alarmType ==ASD_ALARM_TYPE_SMOKE)) {
            jsItem["danger_action"] = enumCvt.Enum2String(pMsgRet->alarmDetail.dangerAction);
            infoCount++;
    }
    if ((pMsgRet->alarmType == ASD_ALARM_TYPE_EXPRESSION)) {
        if ((pMsgRet->alarmDetail.expressionType > ASD_EXPRESSION_TYPE_INVALID) &&
            (pMsgRet->alarmDetail.expressionType <= ASD_EXPRESSION_TYPE_NORMAL)) {
            jsItem["expression"] = enumCvt.Enum2String(pMsgRet->alarmDetail.expressionType);
            infoCount++;
        }
    }
    //distractLevel
    if ((pMsgRet->alarmType == ASD_ALARM_TYPE_DISTRACT)) {
        if ((pMsgRet->distractLevel > ASD_DISTRACT_LEVEL_INVALID) &&
            (pMsgRet->distractLevel <= ASD_DISTRACT_LEVEL_HIGH)) {
            jsItem["distract_level"] = enumCvt.Enum2String(pMsgRet->distractLevel);
            infoCount++;
        }
    }
    
    //drowsyLevel
     if ((pMsgRet->alarmType == ASD_ALARM_TYPE_DROWSY)) {
         if ((pMsgRet->drowsyLevel > ASD_DROWSY_LEVEL_INVALID) &&
            (pMsgRet->drowsyLevel <= ASD_DROWSY_LEVEL_HIGH)) {
            jsItem["drowsy_level"] = enumCvt.Enum2String(pMsgRet->drowsyLevel);
            infoCount++;
        }
    }
     //alarmGazeArea
     if ((pMsgRet->alarmGazeArea > ASD_GAZE_AREA_INVALID) &&
         (pMsgRet->alarmGazeArea <= ASD_GAZE_AREA_OTHER)) {
         jsItem["gaze_area"] = enumCvt.Enum2String(pMsgRet->alarmGazeArea);
        infoCount++;
    }
     //actionType
     if ((pMsgRet->actionType > ASD_ACTION_TYPE_NONE) &&
        (pMsgRet->actionType <= ASD_ACTION_TYPE_SHAKE)) {
        jsItem["action"] = enumCvt.Enum2String(pMsgRet->actionType);
        infoCount++;
    }
     //gestureType
     if ((pMsgRet->gestureType > ASD_GESTURE_TYPE_INVALID) &&
        (pMsgRet->gestureType <= ASD_GESTURE_TYPE_SWIPE_BACKWARD)) {
        jsItem["gesture"] = enumCvt.Enum2String(pMsgRet->gestureType);
        infoCount++;
    }
    //headPoseData not invoke
    #if 0
    json HeadPoseData=json::array();
    HeadPoseData.push_back(pMsgRet->headPoseData.confidence);
    HeadPoseData.push_back(pMsgRet->headPoseData.roll);
    HeadPoseData.push_back(pMsgRet->headPoseData.yaw);
    HeadPoseData.push_back(pMsgRet->headPoseData.pitch);
    HeadPoseData.push_back(pMsgRet->headPoseData.headPoseX);
    HeadPoseData.push_back(pMsgRet->headPoseData.headPoseY);
    HeadPoseData.push_back(pMsgRet->headPoseData.headPoseZ);
    jsItem["head_pose_date"]=HeadPoseData;
    #endif
    jsItem["isheadPoseDataValid"]=pMsgRet->isheadPoseDataValid;
 /*    if (0 == infoCount) {
        return;
    } */
    jsRoot["dms_alarm_result"] = jsItem;
    CB_MergeImgInfo(L_SyncInfo, jsImginfo);
    jsRoot["img_info"] = jsImginfo;
    CB_MergeRet(pathSave.c_str(), jsRoot);
    return;
    return;
}

template<>
void ParseEvent<ASD_DMS_TIMER_RESULT>(void* msg) {
    
    return;
}

template<>
void ParseEvent<ASD_OMS_RESULT>(void* msg) {
    
    return;
}

template<>
void ParseEvent<ASD_DMS_AGE_GENDER>(void* msg) {
    
    return;
}
void CB_JSON(ASD_EVENT_TYPE eventType, void* msg, int32_t msgLen) {
    LOG_INFO("into CB_JSON\n");
    if (eventType == ASD_EVENT_DMS) 
        ParseEvent<ASD_DMS_RESULT>(msg);  
    else if (eventType == ASD_EVENT_OMS) 
        ParseEvent<ASD_OMS_RESULT>(msg);
    else if(eventType ==ASD_EVENT_DMS_TIMER)
        ParseEvent<ASD_DMS_TIMER_RESULT>(msg);
    else if (eventType == ASD_EVENT_DMS_AGE_GENDER) 
        ParseEvent<ASD_DMS_AGE_GENDER>(msg);
    else {
        // LOG_ERROR("input eventType error! value:%d\n", eventType);
        // exit(1);
    }
    LOG_INFO("out CB_JSON\n");
}



#endif