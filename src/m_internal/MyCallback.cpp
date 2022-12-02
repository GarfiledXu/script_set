#include "MyCallback.hpp"

#define THREAD_LOCK(mutex, g_syninfo) \
pthread_mutex_lock(&mutex); \
G_SyncInfo L_SyncInfo = g_syninfo;  \
pthread_mutex_unlock(&mutex); 
#include <thread>
//拷贝一次
class UpLocalSync {
private:
    class LockG{
        private:
            pthread_mutex_t* m_pmutex;
        public:
            LockG( pthread_mutex_t* p_mutex) : m_pmutex(p_mutex){
                pthread_mutex_lock(m_pmutex);
            }
            ~LockG() {
                pthread_mutex_unlock(m_pmutex);
            }
     };
public:
    UpLocalSync() {};
    ~UpLocalSync() {};
    template <typename T_G_SYNC>
    T_G_SYNC operator()(const T_G_SYNC& g_sync, pthread_mutex_t& mutex) const{
        LockG lockG(&mutex);
        return g_sync;
      
    }
};

//拷贝了两次
template <typename T_G_SYNC>
T_G_SYNC UpLocalSync2(const T_G_SYNC& g_sync, const pthread_mutex_t& mutex) {
    pthread_mutex_lock(&mutex);
    T_G_SYNC L_SyncInfo = g_sync;
    pthread_mutex_unlock(&mutex);
    return L_SyncInfo;
}


//拷贝一次
//引用和指针在使用途径上区别显现：引用在初始化时就绑定好对象，后续不能再变更
//当临时对象作为函数引用类型的实参传递时，其生命周期，析构时机是什么时候？？
//func(class_1()) class_1临时对象何时被创建，何时被销毁
//临时对象一直存在到创建它们的完整表达式的末尾。
//临时对象和值拷贝
//运行临时对象作为实参传递给传值或者const引用类型的形参，但不能引用类型


void CB_MergeImgInfo(const G_SyncInfo& L_SyncInfo, json& jsImginfo) {
    if (L_SyncInfo.enumRunModule == RUN_MODULE_MP4) {
            jsImginfo["video_path"] = L_SyncInfo.g_SyncMp4.videoPath;
            jsImginfo["total_frame"] = L_SyncInfo.g_SyncMp4.nbFrame;
            jsImginfo["idx_frame"] = L_SyncInfo.g_SyncMp4.idxFrame;
    }
    else if (L_SyncInfo.enumRunModule == RUN_MODULE_UNIT) {
    }
    else if (L_SyncInfo.enumRunModule == RUN_MODULE_IMG) {
        jsImginfo["img_path"] = L_SyncInfo.g_SyncImg.imgPath;
    }
    else if (L_SyncInfo.enumRunModule == RUN_MODULE_DEFAULT) {
        jsImginfo["is_default"] = 1;
    }
}
void CB_MergeRet(const std::string& pathSave, json& jsRet) {
    std::string strResult = jsRet.dump(4);
    LOG_EXTR("-->ret:\n%s\n", strResult.c_str());
    system("mkdir RESULT");
    std::string pathSave1 = pathSave.c_str();
    std::ofstream stmSaveRet(pathSave1.c_str(), std::ios::app);
    stmSaveRet << jsRet.dump() << std::endl;
    stmSaveRet.close();
    //   std::thread threa(CB_MergeImgInfo, strResult, strResult);
}
void CB_JSON(ASD_EVENT_TYPE eventType, void* msg, int32_t msgLen) {
    // LOG_EXTR("dddd\n");
    // sleep(100);
    LOG_INFO("into CB_JSON\n");
    EnumCvt enumCvt;
    if (eventType == ASD_EVENT_DMS) {
        ////LOG_DEBUG("into ARC_EVENT_DMS_ALARM\n");
        // pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);
        // G_SyncInfo L_SyncInfo = g_Sync_Putdata2Callback_DMS;
        // pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
//g_p_sync_dms_default2
        // CB_MsgPro_ASD_DMS_RESULT(UpLocalSync()(g_Sync_Putdata2Callback_DMS, mtx_dmsput_dms_callbackinfo),
        //     msg, "./RESULT/dms_alarm_result.txt");
        CB_MsgPro_ASD_DMS_RESULT(g_p_sync_dms_default2->get(),
            msg, "./RESULT/dms_alarm_result.txt");

        return;
    }
    else if (eventType == ASD_EVENT_OMS) {
        ////LOG_DEBUG("into ARC_EVENT_OCCUPANT_EXPRESSION\n");
        // pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);
        // G_SyncInfo L_SyncInfo = g_Sync_Putdata2Callback_OOMS;
        // pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);

        CB_MsgPro_ASD_OMS_RESULT(g_p_sync_oms_default2->get(),
            msg, "./RESULT/oms_alarm_result.txt");
        ////LOG_DEBUG("out CB_JSON\n");
        return;
    }
    else if(eventType ==ASD_EVENT_DMS_TIMER)
    {
        pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);
        G_SyncInfo L_SyncInfo = g_p_sync_dms_default2->get();
        pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);

        CB_MsgPro_ASD_DMS_TIMER_RESULT(L_SyncInfo,msg,"./RESULT/dms_timer_restult.txt");
    }
    else if (eventType == ASD_EVENT_DMS_AGE_GENDER) {
        // pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);
        // G_SyncInfo L_SyncInfo = g_Sync_Putdata2Callback_DMS;
        // pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);

        CB_MsgPro_ASD_DMS_AGE_GENDER(g_p_sync_dms_default2->get(),
            msg, "./RESULT/dms_age_gender_restult.txt");
    }
    else {
        // LOG_ERROR("input eventType error! value:%d\n", eventType);
        // exit(1);
    }
    LOG_INFO("out CB_JSON\n");
}

void CB_MsgPro_ASD_DMS_RESULT(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave){
    ASD_DMS_RESULT* pMsgRet = reinterpret_cast<ASD_DMS_RESULT*>(msg);
    EnumCvt enumCvt;
    json jsRoot;
    json jsImginfo;
    json jsItem;
    int32_t infoCount = 0;
     
    jsRoot["event_type"] = "ASD_EVENT_DMS";
    //alarmDetail(dangerAction/expressionType)
    //LOG_DEBUG("\n");
    if ((pMsgRet->alarmType == ASD_ALARM_TYPE_CALL) || (pMsgRet->alarmType ==ASD_ALARM_TYPE_SMOKE)) {
         //LOG_EXTR("\n");
            jsItem["danger_action"] = enumCvt.Enum2String(pMsgRet->alarmDetail.dangerAction);
            infoCount++;
    }
     //LOG_DEBUG("\n");
    if ((pMsgRet->alarmType == ASD_ALARM_TYPE_EXPRESSION)) {
         //LOG_EXTR("\n");
        if ((pMsgRet->alarmDetail.expressionType > ASD_EXPRESSION_TYPE_INVALID) &&
            (pMsgRet->alarmDetail.expressionType <= ASD_EXPRESSION_TYPE_NORMAL)) {
            jsItem["expression"] = enumCvt.Enum2String(pMsgRet->alarmDetail.expressionType);
            infoCount++;
        }
    }
    //distractLevel
     //LOG_DEBUG("\n");
    if ((pMsgRet->alarmType == ASD_ALARM_TYPE_DISTRACT)) {
         //LOG_EXTR("\n");
        if ((pMsgRet->distractLevel > ASD_DISTRACT_LEVEL_INVALID) &&
            (pMsgRet->distractLevel <= ASD_DISTRACT_LEVEL_HIGH)) {
            jsItem["distract_level"] = enumCvt.Enum2String(pMsgRet->distractLevel);
            infoCount++;
        }
    }
    //drowsyLevel
     //LOG_DEBUG("\n");
     if ((pMsgRet->alarmType == ASD_ALARM_TYPE_DROWSY)) {
          //LOG_EXTR("\n");
         if ((pMsgRet->drowsyLevel > ASD_DROWSY_LEVEL_INVALID) &&
            (pMsgRet->drowsyLevel <= ASD_DROWSY_LEVEL_HIGH)) {
            jsItem["drowsy_level"] = enumCvt.Enum2String(pMsgRet->drowsyLevel);
            infoCount++;
        }
    }
     //alarmGazeArea
      //LOG_DEBUG("\n");
     if ((pMsgRet->alarmGazeArea > ASD_GAZE_AREA_INVALID) &&
         (pMsgRet->alarmGazeArea <= ASD_GAZE_AREA_OTHER)) {
          //LOG_EXTR("\n");
         jsItem["gaze_area"] = enumCvt.Enum2String(pMsgRet->alarmGazeArea);
        infoCount++;
    }
     //actionType
      //LOG_DEBUG("\n");
     if ((pMsgRet->actionType > ASD_ACTION_TYPE_NONE) &&
        (pMsgRet->actionType <= ASD_ACTION_TYPE_SHAKE)) {
             //LOG_EXTR("\n");
        jsItem["action"] = enumCvt.Enum2String(pMsgRet->actionType);
        infoCount++;
    }
     //gestureType
      //LOG_DEBUG("\n");
     if ((pMsgRet->gestureType > ASD_GESTURE_TYPE_INVALID) &&
        (pMsgRet->gestureType <= ASD_GESTURE_TYPE_SWIPE_BACKWARD)) {
         //LOG_EXTR("\n");
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
}
void CB_MsgPro_ASD_DMS_TIMER_RESULT(const G_SyncInfo& L_SyncInfo,void *msg, const std::string& pathSave){
    ASD_DMS_TIMER_RESULT* pMsgRet = reinterpret_cast<ASD_DMS_TIMER_RESULT*>(msg);
    EnumCvt enumCvt;
    json jsRoot;
    json jsImginfo;
    json jsItem;
    int32_t infoCount = 0;
    jsItem["distract_level"] = enumCvt.Enum2String(pMsgRet->distractLevel);
    jsItem["drowsy_level"] = enumCvt.Enum2String(pMsgRet->drowsyLevel);
    jsRoot["dms_timer_result"] = jsItem;
    CB_MergeImgInfo(L_SyncInfo, jsImginfo);
    jsRoot["img_info"] = jsImginfo;
    CB_MergeRet(pathSave.c_str(), jsRoot);
    return;
}
void CB_MsgPro_ASD_OMS_RESULT(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave) {
    ASD_OMS_RESULT* pMsgRet = reinterpret_cast<ASD_OMS_RESULT*>(msg);
    EnumCvt enumCvt;
    json jsRoot;
    json jsImginfo;
    json jsItem;
    int32_t infoCount = 0;

    jsRoot["event_type"] = "ASD_EVENT_OMS";
    jsItem["iFaceCount"]=pMsgRet->iFaceCount;

    //gestureType
    if ((pMsgRet->gestureType > ASD_GESTURE_TYPE_INVALID)&&
        (pMsgRet->gestureType <= ASD_GESTURE_TYPE_SWIPE_BACKWARD)) {
        infoCount++;
        jsItem["gesture"] = enumCvt.Enum2String(pMsgRet->gestureType);
    }
    //babyResult
    json jsBabyArray = json::array();
    for (int i = 0;i <(pMsgRet->babyResult.badyNum);i++) {
        infoCount++;
        json e;
        // e["type"] = enumCvt.Enum2String(pMsgRet->babyResult.babyType[i]);
        e["seatid"] = pMsgRet->babyResult.seatId[i];
        jsBabyArray.push_back(e);
    }
    jsItem["baby_array"] = jsBabyArray;
    
    json jsResidueArray=json::array();
    for(int i=0;i<(pMsgRet->residueResult.residueNum);++i)
    {
        infoCount++;
        json e;
        e["type"] = enumCvt.Enum2String(pMsgRet->residueResult.residueType[i]);
        e["seatid"] = pMsgRet->residueResult.seatId[i];
        jsResidueArray.push_back(e);
    }
    jsItem["residue_array"]=jsResidueArray;

    json jsPetArray=json::array();
    for(int i=0;i<(pMsgRet->petResult.petNum);++i)
    {
        infoCount++;
        json e;
        e["type"] = enumCvt.Enum2String(pMsgRet->petResult.petType[i]);
        e["seatid"] = pMsgRet->petResult.seatId[i];
        jsPetArray.push_back(e);
    }
    jsItem["pet_array"]=jsPetArray;
    //hasResidue
    jsItem["has_resideu"] = pMsgRet->hasResidue;
    //hasPet
    jsItem["has_pet"] = pMsgRet->hasPet;
    //occupantInfos[ASD_MAX_FACE_NUM];
    json jsOccupantArray = json::array();
    for (int i = 0;i <(pMsgRet->iFaceCount);i++) {
        infoCount++;
        json jsSub;
        #define SUB_OCCUPANT pMsgRet->occupantInfos[i]
        if (SUB_OCCUPANT.alarmType == ASD_ALARM_TYPE_CALL || SUB_OCCUPANT.alarmType == ASD_ALARM_TYPE_SMOKE) {
            if ((SUB_OCCUPANT.alarmDetail.dangerAction >ASD_DANGER_ACTION_INVALID)&&
                (SUB_OCCUPANT.alarmDetail.dangerAction < ASD_DANGER_ACTION_NONE)) {
                jsSub["danger_action"] = enumCvt.Enum2String(SUB_OCCUPANT.alarmDetail.dangerAction);
                infoCount++;
            }
            if ((SUB_OCCUPANT.alarmDetail.expressionType > ASD_EXPRESSION_TYPE_INVALID)&&
                (SUB_OCCUPANT.alarmDetail.expressionType <=ASD_EXPRESSION_TYPE_NORMAL)) {
                jsSub["expression"] = enumCvt.Enum2String(SUB_OCCUPANT.alarmDetail.expressionType);
                infoCount++;
            }
            if ((SUB_OCCUPANT.seatid > ASD_SEAT_ID_UNKNOWN)&&
                (SUB_OCCUPANT.seatid <= ASD_SEAT_ID_DRIVER)) {
                jsSub["setid"] = enumCvt.Enum2String(SUB_OCCUPANT.seatid);
            }
            jsSub["age"] = SUB_OCCUPANT.age;
            jsSub["gender"] = SUB_OCCUPANT.gender;
        }
        jsOccupantArray.push_back(jsSub);
    }
    jsItem["occupant_array"] = jsOccupantArray;

    
/*     if (0 == infoCount) {
        return;
    } */
    jsRoot["oms_result"] = jsItem;
    CB_MergeImgInfo(L_SyncInfo, jsImginfo);
    jsRoot["img_info"] = jsImginfo;
    CB_MergeRet(pathSave.c_str(), jsRoot);
    return;
}
void CB_MsgPro_ASD_DMS_AGE_GENDER(const G_SyncInfo& L_SyncInfo, void* msg, const std::string& pathSave) {
    ASD_DMS_AGE_GENDER* pMsgRet = reinterpret_cast<ASD_DMS_AGE_GENDER*>(msg);
    EnumCvt enumCvt;
    json jsRoot;
    json jsImginfo;
    json jsItem;
    int32_t infoCount = 0;
    jsRoot["event_type"] = "ASD_EVENT_DMS";
    jsItem["error_code"] = pMsgRet->errorCode;
    jsItem["dirver_age"] = pMsgRet->dirverAge;
    jsItem["dirver_gender"] = pMsgRet->dirverGender;
    infoCount++;

    
 /*    if (0 == infoCount) {
        return;
    } */
    jsRoot["dms_age_gender"] = jsItem;
    CB_MergeImgInfo(L_SyncInfo, jsImginfo);
    jsRoot["img_info"] = jsImginfo;
    CB_MergeRet(pathSave.c_str(), jsRoot);
    return;
}
























