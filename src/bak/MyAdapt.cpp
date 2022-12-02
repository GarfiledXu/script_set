#include "MyAdapt.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                  EnumCvt
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
EnumCvt::EnumCvt() {}
EnumCvt::~EnumCvt() {}

FUNC_ENUM_CVT(SYNC_TYPE, SYNC_TYPE_INVALID, SYNC_TYPE_MAIN_TO_DATAPUT, SYNC_TYPE_DATAPUT_TO_CALLBACK);
FUNC_ENUM_CVT(RUN_MODULE, RUN_MODULE_INVALID, RUN_MODULE_MP4, RUN_MODULE_IMG, RUN_MODULE_UNIT, RUN_MODULE_DEFAULT);
FUNC_ENUM_CVT(SDK_MODULE, SDK_MODULE_INVALID, SDK_MODULE_DMS, SDK_MODULE_OMS, SDK_MODULE_DMS_FACEID, SDK_MODULE_OMS_FACEID);
FUNC_ENUM_CVT(ASD_EVENT_TYPE, ASD_EVENT_DMS, ASD_EVENT_OMS);
FUNC_ENUM_CVT(ASD_ALARM_TYPE, ASD_ALARM_TYPE_NONE, ASD_ALARM_TYPE_DISTRACT,ASD_ALARM_TYPE_DROWSY,ASD_ALARM_TYPE_EXPRESSION,ASD_ALARM_TYPE_CALL,ASD_ALARM_TYPE_SMOKE );
FUNC_ENUM_CVT(ASD_DISTRACT_LEVEL, ASD_DISTRACT_LEVEL_INVALID, ASD_DISTRACT_LEVEL_NONE,
    ASD_DISTRACT_LEVEL_LOW, ASD_DISTRACT_LEVEL_MIDDLE, ASD_DISTRACT_LEVEL_HIGH);
FUNC_ENUM_CVT(ASD_DROWSY_LEVEL, ASD_DROWSY_LEVEL_INVALID, ASD_DROWSY_LEVEL_NONE,
    ASD_DROWSY_LEVEL_LOW, ASD_DROWSY_LEVEL_MIDDLE, ASD_DROWSY_LEVEL_HIGH);
FUNC_ENUM_CVT(ASD_DANGER_ACTION, ASD_DANGER_ACTION_INVALID, ASD_DANGER_ACTION_SMOKE,
    ASD_DANGER_ACTION_CALL, ASD_DANGER_ACTION_NONE);
FUNC_ENUM_CVT(ASD_ACTION_TYPE, ASD_ACTION_TYPE_NONE, ASD_ACTION_TYPE_NOD,
    ASD_ACTION_TYPE_SHAKE);
FUNC_ENUM_CVT(ASD_EXPRESSION_TYPE, ASD_EXPRESSION_TYPE_INVALID, ASD_EXPRESSION_TYPE_HAPPY,
    ASD_EXPRESSION_TYPE_ANGRY, ASD_EXPRESSION_TYPE_SURPRISE, ASD_EXPRESSION_TYPE_SAD, ASD_EXPRESSION_TYPE_NORMAL);
FUNC_ENUM_CVT(ASD_GAZE_AREA, ASD_GAZE_AREA_INVALID, ASD_GAZE_AREA_MAIN_DRIVING_WINDSCREEN,
    ASD_GAZE_AREA_CENTER_CONSOLE, ASD_GAZE_AREA_MAIN_DRIVING_SCREEN, ASD_GAZE_AREA_LEFT_REARVIEW, ASD_GAZE_AREA_RIGHT_REARVIEW,
    ASD_GAZE_AREA_REARVIEW,ASD_GAZE_AREA_OTHER);
FUNC_ENUM_CVT(ASD_GESTURE_TYPE, ASD_GESTURE_TYPE_INVALID, ASD_GESTURE_TYPE_MUTE,
    ASD_GESTURE_TYPE_VSIGN, ASD_GESTURE_TYPE_OK, ASD_GESTURE_TYPE_THUMBUP,
    ASD_GESTURE_TYPE_CIRCLE_CW, ASD_GESTURE_TYPE_CIRCLE_CCW, ASD_GESTURE_TYPE_SWIPE_LEFT,
    ASD_GESTURE_TYPE_SWIPE_RIGHT, ASD_GESTURE_TYPE_SWIPE_FORWARD, ASD_GESTURE_TYPE_SWIPE_BACKWARD);
FUNC_ENUM_CVT(ASD_BABY_TYPE, ASD_BABY_TYPE_INVALID, ASD_BABY_TYPE_IN_SAFETYSEAT,
    ASD_BABY_TYPE_OUT_SAFETYSEAT, ASD_BABY_TYPE_OTHER);
FUNC_ENUM_CVT(ASD_RESIDUE_TYPE,ASD_RESIDUE_TYPE_INVALID,ASD_RESIDUE_TYPE_PHONE,ASD_RESIDUE_TYPE_PACK,ASD_RESIDUE_TYPE_OTHER);
FUNC_ENUM_CVT(ASD_PET_TYPE,ASD_PET_TYPE_INVALID,ASD_PET_TYPE_CAT,ASD_PET_TYPE_DOG,ASD_PET_TYPE_OTHER);
FUNC_ENUM_CVT(ASD_SEAT_ID, ASD_SEAT_ID_UNKNOWN, ASD_SEAT_ID_CO_DRIVER,
    ASD_SEAT_ID_REAR_LEFT, ASD_SEAT_ID_REAR_MID, ASD_SEAT_ID_REAR_RIGHT, ASD_SEAT_ID_DRIVER);
FUNC_ENUM_CVT(ASD_PASSENGER_GENDER, ASD_PASSENGER_GENDER_UNKNOWN, ASD_PASSENGER_GENDER_MALE,
    ASD_PASSENGER_GENDER_FEMALE);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   SetGSync
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
SetGSync::SetGSync() {}
SetGSync::~SetGSync() {
    pG_SyncInfo = NULL;
    //LOG_DEBUG("uinit SetGSync, pG_SyncInfo be NULL!\n");
}
SetGSync::SetGSync(G_SyncInfo& g_SyncInfo) {
    pG_SyncInfo = &g_SyncInfo;
    EnumCvt enumCvt;
    //LOG_DEBUG("init SetGSync, SDK_MODULE:%s, SYNC_TYPE:%s\n", enumCvt.Enum2String(pG_SyncInfo->enumSdkModule).c_str(), enumCvt.Enum2String(pG_SyncInfo->enumSyncType).c_str());
}
void SetGSync::Reset(SDK_MODULE enumSdkModule, SYNC_TYPE enumSyncType) {
    EnumCvt enumCvt;
    pG_SyncInfo->enumRunModule = RUN_MODULE_DEFAULT;
    pG_SyncInfo->enumSdkModule = enumSdkModule;
    pG_SyncInfo->enumSyncType = enumSyncType;
    //LOG_DEBUG("reset GSync, SDK_MODULE:%s, SYNC_TYPE:%s\n", enumCvt.Enum2String(pG_SyncInfo->enumSdkModule).c_str(), enumCvt.Enum2String(pG_SyncInfo->enumSyncType).c_str());
}
void SetGSync::Set(G_SyncMp4& l_SycnMp4) {
    EnumCvt enumCvt;
    pG_SyncInfo->g_SyncMp4 = l_SycnMp4;
    pG_SyncInfo->enumRunModule = RUN_MODULE_MP4;
    //LOG_DEBUG("set Mp4_GSync, SDK_MODULE:%s\n", enumCvt.Enum2String(pG_SyncInfo->enumSdkModule).c_str());
}
void SetGSync::Set(G_SyncImg& l_SycnImg) {
    EnumCvt enumCvt;
    pG_SyncInfo->g_SyncImg = l_SycnImg;
    pG_SyncInfo->enumRunModule = RUN_MODULE_IMG;
    //LOG_DEBUG("set Img_GSync, SDK_MODULE:%s\n", enumCvt.Enum2String(pG_SyncInfo->enumSdkModule).c_str());
}
void SetGSync::Set(G_SyncUnit& l_SycnUnit) {
    EnumCvt enumCvt;
    pG_SyncInfo->g_SyncUnit = l_SycnUnit;
    pG_SyncInfo->enumRunModule = RUN_MODULE_UNIT;
    //LOG_DEBUG("set Unit_GSync, SDK_MODULE:%s\n", enumCvt.Enum2String(pG_SyncInfo->enumSdkModule).c_str());
}
void SetGSync::InitAll(G_SyncInfo& Main2Putdata_DMS,G_SyncInfo& Main2Putdata_OOMS,G_SyncInfo& Putdata2Callback_DMS, G_SyncInfo& Putdata2Callback_OOMS){
    EnumCvt enumCvt;
    Main2Putdata_DMS.enumRunModule = RUN_MODULE_DEFAULT;
    Main2Putdata_DMS.enumSdkModule = SDK_MODULE_DMS;
    Main2Putdata_DMS.enumSyncType = SYNC_TYPE_MAIN_TO_DATAPUT;
    //LOG_DEBUG("reset GSync, SDK_MODULE:%s, SYNC_TYPE:%s\n", enumCvt.Enum2String(Main2Putdata_DMS.enumSdkModule).c_str(), enumCvt.Enum2String(Main2Putdata_DMS.enumSyncType).c_str());

    Main2Putdata_OOMS.enumRunModule = RUN_MODULE_DEFAULT;
    Main2Putdata_OOMS.enumSdkModule = SDK_MODULE_OMS;
    Main2Putdata_OOMS.enumSyncType = SYNC_TYPE_MAIN_TO_DATAPUT;
    //LOG_DEBUG("reset GSync, SDK_MODULE:%s, SYNC_TYPE:%s\n", enumCvt.Enum2String(Main2Putdata_OOMS.enumSdkModule).c_str(), enumCvt.Enum2String(Main2Putdata_OOMS.enumSyncType).c_str());

    Putdata2Callback_DMS.enumRunModule = RUN_MODULE_DEFAULT;
    Putdata2Callback_DMS.enumSdkModule = SDK_MODULE_DMS;
    Putdata2Callback_DMS.enumSyncType = SYNC_TYPE_DATAPUT_TO_CALLBACK;
    //LOG_DEBUG("reset GSync, SDK_MODULE:%s, SYNC_TYPE:%s\n", enumCvt.Enum2String(Putdata2Callback_DMS.enumSdkModule).c_str(), enumCvt.Enum2String(Putdata2Callback_DMS.enumSyncType).c_str());

    Putdata2Callback_OOMS.enumRunModule = RUN_MODULE_DEFAULT;
    Putdata2Callback_OOMS.enumSdkModule = SDK_MODULE_OMS;
    Putdata2Callback_OOMS.enumSyncType = SYNC_TYPE_DATAPUT_TO_CALLBACK;
    //LOG_DEBUG("reset GSync, SDK_MODULE:%s, SYNC_TYPE:%s\n", enumCvt.Enum2String(Putdata2Callback_OOMS.enumSdkModule).c_str(), enumCvt.Enum2String(Putdata2Callback_OOMS.enumSyncType).c_str());
}

