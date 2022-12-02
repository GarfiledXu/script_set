#ifndef _MY_CAMERA_HPP_
#define _MY_CAMERA_HPP_
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <iostream>
#include <thread>
#include <list>
#include <deque>

#include "pmem_id.h"
#include "pmem.h"
#include "qcarcam_types.h"
#include "qcarcam.h"
//
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h> 
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "utils.h"
#include "jftools.hpp"
#include "json.hpp"
#include "MyEasyLog.hpp"
#include "MyAdapt.hpp"
#include "MyGbuffer.hpp"
#include "main.hpp"

#define  DMS_CAMERA_WIDTH 1600
#define  DMS_CAMERA_HEIGHT 1300

#define  OMS_CAMERA_WIDTH 1600
#define  OMS_CAMERA_HEIGHT 1300

////////////////////////qcar cam define///////////////////////////

#define QCARCAM_TEST_DEFAULT_GET_FRAME_TIMEOUT 500000000
#define NUM_MAX_CAMERAS 1
#define BUFSIZE 32
#define SIGWAIT_TIMEOUT_MS 100


//DMS CAMERA
#define DMS_CAMERA_FORMAT QCARCAM_FMT_UYVY_8
#define CAMERA_NUM_BUFFERS 5
#define DMS_CAMERA_ID QCARCAM_INPUT_TYPE_OCCUPANT
////#define DMS_CAMERA_ID QCARCAM_INPUT_TYPE_EXT_FRONT

//OMS CAMERA
#define OMS_CAMERA_FORMAT QCARCAM_FMT_UYVY_8
#define OMS_CAMERA_ID QCARCAM_INPUT_TYPE_GESTURE//2//QCARCAM_INPUT_TYPE_OCCUPANT

typedef struct
{
    qcarcam_buffers_t p_buffers;
    qcarcam_color_fmt_t format;
    unsigned int n_buffers;
    unsigned int width;
    unsigned int height;
} qcarcam_test_buffers_param_t;

typedef struct
{
    void* qcarcam_thread_handle;

    /*qcarcam context*/
    qcarcam_hndl_t qcarcam_context;
    qcarcam_input_desc_t qcarcam_input_id;

    qcarcam_test_buffers_param_t buffers_param;
    qcarcam_buffer_t pBuffers[CAMERA_NUM_BUFFERS];
    void* p_pmem[CAMERA_NUM_BUFFERS];
    qcarcam_buffers_t p_buffers_output;

    unsigned long long int frame_timeout;
    int use_event_callback;

    pthread_mutex_t mutex_close_camera;
    int state;

    int frameCnt;
    int is_running;

} qcarcam_test_input_t;

int qcarcam_init();
int qcarcam_uninit();

/////////////////////////qcar cam define ends//////////////////////////////


//dms
void* start_dms_capture(void* pArg);
void stop_dms_capture();

//oms
void* start_oms_capture(void* pArg);
void stop_oms_capture();

void* thread_camera_check(void* pArg);
void stop_camera_check();


//new
#define USE_PMEM_V2

// //dms
// extern int g_do_dms;
// extern uint8_t* dms_pImageBuffer;
// extern pthread_mutex_t g_dms_lock;
// extern pthread_cond_t cond_do_dms;
// extern bool dms_process_is_running;
// //oms
// extern int g_do_oms;
// extern uint8_t* oms_pImageBuffer;
// extern pthread_mutex_t g_oms_lock;
// extern pthread_cond_t cond_do_oms;
// extern bool oms_process_is_running;

//  extern qcarcam_test_input_t gDmsInput;
//  extern qcarcam_test_input_t gOmsInput;
// extern int camera_check_running;

#endif




