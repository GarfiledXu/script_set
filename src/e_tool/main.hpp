#ifndef _MAIN_HPP_
#define _MAIN_HPP_
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
#include "MyGbuffer.hpp"
// #include "MyFlags.hpp"
#include "MySdk.hpp"
#include "MyAdapt.hpp"
#include "MyCallback.hpp"

// #include "testCode.hpp"
using json = nlohmann::json;


auto func_get_license = [](const std::string& txt_path)->std::string {
        std::ifstream inStmLicense;
        inStmLicense.open((txt_path));
        if (!inStmLicense.good()) {
            LOG_ERROR("file not exsit, path:%s\n", txt_path.c_str());
        }
        std::string strTemp;std::string strOut;
        while (std::getline(inStmLicense, strTemp)) {
            strOut = strOut + strTemp;
        }
        inStmLicense.close();
        return strOut;
};

void thread_dms(TaskImpl& task_info) {
    LOG_INFO("enter task !\n");
    std::vector<std::string> path_vec;//get img
    GetFileListRecursion(task_info.get().dms.pic_dir_path, path_vec, { ".jpg" });
    LOG_INFO("get filelist:%d\n", path_vec.size());
    //sdk
    ASD_SetDmsDetectSwitch(g_handle, task_info.get_dms_detect_mask(task_info.get().detect_mask));
    ASD_SetOmsDetectSwitch(g_handle, task_info.get_oms_detect_mask(task_info.get().detect_mask));
    
    int foreach_count = 0;
    int img_list_count = 0;
    long push_count = 0;
    for (int i = 0;i < task_info.get().foreach_time;i++) {
        ++foreach_count;
        LOG_INFO("enter foreach time [%d, %d]\n", foreach_count, task_info.get().foreach_time);
        img_list_count = 0;
        for (auto& it : path_vec) {
            // ASD_UninitAlgHandle(g_handle);
            
            img_list_count++;
            LOG_INFO("enter foreach path:%s\n", it.c_str());
            int w_continue = 1;
            std::thread thread_timer([](int* cont, int sleep_ms) {
                LOG_INFO("enter sleep loop!\n");
                usleep(sleep_ms * 1000);
                *cont = 0;
                LOG_INFO("out sleep loop!\n");
                }, & w_continue, task_info.get().each_time_ms);
            
            thread_timer.detach();
            push_count = 0;
            tv::Mat mat = tv::imread(it, 1);
            tv::FourccBuffer fbuff;
            tv::cvtToFourcc(mat, fbuff, tv::PIX_FMT_UYVY422);
            ASVLOFFSCREEN arc_buff = tv::cvtToArc(fbuff);
            ASD_SetDmsDetectSwitch(g_handle, task_info.get_dms_detect_mask(task_info.get().detect_mask));
            // ASD_SetOmsDetectSwitch(g_handle, task_info.get_oms_detect_mask(task_info.get().detect_mask));
            while (w_continue)
            {
                push_count++;
                LOG_DEBUG("dms perform, foreach[%d, %d], pic_list[%d, %d], push count:%ld\npath:%s\n", foreach_count, task_info.get().foreach_time, img_list_count, path_vec.size(), push_count, it.c_str());
                //pushframe
                //sync g_buff
                ASD_PushDmsFrame(g_handle, &arc_buff, task_info.get().speed);
                // PicInfo pic;pic.path = it;
                // g_p_sync_dms->set(pic);

                G_SyncInfo l_syncinfo;
                l_syncinfo.enumSyncType = SYNC_TYPE_DATAPUT_TO_CALLBACK;
                l_syncinfo.enumRunModule = RUN_MODULE_IMG;
                l_syncinfo.enumSdkModule = SDK_MODULE_DMS;
                G_SyncImg l_img;
                l_img.imgPath = it;
                l_syncinfo.g_SyncImg = l_img;
                g_p_sync_dms_default2->set(l_syncinfo);
                
                usleep(30 * 1000);
            }
            LOG_INFO("out foreach loop!\n");

            ASD_CheckLicense("FFFFFFFF", func_get_license("./license_xby.dat").c_str());
            // ASD_INIT_PARAM initParam;
            // initParam.camMask = ASD_CAM_MASK_ALL;
            // initParam.dms_CamAngle = 0;
            // initParam.JsonPath="./arc_cfg.json";
            // ASD_InitAlgHandle(&g_handle, &initParam, CB_JSON);
        }
    }
}

void thread_oms(TaskImpl& task_info) {
    std::vector<std::string> path_vec;//get img
    GetFileListRecursion(task_info.get().oms.pic_dir_path, path_vec, { ".jpg" });

    ASD_SetDmsDetectSwitch(g_handle, task_info.get_dms_detect_mask(task_info.get().detect_mask));
    ASD_SetOmsDetectSwitch(g_handle, task_info.get_oms_detect_mask(task_info.get().detect_mask));
    int foreach_count = 0;
    int img_list_count = 0;
    long push_count = 0;
    for (int i = 0;i < task_info.get().foreach_time;i++) {
        foreach_count++;
        img_list_count = 0;
        for (auto& it : path_vec) {
            // ASD_UninitAlgHandle(g_handle);

            
            img_list_count++;
            int w_continue = 1;
            std::thread thread_timer([](int* cont, int sleep_ms) {
                usleep(sleep_ms*1000);
                *cont = 0;
            }, &w_continue, task_info.get().each_time_ms);
            thread_timer.detach();
            
            push_count = 0;
            tv::Mat mat = tv::imread(it, 1);
            tv::FourccBuffer fbuff;
            tv::cvtToFourcc(mat, fbuff, tv::PIX_FMT_UYVY422);
            ASVLOFFSCREEN arc_buff = tv::cvtToArc(fbuff);
            // ASD_SetDmsDetectSwitch(g_handle, task_info.get_dms_detect_mask(task_info.get().detect_mask));
            ASD_SetOmsDetectSwitch(g_handle, task_info.get_oms_detect_mask(task_info.get().detect_mask));
            while (w_continue)
            {
                push_count++;
                //pushframe
                //sync g_buff
                LOG_DEBUG("oms perform, foreach[%d, %d], pic_list[%d, %d], push count:%ld\npath:%s\n", foreach_count, task_info.get().foreach_time, img_list_count, path_vec.size(), push_count, it.c_str());
                ASD_PushOmsFrame(g_handle, &arc_buff);
                // PicInfo pic;pic.path = it;
                // g_p_sync_oms->set(pic);

                G_SyncInfo l_syncinfo;
                l_syncinfo.enumSyncType = SYNC_TYPE_DATAPUT_TO_CALLBACK;
                l_syncinfo.enumRunModule = RUN_MODULE_IMG;
                l_syncinfo.enumSdkModule = SDK_MODULE_OMS;
                G_SyncImg l_img;
                l_img.imgPath = it;
                l_syncinfo.g_SyncImg = l_img;
                g_p_sync_oms_default2->set(l_syncinfo);
                
                usleep(30 * 1000);
            }
            ASD_CheckLicense("FFFFFFFF", func_get_license("./license_xby.dat").c_str());

            // ASD_INIT_PARAM initParam;
            // initParam.camMask = ASD_CAM_MASK_ALL;
            // initParam.dms_CamAngle = 0;
            // initParam.JsonPath="./arc_cfg.json";
            // ASD_InitAlgHandle(&g_handle, &initParam, CB_JSON);
            
        }
    }
}



#endif