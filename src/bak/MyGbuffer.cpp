#include "MyGbuffer.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   GBuffer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
pthread_mutex_t mtx_dms_faceid_putdata = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  mtx_main_dmsput_data = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  mtx_main_oomsput_data = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t mtx_dmsput_dms_callbackinfo = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mtx_oomsput_ooms_callbackinfo = PTHREAD_MUTEX_INITIALIZER;

std::string g_defaultImg_dms = "./dms_default_img.jpg";
std::string g_defaultImg_ooms = "./ooms_default_img.jpg";

G_SyncInfo g_Sync_Main2Putdata_DMS;
G_SyncInfo g_Sync_Main2Putdata_OOMS;
G_SyncInfo g_Sync_Putdata2Callback_DMS;
G_SyncInfo g_Sync_Putdata2Callback_OOMS;

DataThreadCounter G_threadCounter;

//2022-02-28
pthread_mutex_t mtx_camera_pushdata_dms = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t  mtx_camera_pushdata_oms = PTHREAD_MUTEX_INITIALIZER;
unsigned char G_CameraImgBuff_Dms[DMS_BUFF_LENGTH] = { 0 };
unsigned char G_CameraImgBuff_Oms[OMS_BUFF_LENGTH] = { 0 };
