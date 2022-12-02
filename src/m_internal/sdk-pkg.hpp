#ifndef SDK_PKG_HPP
#define SDK_PKG_HPP
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <pthread.h> 
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include <memory>
#include <mutex>
#include <thread>
#include  <stdexcept>
#include "MyEasyLog.hpp"
#include "asvloffscreen.h"
#include "ArcDriveSDK.h"
#include "json.hpp"
#include <string>
#include <iostream>
#include <fstream>


// #include "MyGbuffer.hpp"
class TaskBase
{
public:
    struct TaskProp {
        std::string tag;
        int32_t speed;
        // ASD_DETECT_MASK detect_mask;
        // ASD_CAM_MASK cam_mask;
        std::string detect_mask;
        std::string camera_mask;
        int32_t each_time_ms;
        int32_t foreach_time;
        typedef struct
        {
            int32_t enable;
            std::string pic_dir_path;
        }part;
        part dms, oms;
    };
    using TaskProp = struct TaskProp;
    using json = nlohmann::json;
    TaskBase() {};
    virtual ~TaskBase() {};
    virtual void get_prop(const std::string& str_json) = 0;
    virtual void get_prop(const json& js) = 0;
    virtual json to_json() const = 0;
    virtual ASD_DETECT_MASK get_dms_detect_mask(const std::string& str_mask) = 0;
    virtual ASD_DETECT_MASK get_oms_detect_mask(const std::string& str_mask) = 0;
    virtual ASD_CAM_MASK get_camera_mask(const std::string& str_mask) = 0;
protected:
    /* data */
    TaskProp prop;
};

class TaskImpl :public TaskBase {
private:
    // static std::unique_ptr<TaskImpl> p_default_task_p;
    using TaskBase::prop;
    #define ASSIGNE_TO_STRU(js, js_key, stru_key) \
    if(js.contains(js_key)){stru_key=js[js_key];}else{};
    //#define DEFAULT_TASK default_task_p;
    //using DEFAULT_TASK = default_task_p;
    
    // static TaskProp d_prop = { .tag = "default", .speed = 30, .detect_mask = "non", .camera_mask = "non", .each_time_ms = 4000, .foreach_time = 1, {.enable = 1, .pic_dir_path = "xxx"},{.enable = 1, .pic_dir_path = "xxx"}};
    static TaskProp d_prop;
    // static TaskProp d_prop = { .tag = "default", .speed = 30, .detect_mask = "non", .camera_mask = "non", .each_time_ms = 4000, .foreach_time = 1, .dms = {.enable = 1, .pic_dir_path = "xxx"}, .oms = {.enable = 1, .pic_dir_path = "xxx"} };
    
#define DEFAULT_TASK d_prop
    template<typename T>
    static void assign_to_obj(const json& js, const std::string& js_key, T& obj, T& obj_default) {
        // std::string temp_str;
        if (js.contains(js_key)) {
            try {
                obj = js[js_key];
            }
            catch (json::type_error& ex) {
                LOG_ERROR("parse json value error! key:%s\n", js_key.c_str());
                LOG_ERROR("parse json:%s\n", js.dump(3).c_str());
                exit(1);
            }
        }
        else {
            //obj=obj_default;
            obj = obj_default;
        }
    };
   
    static void assign_to_obj(const json& js, const std::string& js_key, int& obj, int& obj_default) {
        // std::string temp_str;
        if (js.contains(js_key)) {
            try {
                obj = std::stoi(js[js_key].get<std::string>());
            }
            catch (json::type_error& ex) {
                LOG_ERROR("parse json value error! key:%s\n", js_key.c_str());
                LOG_ERROR("parse json:%s\n", js.dump(3).c_str());
                exit(1);
            }
        }
        else {
            //obj=obj_default;
            obj = obj_default;
        }
    };
    
    template<typename T>
    static void assign_to_json(json& js, const std::string& js_key, T& obj) {
        try {
            js[js_key] = obj;
        }
        catch (json::exception& ex) {
            LOG_ERROR("assign json value error! key:%s\n", js_key.c_str());
            exit(1);
        }
    };
    
    void get_prop(const std::string& str_json) override {
        json jsRead;
        try {
            jsRead = json::parse(str_json);
        }
        catch (json::parse_error& ex) {
            LOG_ERROR("parse json error! content:%s, program stop\n", str_json.c_str());
            exit(1);
        }
        get_prop(jsRead);
    };
    
    void get_prop(const json& jsRead) override {
        // ASSIGNE_TO_STRU(jsRead, "tag", prop.tag);
        static int count = 0;
        LOG_DEBUG("start assign![%d]\n", ++count);
        assign_to_obj(jsRead, "tag", prop.tag, DEFAULT_TASK.tag);
        assign_to_obj(jsRead, "speed", prop.speed, DEFAULT_TASK.speed);
        assign_to_obj(jsRead, "detect_mask", prop.detect_mask, DEFAULT_TASK.detect_mask);
        assign_to_obj(jsRead, "camera_mask", prop.camera_mask, DEFAULT_TASK.camera_mask);
        assign_to_obj(jsRead, "each_time_ms", prop.each_time_ms, DEFAULT_TASK.each_time_ms);
        assign_to_obj(jsRead, "foreach_time", prop.foreach_time, DEFAULT_TASK.foreach_time);
        assign_to_obj(jsRead, "tag", prop.tag, DEFAULT_TASK.tag);
        if (jsRead.contains("dms")) {
            try {
                auto& js = jsRead["dms"];
                assign_to_obj(js, "enable", prop.dms.enable, DEFAULT_TASK.dms.enable);
                assign_to_obj(js, "pic_dir_path", prop.dms.pic_dir_path, DEFAULT_TASK.dms.pic_dir_path);
            }
            catch(json::parse_error& ex){
                LOG_ERROR("parse value error! key:%s, program stop\n", "dms");
                exit(1);
            }
        }
        if (jsRead.contains("oms")) {
            try {
                auto& js = jsRead["oms"];
                assign_to_obj(js, "enable", prop.oms.enable, DEFAULT_TASK.oms.enable);
                assign_to_obj(js, "pic_dir_path", prop.oms.pic_dir_path, DEFAULT_TASK.oms.pic_dir_path);
            }
            catch(json::parse_error& ex){
                LOG_ERROR("parse value error! key:%s, program stop\n", "oms");
                exit(1);
            }
        }
        print_cur_prop();
        LOG_DEBUG("end assign![%d]\n", count);
    }
public:
    json to_json() const override {
        json jsout;
        assign_to_json(jsout, "tag", prop.tag);
        assign_to_json(jsout, "speed", prop.speed);
        assign_to_json(jsout, "detect_mask", prop.detect_mask);
        assign_to_json(jsout, "camera_mask", prop.camera_mask);
        assign_to_json(jsout, "each_time_ms", prop.each_time_ms);
        assign_to_json(jsout, "foreach_time", prop.foreach_time);
        assign_to_json(jsout["dms"], "enable", prop.dms.enable);
        assign_to_json(jsout["oms"], "enable", prop.oms.enable);
        assign_to_json(jsout["dms"], "pic_dir_path", prop.dms.pic_dir_path);
        assign_to_json(jsout["oms"], "pic_dir_path", prop.oms.pic_dir_path);
        return jsout;
    }
    #define FIND_STR_MASK(mask, str, out_mask) \
        if (str.find(#mask) != std::string::npos) \
        out_mask = out_mask | mask; 
    ASD_DETECT_MASK get_dms_detect_mask(const std::string& str_mask)  override {
        ASD_DETECT_MASK mask_out = 0;
        FIND_STR_MASK(ASD_MASK_NONE, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_DISTRACT, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_DROWSY, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_DANGE_ACTION, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_NODSHAKE, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_GAZEAREA, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_EXPRESSION, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_GESTURE, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_HEADPOSE, str_mask, mask_out);
        FIND_STR_MASK(ASD_MASK_AGE_GENDER, str_mask, mask_out);
        return mask_out;
    };
    ASD_DETECT_MASK get_oms_detect_mask(const std::string& str_mask)  override {
        ASD_DETECT_MASK mask_out = 0;
        FIND_STR_MASK(ASD_OMS_MASK_DANGE_ACTION, str_mask, mask_out);
        FIND_STR_MASK(ASD_OMS_MASK_EXPRESSION, str_mask, mask_out);
        FIND_STR_MASK(ASD_OMS_MASK_GESTURE, str_mask, mask_out);
        FIND_STR_MASK(ASD_OMS_MASK_BABY, str_mask, mask_out);
        FIND_STR_MASK(ASD_OMS_MASK_HUMEN_PET, str_mask, mask_out);
        FIND_STR_MASK(ASD_OMS_MASK_RESIDUE, str_mask, mask_out);
        FIND_STR_MASK(ASD_OMS_MASK_AGE_GENDER, str_mask, mask_out);
        return mask_out;
    };
    ASD_CAM_MASK get_camera_mask(const std::string& str_mask) override {
        ASD_CAM_MASK mask_out = 0;
        FIND_STR_MASK(ASD_CAM_MASK_DMS, str_mask, mask_out);
        FIND_STR_MASK(ASD_CAM_MASK_OMS, str_mask, mask_out);
        FIND_STR_MASK(ASD_CAM_MASK_ALL, str_mask, mask_out);
        return mask_out;
    };
    TaskImpl(const json& js) {
        get_prop(js);
    };
    TaskImpl(const std::string& js_str) {
        get_prop(js_str);
    };
    TaskImpl() = delete;
    TaskImpl(const TaskImpl& task) {
        prop = task.prop;
    };
    TaskImpl& operator=(const TaskImpl& task) {
        prop = task.prop;
    };
    TaskImpl(TaskImpl&& task) {
        prop = task.prop;
    };
    TaskImpl&& operator=(TaskImpl&& task){
        prop = task.prop;
    };
    ~TaskImpl() {};
    static void set_default(const std::string& str_json) {
        json jsRead;
        try {
            jsRead = json::parse(str_json);
        }
        catch (json::parse_error& ex) {
            LOG_ERROR("parse json error! content:%s, program stop\n", str_json.c_str());
            exit(1);
        }
        set_default(jsRead);
    };
    static void set_default(const json& jsRead) {
        static int count = 0;
        LOG_DEBUG("start set default![%d]\n", ++count);
        // ASSIGNE_TO_STRU(jsRead, "tag", prop.tag);
        assign_to_obj(jsRead, "tag", d_prop.tag, d_prop.tag);
        assign_to_obj(jsRead, "speed", d_prop.speed, d_prop.speed);
        assign_to_obj(jsRead, "detect_mask", d_prop.detect_mask, d_prop.detect_mask);
        assign_to_obj(jsRead, "camera_mask", d_prop.camera_mask, d_prop.camera_mask);
        assign_to_obj(jsRead, "each_time_ms", d_prop.each_time_ms, d_prop.each_time_ms);
        assign_to_obj(jsRead, "foreach_time", d_prop.foreach_time, d_prop.foreach_time);
        assign_to_obj(jsRead, "tag", d_prop.tag, d_prop.tag);
        if (jsRead.contains("dms")) {
            try {
                auto& js = jsRead["dms"];
                assign_to_obj(js, "enable", d_prop.dms.enable, d_prop.dms.enable);
                assign_to_obj(js, "pic_dir_path", d_prop.dms.pic_dir_path, d_prop.dms.pic_dir_path);
            }
            catch(json::parse_error& ex){
                LOG_ERROR("parse value error! key:%s, program stop\n", "dms");
                exit(1);
            }
        }
        if (jsRead.contains("oms")) {
            try {
                auto& js = jsRead["oms"];
                assign_to_obj(js, "enable", d_prop.oms.enable, d_prop.oms.enable);
                assign_to_obj(js, "pic_dir_path", d_prop.oms.pic_dir_path, d_prop.oms.pic_dir_path);
            }
            catch(json::parse_error& ex){
                LOG_ERROR("parse value error! key:%s, program stop\n", "oms");
                exit(1);
            }
        }
        print_default_prop();
        
        LOG_DEBUG("end set default![%d]\n", count);
    }
    static void print_prop(const TaskProp& prop) {
        LOG_DEBUG("prop.tag:%s\n", prop.tag.c_str());
        LOG_DEBUG("prop.speed:%d\n", prop.speed);
        LOG_DEBUG("prop.detect_mask:%s\n", prop.detect_mask.c_str());
        LOG_DEBUG("prop.camera_mask:%s\n", prop.camera_mask.c_str());
        LOG_DEBUG("prop.each_time_ms:%d\n", prop.each_time_ms);
        LOG_DEBUG("prop.foreach_time:%d\n", prop.foreach_time);
        LOG_DEBUG("prop.dms.enable:%d\n", prop.dms.enable);
        LOG_DEBUG("prop.dms.pic_dir_path:%s\n", prop.dms.pic_dir_path.c_str());
        LOG_DEBUG("prop.oms.enable:%d\n", prop.oms.enable);
        LOG_DEBUG("prop.oms.pic_dir_path:%s\n", prop.oms.pic_dir_path.c_str());
    };
    static void print_default_prop(void) {
        print_prop(d_prop);
    };
    void print_cur_prop(void) {
        print_prop(prop);
    };
    TaskProp get(void) const{
        return prop;
    }
};

extern std::unique_ptr<std::vector<TaskImpl>> g_p_out_vec;

inline void LoadTaskTxt(const std::string& txtpath) {
    auto func_read_txt = [](const std::string& txtPath) -> std::string {
        std::ifstream inStmLicense(txtPath);
        std::string strTemp;
        std::string strOut;
        //inStmLicense >> strTemp;
        while (std::getline(inStmLicense, strTemp)) {
            strOut = strOut + strTemp;
        }
        inStmLicense.close();
        return strOut;
    };
    auto func_parse_json = [](const std::string& content)->std::unique_ptr<std::vector<TaskImpl>> {
        using json = nlohmann::json;
        json js_read = json::parse(content);
        std::string js_task_default = js_read["default"].dump();
        LOG_DEBUG("js_task_default:%s\n", js_task_default.c_str()); 
        // default_task_p = TaskImpl(js_task_default);
        TaskImpl::set_default(js_task_default);
        std::unique_ptr<std::vector<TaskImpl>> p_out_vec(new std::vector<TaskImpl>);
        for (auto& js_item : js_read["array"]) {
            TaskImpl task(js_item);
            p_out_vec->push_back(task);
        }
        int count = 0;
        for (auto& item : *p_out_vec) {
            LOG_DEBUG("[%d, %d]p_out_vec item:%s\n", ++count, p_out_vec->size(), item.to_json().dump(4).c_str());
        }
        return std::move(p_out_vec);
    };
    std::string txt_content = func_read_txt(txtpath);
    auto  p_task_vec = func_parse_json(txt_content);
    int count = 0;
    for (auto& item : *p_task_vec) {
        LOG_DEBUG("[%d, %d]p_task_vec item:%s\n", ++count, p_task_vec->size(), item.to_json().dump(4).c_str());
    };
    count = 0;
    g_p_out_vec = std::move(p_task_vec);
    for (auto& item : *g_p_out_vec) {
        LOG_DEBUG("[%d, %d]g_p_out_vec item:%s\n", ++count, g_p_out_vec->size(), item.to_json().dump(4).c_str());
    };
}









#endif