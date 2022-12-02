#ifndef _MY_G_BUFFER_HPP_
#define _MY_G_BUFFER_HPP_ 
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
#include "sync.hpp"

#include "sdk-pkg.hpp"


extern std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_dms;
extern std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_oms;
extern std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_dms_default;
extern std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_oms_default;

extern std::unique_ptr<std::vector<TaskImpl>> g_p_out_vec;
extern ASD_ALG_HANDLE g_handle;
extern std::unique_ptr<SyncBuffBase<G_SyncInfo>> g_p_sync_dms_default2;
extern std::unique_ptr<SyncBuffBase<G_SyncInfo>> g_p_sync_oms_default2;

#endif  



