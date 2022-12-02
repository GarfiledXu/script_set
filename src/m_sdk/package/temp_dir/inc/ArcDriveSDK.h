#ifndef _ARCDRIVESDK_H_
#define _ARCDRIVESDK_H_

#include <stdint.h>
#include <stddef.h>
#include "asvloffscreen.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ASD_OK					0
#define ASD_ERR_VSDK			-50000
#define	ASD_ERR_INVALID_PARAM 	-50001
#define ASD_ERR_LICENSE		  	-50002
#define ASD_ERR_MEM				-50003
#define ASD_ERR_DMS				-50010
#define ASD_ERR_DMS_INIT		-50011
#define ASD_ERR_OMS				-50020
#define ASD_ERR_OMS_INIT		-50021
#define ASD_ERR_FACEID			-50100
#define ASD_ERR_FACEID_INIT		-50101
#define ASD_ERR_FD				-50102
#define ASD_ERR_MULTIFACE		-50103
#define ASD_ERR_FQ				-50104
#define ASD_ERR_FQ_ANGLE		-50105
#define ASD_ERR_FQ_COVER		-50106
#define ASD_ERR_FQ_ILLU			-50107
#define ASD_ERR_FQ_OTHER		-50108
#define ASD_ERR_LF				-50109
#define ASD_ERR_FR				-50110
#define ASD_ERR_NOFACE			-50111
#define ASD_ERR_FACE_ATTR		-50112
#define ASD_ERR_INVALID_FEATURE -50113

//Init camera mask setting
#define ASD_CAM_MASK_NONE		0x00000000
#define ASD_CAM_MASK_DMS        0x00000001
#define ASD_CAM_MASK_OMS        0x00000002
#define ASD_CAM_MASK_ALL (ASD_CAM_MASK_DMS | ASD_CAM_MASK_OMS)
typedef uint32_t ASD_CAM_MASK;

//DMS Detect mask setting
#define ASD_MASK_NONE 0x00000000
#define ASD_MASK_DISTRACT        0x00000001  // 分心
#define ASD_MASK_DROWSY          0x00000002  // 疲劳
#define ASD_MASK_DANGE_ACTION    0x00000004  // 危险动作
#define ASD_MASK_NODSHAKE        0x00000008  // 点头摇头
#define ASD_MASK_GAZEAREA        0x00000010  // 视线区域
#define ASD_MASK_EXPRESSION      0x00000020  // 主驾表情
#define ASD_MASK_GESTURE         0x00000040  // 手势
#define ASD_MASK_HEADPOSE        0x00000080  // 眉心坐标
#define ASD_MASK_AGE_GENDER      0x00000800			//DMS 年龄性别检测

#define ASD_MASK_DMS_ALL                                                                                 \
    (ASD_MASK_DISTRACT | ASD_MASK_DROWSY | ASD_MASK_DANGE_ACTION | ASD_MASK_NODSHAKE | ASD_MASK_GAZEAREA \
     | ASD_MASK_EXPRESSION | ASD_MASK_GESTURE | ASD_MASK_HEADPOSE )

//OMS Detect mask setting
#define ASD_OMS_MASK_DANGE_ACTION ASD_MASK_DANGE_ACTION  // 危险动作
#define ASD_OMS_MASK_EXPRESSION   ASD_MASK_EXPRESSION    // 表情
#define ASD_OMS_MASK_GESTURE      0x00000080             // 手势
#define ASD_OMS_MASK_BABY         0x00000100             // 婴儿
#define ASD_OMS_MASK_HUMEN_PET    0x00000200			 // 人 宠物
#define ASD_OMS_MASK_RESIDUE      0x00000400             // 遗留物
#define ASD_OMS_MASK_AGE_GENDER   0x00000800			// 年龄性别检测

#define ASD_MASK_OMS_DEFAULT                                                         \
        (ASD_OMS_MASK_DANGE_ACTION | ASD_OMS_MASK_EXPRESSION | ASD_OMS_MASK_GESTURE )

typedef uint32_t ASD_DETECT_MASK;

typedef enum tagASD_EVENT_TYPE {
	ASD_EVENT_DMS = 0,
	ASD_EVENT_OMS,
    ASD_EVENT_DMS_TIMER,    //dms event timer
    ASD_EVENT_DMS_AGE_GENDER , // corrding to ASD_DRIVER_AGE_GNEDER_INFO
	ASD_EVENT_MAX,
}ASD_EVENT_TYPE;


typedef struct tagASD_INIT_PARAM {
	ASD_CAM_MASK camMask;
	int32_t dms_CamAngle;
    const char* JsonPath;
}ASD_INIT_PARAM;

//////////////////////////////////

typedef struct tagASD_HEAD_POSE_DATA {
	MFloat confidence;
    MFloat roll;
    MFloat yaw;
    MFloat pitch;
    MFloat headPoseX;
    MFloat headPoseY;
    MFloat headPoseZ;
} ASD_HEAD_POSE_DATA;

typedef enum tagASD_ALARM_TYPE {
    ASD_ALARM_TYPE_NONE,
    ASD_ALARM_TYPE_DISTRACT,
    ASD_ALARM_TYPE_DROWSY,
    ASD_ALARM_TYPE_EXPRESSION,
    ASD_ALARM_TYPE_CALL,
    ASD_ALARM_TYPE_SMOKE,
} ASD_ALARM_TYPE;

typedef enum tagASD_DISTRACT_LEVEL{
    ASD_DISTRACT_LEVEL_INVALID = 0,
    ASD_DISTRACT_LEVEL_NONE,
    ASD_DISTRACT_LEVEL_LOW,
    ASD_DISTRACT_LEVEL_MIDDLE,
    ASD_DISTRACT_LEVEL_HIGH,
} ASD_DISTRACT_LEVEL;

typedef enum tagASD_DROWSY_LEVEL{
    ASD_DROWSY_LEVEL_INVALID = 0,
    ASD_DROWSY_LEVEL_NONE,
    ASD_DROWSY_LEVEL_LOW,
    ASD_DROWSY_LEVEL_MIDDLE,
    ASD_DROWSY_LEVEL_HIGH,
} ASD_DROWSY_LEVEL;

typedef enum tagASD_DANGER_ACTION{
    ASD_DANGER_ACTION_INVALID = 0,
    ASD_DANGER_ACTION_SMOKE,  //抽烟
    ASD_DANGER_ACTION_CALL,   //打电话
    ASD_DANGER_ACTION_NONE,   //无危险动作
} ASD_DANGER_ACTION;

typedef enum tagASD_ACTION_TYPE{
    ASD_ACTION_TYPE_NONE = 0,
    ASD_ACTION_TYPE_NOD,      //点头
    ASD_ACTION_TYPE_SHAKE,    //摇头
} ASD_ACTION_TYPE;

typedef enum {
    ASD_EXPRESSION_TYPE_INVALID = 0,
    ASD_EXPRESSION_TYPE_HAPPY,  //高兴
    ASD_EXPRESSION_TYPE_ANGRY,  //愤怒
    ASD_EXPRESSION_TYPE_SURPRISE, //惊讶
    ASD_EXPRESSION_TYPE_SAD,    //哭泣
    ASD_EXPRESSION_TYPE_NORMAL, //平静
} ASD_EXPRESSION_TYPE;

typedef enum tagASD_GAZE_AREA{
    ASD_GAZE_AREA_INVALID = 0,              //无效值
    ASD_GAZE_AREA_MAIN_DRIVING_WINDSCREEN,  //主驾驶前风挡
    ASD_GAZE_AREA_CENTER_CONSOLE,           //中控屏幕
    ASD_GAZE_AREA_MAIN_DRIVING_SCREEN,      //仪表屏幕
    ASD_GAZE_AREA_LEFT_REARVIEW,            //左后视镜
    ASD_GAZE_AREA_RIGHT_REARVIEW,           //右后视镜
    ASD_GAZE_AREA_REARVIEW,                 //后视镜
    ASD_GAZE_AREA_OTHER,                    //其他区域
} ASD_GAZE_AREA;

typedef enum tagASD_GESTURE_TYPE{
    ASD_GESTURE_TYPE_INVALID = 0,
    ASD_GESTURE_TYPE_MUTE,                      //比"嘘"手势，支持主驾和副驾
    ASD_GESTURE_TYPE_VSIGN,                     //v手势
    ASD_GESTURE_TYPE_OK,                        //OK手势
    ASD_GESTURE_TYPE_THUMBUP,                   //点赞
    ASD_GESTURE_TYPE_CIRCLE_CW,                 //动态手势:食指顺时针旋转
    ASD_GESTURE_TYPE_CIRCLE_CCW,                //动态手势:食指逆时针旋转
    ASD_GESTURE_TYPE_SWIPE_LEFT,                //动态手势:手掌向前然后左移
    ASD_GESTURE_TYPE_SWIPE_RIGHT,               //动态手势:手掌向前然后右移
    ASD_GESTURE_TYPE_SWIPE_FORWARD,             //动态手势:手掌向前然后从后往前移动
    ASD_GESTURE_TYPE_SWIPE_BACKWARD,            //动态手势:手掌向后然后从前往后移动
} ASD_GESTURE_TYPE;


typedef enum tagASD_BABY_TYPE{
    ASD_BABY_TYPE_INVALID = 0,           //婴儿：无
    ASD_BABY_TYPE_IN_SAFETYSEAT,         //婴儿：婴儿在安全座椅
    ASD_BABY_TYPE_OUT_SAFETYSEAT,        //婴儿：婴儿不在安全座椅
    ASD_BABY_TYPE_OTHER,                 //婴儿: 其他
} ASD_BABY_TYPE;

// 遗留物类型更新 2022-10-28
typedef enum {
    ASD_RESIDUE_TYPE_INVALID = 0,         //遗留物：无
    ASD_RESIDUE_TYPE_PHONE,               //遗留物：手机
    ASD_RESIDUE_TYPE_PACK,                //遗留物：包
    ASD_RESIDUE_TYPE_OTHER,               //遗留物：其他
} ASD_RESIDUE_TYPE;


typedef enum {
    ASD_PET_TYPE_INVALID = 0,              //宠物：无
    ASD_PET_TYPE_DOG,                      //宠物：狗
    ASD_PET_TYPE_CAT,                      //宠物：猫
    ASD_PET_TYPE_OTHER,                    //宠物：其他
} ASD_PET_TYPE;


typedef enum tagASD_SEAT_ID{
    ASD_SEAT_ID_UNKNOWN    = 0x0,
    ASD_SEAT_ID_CO_DRIVER  = 0x1,  // the co-pilot position
    ASD_SEAT_ID_REAR_LEFT  = 0x2,  // the left position of back row
    ASD_SEAT_ID_REAR_MID   = 0x3,  // the middle position of back row
    ASD_SEAT_ID_REAR_RIGHT = 0x4,  // the right position of back row
    ASD_SEAT_ID_DRIVER     = 0x5,  // the driver position
} ASD_SEAT_ID;


typedef enum tagASD_LOCATION_ID
{
    ASD_LOCATION_UNKNOW = 0X0,
    ASD_LOCATION_FRONT = 0X1,
    ASD_LOCATION_REAR = 0X2,

}ASD_LOCATION_ID;


#define ASD_OBJ_NUM_MAX 10
//baby 位置信息更新使用新的类型 ASD_LOCATION_ID . 默认大小为10
typedef struct tagASD_BABY_RESULT {
	int32_t       badyNum;
   ASD_LOCATION_ID seatId[ASD_OBJ_NUM_MAX];
} ASD_BABY_RESULT;


// define passenger gender
typedef enum tagASD_PASSENGER_GENDER {
  ASD_PASSENGER_GENDER_UNKNOWN = 0,
  ASD_PASSENGER_GENDER_MALE,
  ASD_PASSENGER_GENDER_FEMALE,
} ASD_PASSENGER_GENDER;

//ASD_EVENT_DMS result
typedef struct tagASD_DMS_RESULT {
    ASD_ALARM_TYPE alarmType;                 // alarm type
    union {
        ASD_DANGER_ACTION dangerAction;       // danger action
        ASD_EXPRESSION_TYPE expressionType;   // expression type
    } alarmDetail;
    ASD_DISTRACT_LEVEL distractLevel;         // distract level
    ASD_DROWSY_LEVEL   drowsyLevel;           // drowsy level
    ASD_GAZE_AREA      alarmGazeArea;         // gaze area, it is ASD_GAZE_AREA_INVALID if not alarm
    ASD_ACTION_TYPE    actionType;            // action type, nod or shake
    ASD_GESTURE_TYPE   gestureType;           // dms detect gesture type
    ASD_HEAD_POSE_DATA headPoseData;          // head posture data
    int isheadPoseDataValid;                  //head posture data is valid 0--invalid   1----valid
} ASD_DMS_RESULT;

//dms timer alarm info
typedef struct tagASD_DMS_TIMER_RESULT
{
    ASD_DISTRACT_LEVEL distractLevel;         // distract level
    ASD_DROWSY_LEVEL   drowsyLevel;           // drowsy level
    
}ASD_DMS_TIMER_RESULT;

//ASD_EVENT_DMS_AGE_GENDER
typedef struct tagASD_DMS_AGE_GENDER {
	int32_t errorCode;
	int32_t dirverAge;  // int32_t ageRealVal; //output real age value              
	ASD_PASSENGER_GENDER dirverGender;  //1:male    // 2:female     // 0:unknow
}ASD_DMS_AGE_GENDER;

typedef struct tagASD_OCCUPANT_INFO {
    ASD_ALARM_TYPE alarmType;  // alarm type
    union {
        ASD_DANGER_ACTION   dangerAction;      // danger action, only call&smoke, no drink
        ASD_EXPRESSION_TYPE expressionType;    // expression type
    } alarmDetail;
    ASD_SEAT_ID seatid;
    int32_t age; 
    // int32_t ageRealVal; //output real age value
    int32_t gender;// 1 male , 2 female
} ASD_OCCUPANT_INFO;

#define ASD_MAX_FACE_NUM 5
//pet 位置信息更新使用新的类型 ASD_LOCATION_ID . 默认大小为10
typedef struct tagASD_PET_RESULT {
	int32_t      petNum;
	ASD_PET_TYPE petType[ASD_OBJ_NUM_MAX];
    ASD_LOCATION_ID seatId[ASD_OBJ_NUM_MAX];
   
} ASD_PET_RESULT;

//residue 位置信息更新使用新的类型 ASD_LOCATION_ID . 默认大小为10
typedef struct tagASD_RESIDUE_RESULT {
	int32_t          residueNum;
	ASD_RESIDUE_TYPE residueType[ASD_OBJ_NUM_MAX];
    ASD_LOCATION_ID seatId[ASD_OBJ_NUM_MAX];
} ASD_RESIDUE_RESULT;


//ASD_EVENT_OMS result
typedef struct tagASD_OMS_RESULT {
    int32_t iFaceCount;
    ASD_OCCUPANT_INFO       occupantInfos[ASD_MAX_FACE_NUM];
    ASD_GESTURE_TYPE        gestureType;  //gesture type
    ASD_BABY_RESULT			babyResult;   //baby result
    ASD_RESIDUE_RESULT      residueResult;  //residue result
    ASD_PET_RESULT          petResult;      //pet  result
    int32_t hasPet;
	int32_t hasResidue;
} ASD_OMS_RESULT;

////////////////////////////////////////////////////


typedef void (*ArcEvent)(ASD_EVENT_TYPE eventType, void *msg, int32_t msgLen);
typedef void* ASD_ALG_HANDLE;


/**
 * @brief:  get license request information for requesting license from Android
 * @param1: device VIN -- VIN of the device, can be null
 * @param2: vehicleID -- vechicleID of the device, can't be null
 * @return: license request information for requesting license from Android
 * @return: nullptr -- get information failed
 */
const char* ASD_GetLicenseReqInfo(const char* VIN, const char* vehicleID);


/**
 * @brief:  check license
 * @param1: device VIN -- VIN of the device, can be null
 * @param2: license string -- input license got from Android
 * @return: ASD_OK  -- check success
 * @return: other -- check failed
 */
int32_t ASD_CheckLicense(const char* VIN, const char* license);

/**
 * @brief:  create the sdk handle
 * @param1: ASD_ALG_HANDLE* pHandle -- [out], sdk handle, pointer can't be NULL or return failed
 * @param2: ArcEvent -- callback function for notify ASD_EVENT
 * @return: ASD_OK -- init success
 * @return: other -- init failed
 */
int32_t ASD_InitAlgHandle(ASD_ALG_HANDLE* pHandle, ASD_INIT_PARAM * pParam, ArcEvent pFunc);


/**
 * @brief:  uninit sdk handle
 * @param1: pHandle -- sdk handle
 */
void ASD_UninitAlgHandle(ASD_ALG_HANDLE Handle);

/**
 * @brief:  push frame to the sdk, for DMS function only
 * @param1: pHandle -- sdk handle
 * @param2: pImage  -- frame info
 * @param3: speed   -- vehicle speed
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_PushDmsFrame(ASD_ALG_HANDLE pHandle, ASVLOFFSCREEN* pImage, int32_t speed);

/**
 * @brief:  push frame to the sdk, for OMS function only
 * @param1: pHandle -- sdk handle
 * @param2: pImage  -- frame info
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_PushOmsFrame(ASD_ALG_HANDLE pHandle, ASVLOFFSCREEN* pImage);

/**
 * @brief:  set detect switch , for DMS Alarm only
 * @param1: pHandle -- sdk handle
 * @param2: mask    -- dms detect mask, default: ASD_MASK_DMS_ALL;
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_SetDmsDetectSwitch(ASD_ALG_HANDLE pHandle, ASD_DETECT_MASK mask);

/**
 * @brief:  set detect switch , for OMS Alarm only
 * @param1: pHandle -- sdk handle
 * @param2: mask    -- oms detect mask, default: ASD_MASK_OMS_DEFAULT
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_SetOmsDetectSwitch(ASD_ALG_HANDLE pHandle, ASD_DETECT_MASK mask);

/**
 * @brief:  to fetch the registered feature data of a face,
 * @param1: pHandle -- sdk handle
 * @param2: pImage  -- frame info
 * @param3: camType  -- camera type info, should be ASD_CAM_MASK_DMS or ASD_CAM_MASK_OMS
 * @param4: pFeatureReg -- [out] registered feature buffer to be written
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_FaceRegister(ASD_ALG_HANDLE pHandle, ASVLOFFSCREEN* pImage, ASD_CAM_MASK camType, char* pFeatureReg);

/**
 * @brief:  to fetch the recognized feature data of a face,
 * @param1: pHandle -- sdk handle
 * @param2: pImage  -- frame info
 * @param3: camType  -- camera type info, should be ASD_CAM_MASK_DMS or ASD_CAM_MASK_OMS
 * @param4: pFeatureRegcog -- [out] recognized feature buffer to be written
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_FaceRecognition(ASD_ALG_HANDLE pHandle, ASVLOFFSCREEN* pImage, ASD_CAM_MASK camType, char* pFeatureRegcog);

/**
 * @brief:  to get the similarity of two faces by feature data
 * @param1: pHandle -- sdk handle
 * @param2: pFeatureReg  -- the registered feature of a face
 * @param3: pFeatureRegcog -- the recognized feature of a face
 * @param4: similarity -- [out] the similarity of two faces
 * @return: ASD_OK --  success
 * @return: other --  failed
 */
int32_t ASD_FaceMatch(ASD_ALG_HANDLE pHandle, const char* pFeatureReg,  const char* pFeatureRegcog, double* similarity);

/**
 * @brief:  get version of the sdk
 */
const char* ASD_GetSDKVersion();

#ifdef __cplusplus
}
#endif

#endif /* _ARCDRIVESDK_H_ */

