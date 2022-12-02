#include <stdio.h>
#include <iostream>
using namespace std;

#include "ArcDriveSDK.h"

#define  DMS_CAMERA_WIDTH 1280
#define  DMS_CAMERA_HEIGHT 800
#define  OMS_CAMERA_WIDTH 1920
#define  OMS_CAMERA_HEIGHT 1080

bool runFlag = true;


void processDmsAlarm(ASD_DMS_RESULT* result);
void processOmsAlarm(ASD_OMS_RESULT* result);

void alarmProcessCallback(ASD_EVENT_TYPE msgid, void* msg, int msg_len)
{
	//printf("alarm %d received, msglen:%d \n", msgid, msg_len);
	switch(msgid)
	{
	case ASD_EVENT_DMS:
	{
		ASD_DMS_RESULT* dmsResult = (ASD_DMS_RESULT*)msg;
		//printf("DMS_ALARM type:%d \n", dmsResult->alarmType);
		processDmsAlarm(dmsResult);
	}
	break;

	case ASD_EVENT_OMS:
	{
		ASD_OMS_RESULT* omsResult = (ASD_OMS_RESULT*)msg;
		processOmsAlarm(omsResult);
	}
	break;

	default:
		break;
	}
}


void processDmsAlarm(ASD_DMS_RESULT* result)
{
	switch(result->alarmType)
	{
	case     ASD_ALARM_TYPE_DISTRACT:
		printf("---------Distract Alarm , level:%d \n", result->distractLevel);
		break;
	case     ASD_ALARM_TYPE_DROWSY:
		printf("---------Drowsy Alarm , level:%d \n", result->drowsyLevel);
		break;
	case     ASD_ALARM_TYPE_DANGER_ACTION:
		printf("---------Danger Action Alarm , level:%d \n", result->alarmDetail.dangerAction);
		break;
	case 	ASD_ALARM_TYPE_EXPRESSION:
		printf("---------Expression Alarm , level:%d \n", result->alarmDetail.expressionType);
		break;
	default:
		break;
	}
	if(result->gestureType == ASD_GESTURE_TYPE_MUTE) //mute gesture for dms only
	{
		printf("Gesture Detected , mute gesture:%d \n", result->gestureType);
	}

    if(result->actionType != ASD_ACTION_TYPE_NONE)            // action type, nod or shake
    {
    	printf("action Detected , type:%d \n", result->actionType);
    }

    if(result->alarmGazeArea > ASD_GESTURE_TYPE_INVALID && result->alarmGazeArea <= ASD_GAZE_AREA_OTHER)      //gaze area detected
    {
    	printf("gaze area Detected , area:%d \n", result->alarmGazeArea);
    }

    //printf("HeadPose roll:%f, yaw:%f, pitch:%f \n", result->headPoseData.roll, result->headPoseData.yaw, result->headPoseData.pitch);
}


void processOmsAlarm(ASD_OMS_RESULT* result)
{

	printf("processOmsAlarm, result iFaceCount %d \n", result->iFaceCount);

	for(int i=0; i<result->iFaceCount; i++)
	{
		printf("[%d] seat :%d, alarmType:%d \n", i, result->occupantInfos[i].seatid, result->occupantInfos[i].alarmType);


		if( result->occupantInfos[i].age !=0 || result->occupantInfos[i].gender != 0)
		{
			printf("seat %d, Age:%d, gender:%d\n", result->occupantInfos[i].seatid, result->occupantInfos[i].age, result->occupantInfos[i].gender);
		}

		if(result->occupantInfos[i].alarmType == ASD_ALARM_TYPE_DANGER_ACTION)
		{
			printf("seat %d, danger action alarm:%d \n", result->occupantInfos[i].seatid, result->occupantInfos[i].alarmDetail.dangerAction);
		}
		else if(result->occupantInfos[i].alarmType == ASD_ALARM_TYPE_EXPRESSION)
		{
			printf("seat %d, expression alarm:%d \n", result->occupantInfos[i].seatid, result->occupantInfos[i].alarmDetail.expressionType);
		}

	}

	if(result->gestureType != ASD_GESTURE_TYPE_INVALID)
	{
		printf("oms gesture detected: %s ", result->gestureType);
	}

	if(result->babyResult.badyNum  > 0)
	{
		for(int i=0; i<result->babyResult.badyNum; i++)
		{
			printf("baby[%d]: type %d", i, result->babyResult.babyType[i]);
		}
	}
	if(result->hasPet == 1)
	{
			printf("pet detected !");
	}
	if(result->hasResidue == 1)
	{
		printf("Residue detected !");
	}
}


unsigned char *getPic(const char *path, long &size) {
    FILE *fp1 = fopen(path, "rb");

    if (!fp1) {
        printf("fopen failed\n");
        return nullptr;
    }

    printf("open file :%s \n", path);
    fseek(fp1, 0, SEEK_END);
    size = ftell(fp1);
    fseek(fp1, 0, SEEK_SET);
    unsigned char *data = (unsigned char *) malloc(size);
    fread(data, 1, size, fp1);
    fclose(fp1);
    return data;
}

bool pickFrame(ASVLOFFSCREEN &faceidFrame, unsigned fmt, int w, int h, unsigned char *frame) {
	if(!frame)
	{
		return false;
	}

	faceidFrame.u32PixelArrayFormat = fmt;
	faceidFrame.i32Width = w;
	faceidFrame.i32Height = h;
	if(fmt == ASVL_PAF_UYVY)
	{
		faceidFrame.pi32Pitch[0] = w * 2;
		faceidFrame.ppu8Plane[0] = frame;
		return true;
	}else{
		return false;
	}
}


void simulateFaceidFuction(void* arg)
{
	ASD_ALG_HANDLE pHandle = (ASD_ALG_HANDLE)arg;
	int32_t ret = 0;

	//register face 1, get the feature of the face 1
	ASVLOFFSCREEN ImageInput = {0};
    long size = 0;
	unsigned char *frame = getPic("./1.UYVY", size);
	if(!pickFrame(ImageInput, ASVL_PAF_UYVY, 1280, 800, frame))
	{
		return;
	}
    char feature1[1024] = {0};
    ret = ASD_FaceRegister( pHandle, &ImageInput, ASD_CAM_MASK_DMS, feature1);
    if (ret < 0) {
    	printf("face register err ret:%d \n", ret );
        return ;
    }
    std::cout << "feature1:" << feature1 << std::endl;

    //get the feature of the face 2
    ASVLOFFSCREEN ImageInput2 = {0};
    unsigned char *frame2 =  getPic("2.UYVY", size);
    if(!frame2)
    {
    	return ;
    }
    if(!pickFrame(ImageInput2, ASVL_PAF_UYVY, 1280, 800, frame2))
    {
    	return ;
    }
    char feature2[1024] = {0};
    ret = ASD_FaceRecognition( pHandle, &ImageInput2, ASD_CAM_MASK_DMS, feature2);
    if (ret < 0) {
        printf("face recognition err ret:%d \n", ret );
        return ;
    }
    std::cout << "feature2:" << feature2 << std::endl;

    //compare two faces, if the similarity exceeds 0.75, then match success; else match failed
    double result = 0;
    ret = ASD_FaceMatch(pHandle, feature1, feature2, &result);
    if (ret < 0) {
        printf("face compare err ret:%d \n", ret );
        return;
    }
    printf( "similarity: %lf \n",  result);
    if(result >= 0.75)
    {
    	printf("Face match success !\n");
    }

    if(frame)
    {
    	free(frame);
    }

    if(frame2)
    {
    	free(frame2);
    }
}

void* dmsAlgoRunThread(void* arg){

	ASD_ALG_HANDLE pHandle = (ASD_ALG_HANDLE)arg;
	int32_t ret = 0;
	int32_t speed = 15;
	long size = 0;
	ASVLOFFSCREEN sImage = {0};
	unsigned char *frame = getPic("./1280x800_normal.UYVY", size);
	if(!pickFrame(sImage, ASVL_PAF_UYVY, DMS_CAMERA_WIDTH, DMS_CAMERA_HEIGHT, frame))
	{
		return nullptr;
	}


	while(runFlag)
	{
			//DMS检测，一直送帧，从回调获取报警结果
			ret = ASD_PushDmsFrame(pHandle, &sImage, speed);
	}

	if(frame)
	{
		free(frame);
	}
	pthread_exit(nullptr);
	return nullptr;
}

void* omsAlgoRunThread(void* arg){

	ASD_ALG_HANDLE pHandle = (ASD_ALG_HANDLE)arg;
	int32_t ret = 0;
	long size = 0;
	ASVLOFFSCREEN sImage = {0};
	unsigned char *frame = getPic("./1920x1080_normal.UYVY", size);
	if(!pickFrame(sImage, ASVL_PAF_UYVY, OMS_CAMERA_WIDTH, OMS_CAMERA_HEIGHT, frame))
	{
		return nullptr;
	}


	while(runFlag)
	{
			//OMS检测，一直送帧，从回调获取报警结果
			//如果需要进行触发性检测，如年龄性别，遗留物，遗留生物，可调用ASD_SetOmsDetectMask一次进行设置，结果回执后，再关闭检测
			ret = ASD_PushOmsFrame(pHandle, &sImage);
	}

	if(frame)
	{
		free(frame);
	}
	pthread_exit(nullptr);
	return nullptr;
}

int main() {

	printf("sdk version:%s \n", ASD_GetSDKVersion());
	ASD_ALG_HANDLE pHandle = nullptr;

	const char* SIN = nullptr;
	const char* vehicleID = "EP40";

#if 0
	/*  Notes about license:
	 *
	 * 1. Use API ASD_GetLicenseReqInfo to get license request string.
	 * 2. Send license request string to Android, to get license ACK.
	 * 3. Use API ASD_CheckLicense to check license.
	 * 	  if license check failed, ASD_InitAlgHandle will be failed.
	 * */

	const char* license_req = ASD_GetLicenseReqInfo(SIN, vehicleID);
	printf("license_req:%s , strlen %d \n", license_req, strlen(license_req));

	//send license request info license_req to Android to get license ACK ...
	//save license info into local file "license.dat"
	//char* licsensestr = loadFile("license.dat");
#endif

	char* licsensestr = "XXX";
	int32_t ret = ASD_CheckLicense(SIN, licsensestr);
	if(ret != ASD_OK)
	{
		printf("license check failed!\n");
		return -1;
	}

	ASD_INIT_PARAM initParam = {0};
	initParam.camMask = ASD_CAM_MASK_ALL; //ASD_CAM_MASK_DMS / ASD_CAM_MASK_OMS;
	initParam.dms_CamAngle = 0;

	ret = ASD_InitAlgHandle( &pHandle, &initParam, alarmProcessCallback);
	if(ret != 0)
	{
		printf("init alg handle failed %d \n", ret);
		return -1;
	}

	//FACEID 功能用两张静态图片示例
	simulateFaceidFuction(pHandle);

	//DMS 功能用静态图片模拟送帧示例
	pthread_t dmsAlgoThreadId = 0;
	pthread_create(&dmsAlgoThreadId, nullptr, dmsAlgoRunThread, pHandle);

	//OMS 功能用静态图片模拟送帧示例
	pthread_t omsAlgoThreadId = 0;
	pthread_create(&omsAlgoThreadId, nullptr, omsAlgoRunThread, pHandle);

	while(runFlag)
	{
		usleep(40000);
	}

	if(dmsAlgoThreadId != 0)
		pthread_join(dmsAlgoThreadId, nullptr);

	if(omsAlgoThreadId != 0)
		pthread_join(omsAlgoThreadId, nullptr);

    ASD_UninitAlgHandle(pHandle);
	return 0;
}
