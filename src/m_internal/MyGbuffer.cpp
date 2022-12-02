#include "MyGbuffer.hpp"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   GBuffer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_dms;
std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_oms;
std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_dms_default;
std::unique_ptr<SyncBuffBase<PicInfo>> g_p_sync_oms_default;

std::unique_ptr<std::vector<TaskImpl>> g_p_out_vec;
ASD_ALG_HANDLE g_handle = NULL;
 std::unique_ptr<SyncBuffBase<G_SyncInfo>> g_p_sync_dms_default2(new SyncBuffBase<G_SyncInfo>());
 std::unique_ptr<SyncBuffBase<G_SyncInfo>> g_p_sync_oms_default2(new SyncBuffBase<G_SyncInfo>());