#include "main.hpp"
// #include "MyFlags.hpp"
#include "sdk-pkg.hpp"
#include "threadpool.hpp"
//模型设计： 一个线程池只包含一个线程，并支持线程池间任务同步控制
// std::unique_ptr<std::vector<ThreadPool>> g_p_thread_vec(new std::vector<ThreadPool>());
// void PoolSelecter() {
    
// }
// typedef enum MODULE { dms, oms, faceid };
// typedef enum METIRAL{pic, video, uut};

int g_arclog_level = 3;
int32_t LOG_LEVEL_MASK = EROR | VEBO | PRINT | INFO | DBUG | COLOR | EXTR;
char MYLOG_PATH[200] = "./my_log.txt";

int main(int argc, char** argv) {
    
    LoadTaskTxt("/var/xjf2613/bin/task_cfg.json");
    
    //sdk init
    ASD_CheckLicense("FFFFFFFF", func_get_license("./license_xby.dat").c_str());
    ASD_INIT_PARAM initParam;
    initParam.camMask = ASD_CAM_MASK_ALL;
    initParam.dms_CamAngle = 0;
    initParam.JsonPath="./arc_cfg.json";
    ASD_InitAlgHandle(&g_handle, &initParam, CB_JSON);

    //review all task
    int count = 0;
    for (auto& task : *g_p_out_vec) {
        count++;
        LOG_EXTR("[%d, %d] task, json:%s\n", count, g_p_out_vec->size(), task.to_json().dump(4).c_str());
    }
    
    ////run all task
    LOG_INFO("start run task!\n");
    int record_count = 0;
    for (auto& taskinfo : *g_p_out_vec) {
        record_count++;
        std::thread thr_dms, thr_oms;
        if (taskinfo.get().dms.enable) {
            LOG_INFO("create dms task thread!\n");
            thr_dms = std::thread(thread_dms, std::ref(taskinfo));
        }
        if (taskinfo.get().oms.enable) {
            LOG_INFO("create oms task thread!\n");
            thr_oms = std::thread(thread_oms, std::ref(taskinfo));
        }
        else {
            LOG_ERROR("[%d, %d] task break! content:%s\n", record_count, g_p_out_vec->size(), taskinfo.to_json().dump().c_str());
        }
        LOG_INFO("start join task thread!\n");
        if (thr_dms.joinable())
            thr_dms.join();
        if(thr_oms.joinable())
            thr_oms.join();
        LOG_INFO("end join task thread!\n");
    }

    //sdk uninit
    ASD_UninitAlgHandle(g_handle);

    return 0;
}
