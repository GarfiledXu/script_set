#include "MyProcess.hpp"
//#include "input.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_decode_img
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void SaveUYVY(unsigned char* buf, int width, int height,char* filename)
{
    FILE* pFile = fopen(filename, "wb");
    //fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    fwrite(buf, 1, width * height * 2, pFile);
    fclose(pFile);
}
void DecodeMp4PushFrame(SDK_MODULE enumSdkModule, const std::string& strVideoPath, G_SyncInfo& L_SyncInfo, void* pmySdk) {
      LOG_INFO("start init ffmpeg Ctx:%s\n", strVideoPath.c_str());
      int ret = -1;
      AVFormatContext* pFormatCtx = NULL;
      AVCodec* pCodec = NULL;
      AVCodecContext* pCodecCtx = NULL;
      AVFrame* pAvFrame = NULL;
      AVFrame* pAvFrame_UYVY = NULL;

      av_register_all();
      avcodec_register_all();
      pFormatCtx = avformat_alloc_context();
      pAvFrame = av_frame_alloc();
      pAvFrame_UYVY = av_frame_alloc();
      ret = avformat_open_input(&pFormatCtx, strVideoPath.c_str(), NULL, NULL);
      if (0 != ret) {
            LOG_ERROR("avformat_open_input fail! return %d\n", ret);
            exit(1);
      }
      else {
            LOG_INFO("avformat_open_input success! return %d\n",ret);
      }
      ret = avformat_find_stream_info(pFormatCtx, NULL);//4.获取视频流信息
      if (ret < 0) {
            LOG_ERROR("avformat_find_stream_info fail! return %d\n", ret);
            exit(1);
      }else{
            LOG_INFO("avformat_find_stream_info success! return %d\n",ret);
      }
      int idxVideoStream = -1;//5.遍历获取视频流索引
      for (int i = 0;i < pFormatCtx->nb_streams;i++) {
            if (AVMEDIA_TYPE_VIDEO == pFormatCtx->streams[i]->codecpar->codec_type){
                  idxVideoStream = i;
                  break;
            }
      }
      if (-1 == idxVideoStream) {
            LOG_ERROR("can't find video stream!\n");
            exit(1);
      }
      pCodecCtx = pFormatCtx->streams[idxVideoStream]->codec;//6.获取编解码上下文，确定编码方式，确定解码器
      pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
      if (NULL == pCodec) {
            LOG_ERROR("can't find corresponding decoder!\n");
            exit(1);
      }
      ret = avcodec_open2(pCodecCtx, pCodec, NULL);//7.open decode
      if (ret < 0) {
            LOG_ERROR("avcodec_open2 fail! return %d\n", ret);
            exit(1);
      }
      else {
            LOG_INFO("avcodec_open2 success! return %d\n",ret);
      }
      int sizeFrame_UYVY = avpicture_get_size(AV_PIX_FMT_UYVY422, pCodecCtx->width, pCodecCtx->height);
      uint8_t* buffFrame_UYVY = (uint8_t*)av_malloc(sizeFrame_UYVY);
      avpicture_fill((AVPicture*)pAvFrame_UYVY, buffFrame_UYVY, AV_PIX_FMT_UYVY422, pCodecCtx->width, pCodecCtx->height);

      AVPacket* pPacket = (AVPacket*)malloc(sizeof(AVPacket));
      if (NULL == pPacket) {
            LOG_ERROR("av_packet_alloc() fail!\n");
            exit(1);
      }
      //av_dump_format(pFormatCtx, 0, pathVideo.c_str(), 0);//打印信息
      struct SwsContext* pCntCtx;
      pCntCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_UYVY422, SWS_BICUBIC, NULL, NULL, NULL);
      int got_picture = 0;
      int idxFrame = -1;
      LOG_INFO("start decode frame!\n");
      while ((ret = av_read_frame(pFormatCtx, pPacket)) >= 0) {
            //long long startTime = mstime();
            idxFrame++;
            if (idxVideoStream == pPacket->stream_index) {
                  //LOG_DEBUG("pPacket->stream_index = %d\n", pPacket->stream_index);
                  ret = avcodec_decode_video2(pCodecCtx, pAvFrame, &got_picture, pPacket);
                  //LOG_DEBUG("avcodec_decode_video2() return:%d\n", ret);
                  if (ret < 0) {
                        LOG_ERROR("avcodec_decode_video2 error! return:%d\n", ret);
                        //return -1;
                        //continue;
                        exit(1);
                  }
                  if (0 != got_picture) {
                        ret = sws_scale(pCntCtx, pAvFrame->data, pAvFrame->linesize, 0, pCodecCtx->height, pAvFrame_UYVY->data, pAvFrame_UYVY->linesize);
                        LOG_INFO("sws_scale ret = %d\n", ret);
                         // if (ret != 0) {
                        //       LOG_ERROR("sws_scale error, ret = %d\n", ret);
                        //       exit(1);
                        // }
                        ASVLOFFSCREEN tempArcImg;
                        tempArcImg.i32Height = pCodecCtx->height;
                        tempArcImg.i32Width = pCodecCtx->width;
                        tempArcImg.pi32Pitch[0] = pAvFrame_UYVY->linesize[0];
                        tempArcImg.ppu8Plane[0] = pAvFrame_UYVY->data[0];
                        tempArcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
                        
                        if (SDK_MODULE_DMS == enumSdkModule) {
///////////////////////////////////////////////////////
///               decode mp4 push dms
///////////////////////////////////////////////////////
                              MySdk* pMySdk = (MySdk*)reinterpret_cast<MySdk*>(pmySdk);
                              pMySdk->PushAdapt(enumSdkModule, &tempArcImg);
                              //pMySdk->PushDmsFrame(&tempArcImg);
                              //ARC_PutDmsData(&tempArcImg);
                              //reinterpret_cast<MySdk*>(pmySdk)->PushDmsFrame(&tempArcImg);
                              //std::string strTemp = "./" + std::to_string(idxFrame)+".uyvy";
                              //SaveUYVY(pAvFrame_UYVY->data[0], pCodecCtx->width, pCodecCtx->height, const_cast<char*>(strTemp.c_str()));
                              pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);

                              SetGSync setGSync(g_Sync_Putdata2Callback_DMS);//need change
                              setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                              G_SyncMp4 L_SyncMp4;
                              L_SyncMp4.videoPath = L_SyncInfo.g_SyncMp4.videoPath;
                              L_SyncMp4.nbFrame = L_SyncInfo.g_SyncMp4.nbFrame;   
                              L_SyncMp4.idxFrame = idxFrame;
                              setGSync.Set(L_SyncMp4);
                              pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
                              usleep(1000);//usleep(l_syncUnitSource.unitSdkCfg.perFrameTimeUs);
                              //LOG_DEBUG("decoded dms mp4 frame[%d, %d]\n", idxFrame, L_SyncInfo.g_SyncMp4.nbFrame - 1);
                        }
                        else if (SDK_MODULE_OMS == enumSdkModule) {
///////////////////////////////////////////////////////
///               decode mp4 push oms
///////////////////////////////////////////////////////
                              MySdk* pMySdk = (MySdk*)reinterpret_cast<MySdk*>(pmySdk);
                              pMySdk->PushAdapt(enumSdkModule, &tempArcImg);
                              //pMySdk->PushOmsFrame(&tempArcImg);
                              //std::string strTemp = "./" + std::to_string(idxFrame)+".uyvy";
                              //SaveUYVY(pAvFrame_UYVY->data[0], pCodecCtx->width, pCodecCtx->height, const_cast<char*>(strTemp.c_str()));
                              //ARC_PutOmsData(&tempArcImg);
                              //reinterpret_cast<MySdk*>(pmySdk)->PushOmsFrame(&tempArcImg);
                              pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);

                              SetGSync setGSync(g_Sync_Putdata2Callback_OOMS);//need change
                              setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                              G_SyncMp4 L_SyncMp4;
                              L_SyncMp4.videoPath = L_SyncInfo.g_SyncMp4.videoPath;
                              L_SyncMp4.nbFrame = L_SyncInfo.g_SyncMp4.nbFrame;   
                              L_SyncMp4.idxFrame = idxFrame;
                              setGSync.Set(L_SyncMp4);
                              
                              pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);
                              usleep(1000);//usleep(l_syncUnitSource.unitSdkCfg.perFrameTimeUs);
                              //LOG_DEBUG("decoded oms mp4 frame[%d, %d]\n", idxFrame, L_SyncInfo.g_SyncMp4.nbFrame - 1);
                        }
                        
                  }
            }
            //long long endTime = mstime();
            //LOG_INFO("get all duration time:%lld\n", endTime - startTime);
            av_free_packet(pPacket);
      }
      LOG_INFO("end decode frame!\n");
      LOG_INFO("start uinit ffmpeg Ctx!\n");
      av_free(buffFrame_UYVY);
      avformat_close_input(&pFormatCtx);
      sws_freeContext(pCntCtx);
      avcodec_close(pCodecCtx);
    
      av_free(pFormatCtx);//不确定
      av_free(pAvFrame);
      av_free(pAvFrame_UYVY);
      LOG_INFO("end uinit ffmpeg Ctx!\n");
      return;
}
void DecodeJpgPushFrame(SDK_MODULE enumSdkModule, const std::string& strJpgPath, void* pmySdk) {
      if (SDK_MODULE_DMS == enumSdkModule) {
            //LOG_DEBUG("start decode dms jpg:%s\n", strJpgPath.c_str());
            MySdk* pMySdk = reinterpret_cast<MySdk*>(pmySdk);
            *(pMySdk->pDmsMatImg) = tv::imread(strJpgPath, 1);
            tv::cvtToFourcc(*(pMySdk->pDmsMatImg), *(pMySdk->pDmsFbImg), tv::PIX_FMT_UYVY422);
            *(pMySdk->pDmsArcImg) = tv::cvtToArc(*(pMySdk->pDmsFbImg));
            //ARC_PutDmsData(pMySdk->pDmsArcImg);
///////////////////////////////////////////////////////
///               decode jpg push dms
///////////////////////////////////////////////////////
            pMySdk->PushAdapt(enumSdkModule, pMySdk->pDmsArcImg);
            //pMySdk->PushDmsFrame(pMySdk->pDmsArcImg);
            //pMySdk->PushDmsFrame(pMySdk -> pDmsArcImg);
            //LOG_DEBUG("end decode dms jpg:%s\n", strJpgPath.c_str());
      }
      else if (SDK_MODULE_OMS == enumSdkModule) {
            LOG_INFO("\n");
            //LOG_DEBUG("start decode oms jpg:%s\n", strJpgPath.c_str());
            MySdk* pMySdk = reinterpret_cast<MySdk*>(pmySdk);
            LOG_INFO("%s\n", strJpgPath.c_str());
            *(pMySdk->pOmsMatImg) = tv::imread(strJpgPath, 1);
            LOG_INFO("\n");
            tv::cvtToFourcc(*(pMySdk->pOmsMatImg), *(pMySdk->pOmsFbImg), tv::PIX_FMT_UYVY422);
            LOG_INFO("\n");
            *(pMySdk->pOmsArcImg) = tv::cvtToArc(*(pMySdk->pOmsFbImg));
            LOG_INFO("\n");
            //ARC_PutOmsData(pMySdk->pOmsArcImg);
///////////////////////////////////////////////////////
///               decode jpg push oms
///////////////////////////////////////////////////////
            pMySdk->PushAdapt(enumSdkModule, pMySdk->pOmsArcImg);
            LOG_INFO("\n");
            //pMySdk->PushOmsFrame(pMySdk->pOmsArcImg);
            //pMySdk->PushOmsFrame(pMySdk -> pOmsArcImg);
            //LOG_DEBUG("end decode oms jpg:%s\n", strJpgPath.c_str());
      }
      else {
            LOG_ERROR("param enumSdkModule:%d error!\n", static_cast<int>(enumSdkModule));
            exit(1);
      }
}
long DecodeMp4ForFrameNb(const std::string& pathVideo) {
      int ret = -1;
      AVFormatContext* pFormatCtx = NULL;
      AVCodec* pCodec = NULL;
      AVCodecContext* pCodecCtx = NULL;
      AVFrame* pAvFrame = NULL;
      AVFrame* pAvFrame_UYVY = NULL;

      av_register_all();
      pFormatCtx = avformat_alloc_context();
      pAvFrame = av_frame_alloc();
      pAvFrame_UYVY = av_frame_alloc();
      
    
      ret = avformat_open_input(&pFormatCtx, pathVideo.c_str(), NULL, NULL);
      if (0 != ret) {
            LOG_ERROR("avformat_open_input fail! return %d\n", ret);
            exit(1);
      }
      else {
            //LOG_INFO("avformat_open_input success!! return %d\n",ret);
      }
      ret = avformat_find_stream_info(pFormatCtx, NULL);//4.获取视频流信息
      if (ret < 0) {
            LOG_ERROR("avformat_find_stream_info fail! return %d\n", ret);
            exit(1);
      }else{
            //LOG_INFO("avformat_find_stream_info success!! return %d\n",ret);
      }
      int idxVideoStream = -1;//5.遍历获取视频流索引
      for (int i = 0;i < pFormatCtx->nb_streams;i++) {
            if (AVMEDIA_TYPE_VIDEO == pFormatCtx->streams[i]->codecpar->codec_type){
                  idxVideoStream = i;
                  break;
            }
      }
      if (-1 == idxVideoStream) {
            LOG_ERROR("can't find video stream!\n");
            exit(1);
      }
      
      pCodecCtx = pFormatCtx->streams[idxVideoStream]->codec;//6.获取编解码上下文，确定编码方式，确定解码器
      pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
      if (NULL == pCodec) {
            LOG_ERROR("can't find corresponding decoder!\n");
            exit(1);
            //return ;
      }
      ret = avcodec_open2(pCodecCtx, pCodec, NULL);//7.open decode
      if (ret < 0) {
            LOG_ERROR("avcodec_open2 fail! return %d\n", ret);
            exit(1);//return ;
      }
      else {
            //LOG_INFO("avcodec_open2 success! return %d\n",ret);
      }
      //int sizeFrame_UYVY = avpicture_get_size(AV_PIX_FMT_UYVY422, pCodecCtx->width, pCodecCtx->height);
      //uint8_t* buffFrame_UYVY = (uint8_t*)av_malloc(sizeFrame_UYVY);
      //avpicture_fill((AVPicture*)pAvFrame_UYVY, buffFrame_UYVY, AV_PIX_FMT_UYVY422, pCodecCtx->width, pCodecCtx->height);

      AVPacket* pPacket = (AVPacket*)malloc(sizeof(AVPacket));
      if (NULL == pPacket) {
            LOG_ERROR("av_packet_alloc() fail!\n");
            exit(1);
      }
      //av_dump_format(pFormatCtx, 0, pathVideo.c_str(), 0);//打印信息
      //struct SwsContext* pCntCtx;
      //pCntCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_UYVY422, SWS_BICUBIC, NULL, NULL, NULL);
      int got_picture = 0;
      long idxFrame = 0;
      while ((ret = av_read_frame(pFormatCtx, pPacket)) >= 0) {
            idxFrame++;
            av_free_packet(pPacket);
      }
      //av_free(buffFrame_UYVY);
      //av_free(pFormatCtx);//不确定
      av_free(pAvFrame);
      av_free(pAvFrame_UYVY);
      
      avformat_close_input(&pFormatCtx);
      //sws_freeContext(pCntCtx);
      avcodec_close(pCodecCtx);
      
      if (pPacket != NULL) {
            free(pPacket);
            pPacket = NULL;
      }
      
      return idxFrame;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_thread_function
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* G_DmsImg_DataThread(void* pmySdk) {
      while (1) {
            //for conflict faceid
            pthread_mutex_lock(&mtx_dms_faceid_putdata);
            //
            //get local_sync
            pthread_mutex_lock(&mtx_main_dmsput_data);//need change
            G_SyncInfo L_SyncInfo = g_Sync_Main2Putdata_DMS;//need change
            pthread_mutex_unlock(&mtx_main_dmsput_data);

            //default
            if (RUN_MODULE_DEFAULT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("dmsImg dataThread play default!\n");
                  DecodeJpgPushFrame(SDK_MODULE_DMS, g_defaultImg_dms, pmySdk);
                  pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);
                  SetGSync setGSync(g_Sync_Putdata2Callback_DMS);//need change
                  setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
            }
            //img
            else if (RUN_MODULE_IMG == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("dmsImg dataThread play Imgpath:%s\n", L_SyncInfo.g_SyncImg.imgPath.c_str());
                  DecodeJpgPushFrame(SDK_MODULE_DMS, L_SyncInfo.g_SyncImg.imgPath, pmySdk);
                  //usleep(100000);
                  //keep state, time control by main function
                  pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);//need change
                  SetGSync setGSync(g_Sync_Putdata2Callback_DMS);//need change
                  setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  G_SyncImg L_SyncImg;
                  L_SyncImg.imgPath = L_SyncInfo.g_SyncImg.imgPath;
                  setGSync.Set(L_SyncImg);
                  pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
            }
            else {
                  EnumCvt enumCvt;
                  LOG_ERROR("in data thread, L_SyncInfo.RUN_MODULE:%s error! \n",enumCvt.Enum2String(L_SyncInfo.enumRunModule).c_str());
                  exit(1);
            }
            //for conflict faceid
            pthread_mutex_unlock(&mtx_dms_faceid_putdata);
      }
}
void* G_OmsImg_DataThread(void* pmySdk) {
      LOG_INFO("into G_OmsImg_DataThread\n");
      while (1) {
            //get local_sync
            LOG_INFO("into L_SyncInfo\n");
            pthread_mutex_lock(&mtx_main_oomsput_data);//need change
            G_SyncInfo L_SyncInfo = g_Sync_Main2Putdata_OOMS;//need change
            pthread_mutex_unlock(&mtx_main_oomsput_data);
            LOG_INFO("end L_SyncInfo\n");
            //default
            if (RUN_MODULE_DEFAULT == L_SyncInfo.enumRunModule) {
                  LOG_INFO("into RUN_MODULE_DEFAULT\n");
                  //LOG_DEBUG("dmsImg dataThread play default!\n");
                  DecodeJpgPushFrame(SDK_MODULE_OMS, g_defaultImg_ooms, pmySdk);
                  LOG_INFO("into RUN_MODULE_DEFAULT\n");
                  pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);
                  LOG_INFO("into RUN_MODULE_DEFAULT\n");
                  SetGSync setGSync(g_Sync_Putdata2Callback_OOMS);//need change
                  LOG_INFO("into RUN_MODULE_DEFAULT\n");
                  setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  LOG_INFO("into RUN_MODULE_DEFAULT\n");
                  pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);
            }
            //img
            else if (RUN_MODULE_IMG == L_SyncInfo.enumRunModule) {
                  LOG_INFO("into RUN_MODULE_IMG\n");
                  //LOG_DEBUG("omsImg dataThread play Imgpath:%s\n", L_SyncInfo.g_SyncImg.imgPath.c_str());
                  DecodeJpgPushFrame(SDK_MODULE_OMS, L_SyncInfo.g_SyncImg.imgPath, pmySdk);
                  //usleep(100000);
                  //keep state, time control by main function
                  pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);//need change
                  SetGSync setGSync(g_Sync_Putdata2Callback_OOMS);//need change
                  setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  G_SyncImg L_SyncImg;
                  L_SyncImg.imgPath = L_SyncInfo.g_SyncImg.imgPath;
                  setGSync.Set(L_SyncImg);
                  pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);
            }
            else {
                  EnumCvt enumCvt;
                  LOG_ERROR("in data thread, L_SyncInfo.RUN_MODULE:%s error! \n",enumCvt.Enum2String(L_SyncInfo.enumRunModule).c_str());
                  exit(1);
            }
      }
}
void* G_DmsMp4_DataThread(void* pmySdk){
      while (1) {
            //for conflict faceidK)
            pthread_mutex_lock(&mtx_dms_faceid_putdata);
            //get local_sync
            pthread_mutex_lock(&mtx_main_dmsput_data);//need change
            G_SyncInfo L_SyncInfo = g_Sync_Main2Putdata_DMS;//need change
            pthread_mutex_unlock(&mtx_main_dmsput_data);
            //default
            if (RUN_MODULE_DEFAULT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("dmsMp4 dataThread play default!\n");
                  //DecodeJpg(SDK_MODULE_DMS, g_defaultImg_dms, pmySdk);
                  pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);
                  SetGSync setGSync(g_Sync_Putdata2Callback_DMS);//need change
                  setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
            }
            //mp4
            else if (RUN_MODULE_MP4 == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("dmsMp4 dataThread play Mp4:%s\n", L_SyncInfo.g_SyncMp4.videoPath.c_str());
                  DecodeMp4PushFrame(SDK_MODULE_DMS, L_SyncInfo.g_SyncMp4.videoPath, L_SyncInfo, pmySdk);
                  //finish play to reset default
                  pthread_mutex_lock(&mtx_main_dmsput_data);
                  SetGSync setGSync(g_Sync_Main2Putdata_DMS);//need change
                  setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_MAIN_TO_DATAPUT);
                  pthread_mutex_unlock(&mtx_main_dmsput_data);
            }
            else {
                  EnumCvt enumCvt;
                  LOG_ERROR("in data thread, L_SyncInfo.RUN_MODULE:%s error! \n",enumCvt.Enum2String(L_SyncInfo.enumRunModule).c_str());
                  exit(1);
            }
            //for conflict faceid
            pthread_mutex_unlock(&mtx_dms_faceid_putdata);
      }
}
void* G_OmsMp4_DataThread(void* pmySdk) {
      while (1) {
            //get local_sync
            pthread_mutex_lock(&mtx_main_oomsput_data);//need change
            G_SyncInfo L_SyncInfo = g_Sync_Main2Putdata_OOMS;//need change
            pthread_mutex_unlock(&mtx_main_oomsput_data);
            //default
            if (RUN_MODULE_DEFAULT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("omsMp4 dataThread play default!\n");
                  //DecodeJpg(SDK_MODULE_OMS, g_defaultImg_ooms, pmySdk);//nedd change
                  pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);
                  SetGSync setGSync(g_Sync_Putdata2Callback_OOMS);//need change
                  setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);
            }
            //mp4
            else if (RUN_MODULE_MP4 == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("omsMp4 dataThread play Mp4:%s\n", L_SyncInfo.g_SyncMp4.videoPath.c_str());
                  DecodeMp4PushFrame(SDK_MODULE_OMS, L_SyncInfo.g_SyncMp4.videoPath, L_SyncInfo, pmySdk);
                  //finish play to reset default
                  pthread_mutex_lock(&mtx_main_oomsput_data);
                  SetGSync setGSync(g_Sync_Main2Putdata_OOMS);//need change
                  setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_MAIN_TO_DATAPUT);
                  pthread_mutex_unlock(&mtx_main_oomsput_data);
            }
            else {
                  EnumCvt enumCvt;
                  LOG_ERROR("in data thread, L_SyncInfo.RUN_MODULE:%s error! \n",enumCvt.Enum2String(L_SyncInfo.enumRunModule).c_str());
                  exit(1);
            }
      }
}
void* G_DmsUnit_DataThread(void* pmySdk){
      while (1) {
           //for conflict faceid
            pthread_mutex_lock(&mtx_dms_faceid_putdata);
            //get local_sync
            pthread_mutex_lock(&mtx_main_dmsput_data);//need change
            G_SyncInfo L_SyncInfo = g_Sync_Main2Putdata_DMS;//need change
            pthread_mutex_unlock(&mtx_main_dmsput_data);
            //default
            if (RUN_MODULE_DEFAULT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("dmsMp4 dataThread play default!\n");
                  DecodeJpgPushFrame(SDK_MODULE_DMS, g_defaultImg_dms, pmySdk);
                  pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);
                  SetGSync setGSync(g_Sync_Putdata2Callback_DMS);//need change
                  setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
            }
            //mp4
            else if (RUN_MODULE_UNIT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("dmsUnit dataThread play Unit:%s\n", L_SyncInfo.g_SyncUnit.videoPath.c_str());
                  //get vecImg
                  std::vector<std::string> vecImg;
                  GetFileListRecursion(L_SyncInfo.g_SyncUnit.videoPath, vecImg, { ".jpg" });
                  long idxFrame = -1;
                  long nbFrame = vecImg.size();
                  for (std::string tempImg : vecImg) {
                        idxFrame++;
                        DecodeJpgPushFrame(SDK_MODULE_DMS, tempImg, pmySdk);
                        pthread_mutex_lock(&mtx_dmsput_dms_callbackinfo);

                        SetGSync setGSync(g_Sync_Putdata2Callback_DMS);//need change
                        setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                        G_SyncUnit L_SyncUnit;
                        L_SyncUnit.videoPath = L_SyncInfo.g_SyncUnit.videoPath;
                        L_SyncUnit.nbFrame = nbFrame;   
                        L_SyncUnit.idxFrame = idxFrame;
                        L_SyncUnit.framePath = tempImg;
                        setGSync.Set(L_SyncUnit);

                        pthread_mutex_unlock(&mtx_dmsput_dms_callbackinfo);
                        usleep(1000);//usleep(l_syncUnitSource.unitSdkCfg.perFrameTimeUs);
                        //LOG_DEBUG("decoded dms mp4 frame[%d, %d]\n", idxFrame, vecImg.size());
                  }
                  //finish play to reset default
                  pthread_mutex_lock(&mtx_main_dmsput_data);
                  SetGSync setGSync(g_Sync_Main2Putdata_DMS);//need change
                  setGSync.Reset(SDK_MODULE_DMS, SYNC_TYPE_MAIN_TO_DATAPUT);
                  pthread_mutex_unlock(&mtx_main_dmsput_data);
            }
            else {
                  EnumCvt enumCvt;
                  LOG_ERROR("in data thread, L_SyncInfo.RUN_MODULE:%s error! \n",enumCvt.Enum2String(L_SyncInfo.enumRunModule).c_str());
                  exit(1);
            }
            //for conflict faceid
            pthread_mutex_unlock(&mtx_dms_faceid_putdata);
      }
}
void* G_OmsUnit_DataThread(void* pmySdk) {
      while (1) {
            
            //get local_sync
            pthread_mutex_lock(&mtx_main_oomsput_data);//need change
            G_SyncInfo L_SyncInfo = g_Sync_Main2Putdata_OOMS;//need change
            pthread_mutex_unlock(&mtx_main_oomsput_data);
            //default
            if (RUN_MODULE_DEFAULT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("omsMp4 dataThread play default!\n");
                  DecodeJpgPushFrame(SDK_MODULE_OMS, g_defaultImg_ooms, pmySdk);
                  pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);
                  SetGSync setGSync(g_Sync_Putdata2Callback_OOMS);//need change
                  setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                  pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);
            }
            //mp4
            else if (RUN_MODULE_UNIT == L_SyncInfo.enumRunModule) {
                  //LOG_DEBUG("omsUnit dataThread play Unit:%s\n", L_SyncInfo.g_SyncUnit.videoPath.c_str());
                  //get vecImg
                  std::vector<std::string> vecImg;
                  GetFileListRecursion(L_SyncInfo.g_SyncUnit.videoPath, vecImg, { ".jpg" });
                  long idxFrame = -1;
                  long nbFrame = vecImg.size();
                  for (std::string tempImg : vecImg) {
                        idxFrame++;
                        DecodeJpgPushFrame(SDK_MODULE_OMS, tempImg, pmySdk);//need change
                        pthread_mutex_lock(&mtx_oomsput_ooms_callbackinfo);

                        SetGSync setGSync(g_Sync_Putdata2Callback_OOMS);//need change
                        setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_DATAPUT_TO_CALLBACK);
                        G_SyncUnit L_SyncUnit;
                        L_SyncUnit.videoPath = L_SyncInfo.g_SyncUnit.videoPath;
                        L_SyncUnit.nbFrame = nbFrame;   
                        L_SyncUnit.idxFrame = idxFrame;
                        L_SyncUnit.framePath = tempImg;
                        setGSync.Set(L_SyncUnit);

                        pthread_mutex_unlock(&mtx_oomsput_ooms_callbackinfo);
                        usleep(1000);//usleep(l_syncUnitSource.unitSdkCfg.perFrameTimeUs);
                        //LOG_DEBUG("decoded oms mp4 frame[%d, %d]\n", idxFrame, vecImg.size());
                  }
                  //finish play to reset default
                  pthread_mutex_lock(&mtx_main_oomsput_data);
                  SetGSync setGSync(g_Sync_Main2Putdata_OOMS);//need change
                  setGSync.Reset(SDK_MODULE_OMS, SYNC_TYPE_MAIN_TO_DATAPUT);
                  pthread_mutex_unlock(&mtx_main_oomsput_data);
            }
            else {
                  EnumCvt enumCvt;
                  LOG_ERROR("in data thread, L_SyncInfo.RUN_MODULE:%s error! \n",enumCvt.Enum2String(L_SyncInfo.enumRunModule).c_str());
                  exit(1);
            }

      }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_thread_camera_pushFrame
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void* G_DmsCamera_DataThread(void* pmySdk){
      unsigned char* imgTempBuff = (unsigned char*)malloc(sizeof(unsigned char*) * DMS_BUFF_LENGTH);
      int iCount = 0;
      while (1) {
            iCount++;
            //LOG_DEBUG("icount：%d\n", iCount);
            //LOG_DEBUG("start dms cp image!\n");
            //copy image, need mutex
            pthread_mutex_lock(&mtx_camera_pushdata_dms);
            memcpy(imgTempBuff, G_CameraImgBuff_Dms, sizeof(unsigned char)*DMS_BUFF_LENGTH);
            pthread_mutex_unlock(&mtx_camera_pushdata_dms);

            //assemble dmsImg
            MySdk* pMySdk = (MySdk*)reinterpret_cast<MySdk*>(pmySdk);
            ASVLOFFSCREEN arcImg = { 0 };
            arcImg.i32Width = DMS_CAMERA_WIDTH;
            arcImg.i32Height = DMS_CAMERA_HEIGHT;
            arcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
            arcImg.pi32Pitch[0] = arcImg.i32Width * 2;//间距
            arcImg.ppu8Plane[0] = (MUInt8 *)(imgTempBuff);
            arcImg.ppu8Plane[1] = MNull;
            //LOG_DEBUG("end dms assemble dmsImg!\n");

            //LOG_DEBUG("start dms push frame!\n");
            //push frame
             if (FLAGS_dump_img) {
                  std::string strFileName = std::to_string(iCount) + "_dms.UYVY";
                  SaveUYVY(imgTempBuff, DMS_CAMERA_WIDTH, DMS_CAMERA_HEIGHT, (char*)const_cast<char*>(strFileName.c_str()) );
                  sleep(1);
            }
            //std::string strFileName = std::to_string(iCount) + ".UYVY";
            //SaveUYVY(imgTempBuff, DMS_CAMERA_WIDTH, DMS_CAMERA_HEIGHT, (char*)const_cast<char*>(strFileName.c_str()) );
            pMySdk->PushAdapt(SDK_MODULE_DMS, &arcImg);
            
            //LOG_DEBUG("end dms push frame!\n");
            //pMySdk->PushDmsFrame(&arcImg);
            //non sync
      }
      free(imgTempBuff);
}
void* G_OmsCamera_DataThread(void* pmySdk){
      
      unsigned char* imgTempBuff = (unsigned char*)malloc(sizeof(unsigned char*) * OMS_BUFF_LENGTH);
      //copy image, need mutex
      int iCount = 0;
      while (1) {
            iCount++;
            //LOG_DEBUG("icount：%d\n", iCount);
            //LOG_DEBUG("start oms cp image!\n");
            pthread_mutex_lock(&mtx_camera_pushdata_oms);
            memcpy(imgTempBuff, G_CameraImgBuff_Oms, sizeof(char)*OMS_BUFF_LENGTH);
            pthread_mutex_unlock(&mtx_camera_pushdata_oms);
            //LOG_DEBUG("end oms cp image!\n");
            //assemble omsImg
            //LOG_DEBUG("start oms assemble omsImg!\n");
            MySdk* pMySdk = (MySdk*)reinterpret_cast<MySdk*>(pmySdk);
            ASVLOFFSCREEN arcImg = { 0 };
            arcImg.i32Width = OMS_CAMERA_WIDTH;
            arcImg.i32Height = OMS_CAMERA_HEIGHT;
            arcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
            arcImg.pi32Pitch[0] = arcImg.i32Width * 2;//间距
            arcImg.ppu8Plane[0] = (MUInt8 *)(imgTempBuff);
            arcImg.ppu8Plane[1] = MNull;
            //LOG_DEBUG("end oms assemble omsImg!\n");
            //push frame
            //LOG_DEBUG("start oms push frame!\n");
            
            if (FLAGS_dump_img) {
                  std::string strFileName = std::to_string(iCount) + "_oms.UYVY";
                  SaveUYVY(imgTempBuff, OMS_CAMERA_WIDTH, OMS_CAMERA_HEIGHT, (char*)const_cast<char*>(strFileName.c_str()));
                  sleep(1);
            }
            //SaveUYVY(imgTempBuff, OMS_CAMERA_WIDTH, OMS_CAMERA_HEIGHT, (char*)const_cast<char*>(strFileName.c_str()) );
            pMySdk->PushAdapt(SDK_MODULE_OMS, &arcImg);
            //LOG_DEBUG("end oms push frame!\n");
      }
     free(imgTempBuff);       //pMySdk->PushOmsFrame(&arcImg);
      //non sync
}
void* G_DmsDumpImg(void* agv) {
       unsigned char* imgTempBuff = (unsigned char*)malloc(sizeof(unsigned char*) * DMS_BUFF_LENGTH);
      int iCount = 0;
      while (1) {
            iCount++;

            pthread_mutex_lock(&mtx_camera_pushdata_dms);
            memcpy(imgTempBuff, G_CameraImgBuff_Dms, sizeof(unsigned char)*DMS_BUFF_LENGTH);
            pthread_mutex_unlock(&mtx_camera_pushdata_dms);

            ASVLOFFSCREEN arcImg = { 0 };
            arcImg.i32Width = DMS_CAMERA_WIDTH;
            arcImg.i32Height = DMS_CAMERA_HEIGHT;
            arcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
            arcImg.pi32Pitch[0] = arcImg.i32Width * 2;//间距
            arcImg.ppu8Plane[0] = (MUInt8 *)(imgTempBuff);
            arcImg.ppu8Plane[1] = MNull;

             if (FLAGS_dump_img) {
                  std::string strFileName = std::to_string(iCount) + "_dms.UYVY";
                  SaveUYVY(imgTempBuff, DMS_CAMERA_WIDTH, DMS_CAMERA_HEIGHT, (char*)const_cast<char*>(strFileName.c_str()));
                  LOG_INFO("dump dms img:%s\n", strFileName.c_str());
                  sleep(1);
            }

      }
      free(imgTempBuff);
}
void* G_OmsDumpImg(void* agv) {
      unsigned char* imgTempBuff = (unsigned char*)malloc(sizeof(unsigned char*) * OMS_BUFF_LENGTH);
      //copy image, need mutex
      int iCount = 0;
      while (1) {
            iCount++;

            pthread_mutex_lock(&mtx_camera_pushdata_oms);
            memcpy(imgTempBuff, G_CameraImgBuff_Oms, sizeof(char)*OMS_BUFF_LENGTH);
            pthread_mutex_unlock(&mtx_camera_pushdata_oms);

            ASVLOFFSCREEN arcImg = { 0 };
            arcImg.i32Width = OMS_CAMERA_WIDTH;
            arcImg.i32Height = OMS_CAMERA_HEIGHT;
            arcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
            arcImg.pi32Pitch[0] = arcImg.i32Width * 2;//间距
            arcImg.ppu8Plane[0] = (MUInt8 *)(imgTempBuff);
            arcImg.ppu8Plane[1] = MNull;
    
            
            if (FLAGS_dump_img) {
                  std::string strFileName = std::to_string(iCount) + "_oms.UYVY";
                  SaveUYVY(imgTempBuff, OMS_CAMERA_WIDTH, OMS_CAMERA_HEIGHT, (char*)const_cast<char*>(strFileName.c_str()));
                  LOG_INFO("dump oms img:%s\n", strFileName.c_str());
                  sleep(1);
            }

      }
     free(imgTempBuff);     
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_thread_manage
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void G_CheckDataThreadCounter(DataThreadCounter& threadCounter) {
       int dmsRunTypeCounter = 0;
       int omsRunTypeCounter = 0;
      if (threadCounter.dmsImgCounter > 0)
            dmsRunTypeCounter++;
      if (threadCounter.dmsMp4Counter > 0)
            dmsRunTypeCounter++;
      if (threadCounter.dmsUnitCounter > 0)
            dmsRunTypeCounter++;
      if (threadCounter.omsImgCounter > 0)
            omsRunTypeCounter++;
      if (threadCounter.omsMp4Counter > 0)
            omsRunTypeCounter++;
      if (threadCounter.omsUnitCounter > 0)
            omsRunTypeCounter++;
      if (dmsRunTypeCounter > 1) {
            LOG_ERROR("dms run type counter > 1, error! nb = %d\n", dmsRunTypeCounter);
            exit(1);
      }
      if (omsRunTypeCounter > 1) {
            LOG_ERROR("oms run type counter > 1, error! nb = %d\n", omsRunTypeCounter);
            exit(1);
      }
      
}

void G_RiseDataPthread(RUN_MODULE enumRunModule, SDK_MODULE enumSdkModule, DataThreadCounter& threadCounter, MySdk& mySdk) {
       LOG_INFO("into G_RiseDataPthread\n");
      MySdk* pAlgPro = &mySdk;
      if (RUN_MODULE_MP4 == enumRunModule) {
            //SDK_MODULE_OMS
            if (SDK_MODULE_OMS == enumSdkModule) {
                  if (0 == threadCounter.omsMp4Counter) {
                        int ret = pthread_create(&(pAlgPro->thrOmsDataPut), nullptr, G_OmsMp4_DataThread, pAlgPro);
                        sleep(1);
                        if (0 == ret) {
                              LOG_INFO("G_OmsMp4_DataThread create success!\n");
                              threadCounter.omsMp4Counter++;
                              G_CheckDataThreadCounter(threadCounter);
                              return;
                        }
                        else {
                              LOG_ERROR("G_OmsMp4_DataThread create error! return %d\n", ret);
                              exit(1);
                        }
                  }else if (1 <= threadCounter.omsMp4Counter) {
                        //LOG_DEBUG("G_OmsMp4_DataThread already created!\n");
                        return;
                  }
            }
            //SDK_MODULE_DMS
            else if (SDK_MODULE_DMS == enumSdkModule) {
                  if (0 == threadCounter.dmsMp4Counter) {
                        int ret = pthread_create(&(pAlgPro->thrDmsDataPut), nullptr, G_DmsMp4_DataThread, pAlgPro);
                        sleep(1);
                        if (0 == ret) {
                              LOG_INFO("G_DmsMp4_DataThread create success!\n");
                              threadCounter.dmsMp4Counter++;
                              G_CheckDataThreadCounter(threadCounter);
                              return;
                        }
                        else {
                              LOG_ERROR("G_DmsMp4_DataThread create error! return %d\n", ret);
                              exit(1);
                        }
                  }else if (1 <= threadCounter.dmsMp4Counter) {
                        //LOG_DEBUG("G_DmsMp4_DataThread already created!\n");
                        return;
                  }
            }
            else {
                  LOG_ERROR("Get SDK_MODULE error!\n");
                  exit(1);
            }
      }
      else if (RUN_MODULE_IMG == enumRunModule) {
            //SDK_MODULE_OMS
            LOG_INFO("into G_RiseDataPthread RUN_MODULE_IMG\n");
            if (SDK_MODULE_OMS == enumSdkModule) {
                  LOG_INFO("into G_RiseDataPthread SDK_MODULE_OMS\n");
                  if (0 == threadCounter.omsImgCounter) {
                        int ret = pthread_create(&(pAlgPro->thrOmsDataPut), nullptr, G_OmsImg_DataThread, pAlgPro);
                        LOG_INFO("end G_RiseDataPthread SDK_MODULE_OMS\n");
                        sleep(1);
                        LOG_INFO("end G_RiseDataPthread SDK_MODULE_OMS\n");
                        if (0 == ret) {
                              LOG_INFO("G_OmsImg_DataThread create success!\n");
                              threadCounter.omsImgCounter++;
                              G_CheckDataThreadCounter(threadCounter);
                              return;
                        }
                        else {
                              LOG_ERROR("G_OmsImg_DataThread create error! return %d\n", ret);
                              exit(1);
                        }
                        LOG_INFO("end G_RiseDataPthread SDK_MODULE_OMS\n");
                  }
                  else if (1 <= threadCounter.omsImgCounter) {
                        LOG_INFO("end G_RiseDataPthread SDK_MODULE_OMS\n");
                        //LOG_DEBUG("G_OmsImg_DataThread already created!\n");
                        return;
                  }            
            }
            //SDK_MODULE_DMS
            else if (SDK_MODULE_DMS == enumSdkModule) {
                  LOG_INFO("into G_RiseDataPthread SDK_MODULE_DMS\n");
                  if (0 == threadCounter.dmsImgCounter) {
                        int ret = pthread_create(&(pAlgPro->thrDmsDataPut), nullptr, G_DmsImg_DataThread, pAlgPro);
                        sleep(1);
                        if (0 == ret) {
                              LOG_INFO("G_DmsImg_DataThread create success!\n");
                              threadCounter.dmsImgCounter++;
                              G_CheckDataThreadCounter(threadCounter);
                              return;
                        }
                        else {
                              LOG_ERROR("G_DmsImg_DataThread create error! return %d\n", ret);
                              exit(1);
                        }
                  }else if (1 <= threadCounter.dmsImgCounter) {
                        //LOG_DEBUG("G_DmsImg_DataThread already created!\n");
                        return;
                  }   
            }
            else {
                  
            }
      }
      else if (RUN_MODULE_UNIT == enumRunModule) {
            //SDK_MODULE_OMS
            if (SDK_MODULE_OMS == enumSdkModule) {
                  if (0 == threadCounter.omsUnitCounter) {
                        int ret = pthread_create(&(pAlgPro->thrOmsDataPut), nullptr, G_OmsUnit_DataThread, pAlgPro);
                        sleep(1);
                        if (0 == ret) {
                              LOG_INFO("G_OmsUnit_DataThread create success!\n");
                              threadCounter.omsUnitCounter++;
                              G_CheckDataThreadCounter(threadCounter);
                              return;
                        }
                        else {
                              LOG_ERROR("G_OmsUnit_DataThread create error! return %d\n", ret);
                              exit(1);
                        }
                  }else if (1 <= threadCounter.omsUnitCounter) {
                        //LOG_DEBUG("G_OmsUnit_DataThread already created!\n");
                        return;
                  }   
            }
            //SDK_MODULE_DMS
            else if (SDK_MODULE_DMS == enumSdkModule) {
                  if (0 == threadCounter.dmsUnitCounter) {
                        int ret = pthread_create(&(pAlgPro->thrDmsDataPut), nullptr, G_DmsUnit_DataThread, pAlgPro);
                        sleep(1);
                        if (0 == ret) {
                              LOG_INFO("G_DmsUnit_DataThread create success!\n");
                              threadCounter.dmsUnitCounter++;
                              G_CheckDataThreadCounter(threadCounter);
                              return;
                        }
                        else {
                              LOG_ERROR("G_DmsUnit_DataThread create error! return %d\n", ret);
                              exit(1);
                        }
                  }else if (1 <= threadCounter.dmsUnitCounter) {
                        //LOG_DEBUG("G_DmsUnit_DataThread already created!\n");
                        return;
                  }   
            }
      }
      else {
            LOG_ERROR("Get RUN_MODULE error!\n");
            exit(1);
      }
}
void G_ReleaseDataPthread(DataThreadCounter& threadCounter, MySdk& algPro) {
      //LOG_DEBUG("threadCounter.dmsImgCounter:%d\n", threadCounter.dmsImgCounter);
      if (1 == threadCounter.dmsImgCounter) {
            LOG_INFO("Start cancel Dms pthread!\n");
            int ret, ret2;
            ret = pthread_cancel(algPro.thrDmsDataPut);
            //LOG_DEBUG("\n");
            if (ret != 0) {
                  LOG_ERROR("pthread_cancel dms, false\n", ret, ret2);
                  exit(1);
             }
            ret2 = pthread_join(algPro.thrDmsDataPut, nullptr);
            //LOG_DEBUG("\n");
            if (ret2 != 0) {
                  LOG_ERROR("pthread_join dms, false\n", ret, ret2);
                  exit(1);
             }
            LOG_INFO("End cancel Dms pthread!\n");
            threadCounter.dmsImgCounter = 0;
            G_CheckDataThreadCounter(threadCounter);
      }
      else if (1 == threadCounter.dmsMp4Counter) {
            LOG_INFO("Start cancel Dms pthread!\n");
            pthread_cancel(algPro.thrDmsDataPut);
            pthread_join(algPro.thrDmsDataPut, nullptr);
            LOG_INFO("End cancel Dms pthread!\n");
            threadCounter.dmsMp4Counter = 0;
            G_CheckDataThreadCounter(threadCounter);
      }
      else if (1 == threadCounter.dmsUnitCounter){
            LOG_INFO("Start cancel Dms pthread!\n");
            pthread_cancel(algPro.thrDmsDataPut);
            pthread_join(algPro.thrDmsDataPut, nullptr);
            LOG_INFO("End cancel Dms pthread!\n");
            threadCounter.dmsUnitCounter = 0;
            G_CheckDataThreadCounter(threadCounter);
      }
      else if (1 == threadCounter.omsImgCounter) {
            LOG_INFO("Start cancel Oms pthread!\n");
            pthread_cancel(algPro.thrOmsDataPut);
            pthread_join(algPro.thrOmsDataPut, nullptr);
            LOG_INFO("End cancel Dms pthread!\n");
            threadCounter.omsImgCounter = 0;
            G_CheckDataThreadCounter(threadCounter);
      }
      else if (1 == threadCounter.omsMp4Counter) {
            LOG_INFO("Start cancel Oms pthread!\n");
            pthread_cancel(algPro.thrOmsDataPut);
            pthread_join(algPro.thrOmsDataPut, nullptr);
            LOG_INFO("End cancel Dms pthread!\n");
            threadCounter.omsMp4Counter = 0;
            G_CheckDataThreadCounter(threadCounter);
      }
      else if (1 == threadCounter.omsUnitCounter) {
            LOG_INFO("Start cancel Oms pthread!\n");
            pthread_cancel(algPro.thrOmsDataPut);
            pthread_join(algPro.thrOmsDataPut, nullptr);
            LOG_INFO("End cancel Dms pthread!\n");
            threadCounter.omsUnitCounter = 0;
            G_CheckDataThreadCounter(threadCounter);
      }
      
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_test_faceid
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
TestFaceid::TestFaceid() {}
TestFaceid::~TestFaceid() {}
void TestFaceid::Run(const std::string& strModule, const std::string& strFaceidOpt, const std::string& strPath) {
//////////////////////////////////////////////////////
////                   sdk-init
//////////////////////////////////////////////////////
      MySdk mySdk;
      //init param
      ASD_INIT_PARAM initParam;
      initParam.camMask = ASD_CAM_MASK_ALL;
      initParam.dms_CamAngle = 0;
      //char strTemp[100] = "./arc_cfg.json";
      //initParam.cfgFile = strTemp;
      //init
      initParam.JsonPath="./arc_cfg.json";
      mySdk.Init(&initParam, CB_JSON);
//////////////////////////////////////////////////////
////                   sdk-set
//////////////////////////////////////////////////////
      mySdk.AllSet();
      //set
      //mySdk.AllSet(MYSDK_SET_MASK_SIGNAL, ASD_VEHICLE_SIGNAL_SPEED, 40, 0);
//////////////////////////////////////////////////////
////                   get-img-vector
//////////////////////////////////////////////////////
      std::vector<std::string> vecImg;
      GetFileListRecursion(strPath, vecImg, { ".jpg" });
      if(0 == vecImg.size()){
            LOG_ERROR("can't find any jpg, by dirpath:%s, vecImg.size() = %d\n", strPath.c_str(), vecImg.size());
            exit(1);
      }
//////////////////////////////////////////////////////
////                   strmodule-cvt-enum
//////////////////////////////////////////////////////
      SDK_MODULE temEnumSdkModule;
      if (0 == strcmp(strModule.c_str(), "dms")) 
            temEnumSdkModule = SDK_MODULE_DMS;
      else if (0 == strcmp(strModule.c_str(), "oms")) 
            temEnumSdkModule = SDK_MODULE_OMS;
//////////////////////////////////////////////////////
////                   check-flags
//////////////////////////////////////////////////////
      CheckStrFlags("-faceid_opt", FLAGS_faceid_opt.c_str(), { "register_recognition_match" }, true);
//////////////////////////////////////////////////////
////                   select-module
//////////////////////////////////////////////////////
      if (0 == strcmp(FLAGS_faceid_opt.c_str(), "register_recognition_match")) {
            this->Register_Recog_Match(temEnumSdkModule, strPath + "/zhuce", strPath + "/shibie", mySdk);
      }
//////////////////////////////////////////////////////
////                   sdk-uninit
//////////////////////////////////////////////////////
      G_ReleaseDataPthread(G_threadCounter, mySdk);
      mySdk.Uninit();
}
void TestFaceid::Register_Recog_Match(SDK_MODULE enumInput, const std::string& pathRegister, const std::string& pathRecognition, MySdk& mySdk) {
      LOG_INFO("start Register_Recog_Match!\n");
      //get img vector
      std::vector<std::string> vecRegisterImg, vecRecogImg;
      GetFileListRecursion(pathRegister, vecRegisterImg, { ".jpg" });
      if(0 == vecRegisterImg.size()){
            LOG_ERROR("can't find any jpg, by dirpath:%s, vecRegisterImg.size() = %d\n", pathRegister.c_str(), vecRegisterImg.size());
            exit(1);
      }
      GetFileListRecursion(pathRecognition, vecRecogImg, { ".jpg" });
      if(0 == vecRecogImg.size()){
            LOG_ERROR("can't find any jpg, by dirpath:%s, vecRecogImg.size() = %d\n", pathRecognition.c_str(), vecRecogImg.size());
            exit(1);
      }
     
      int idxRegisterImg = -1;
      int idxRecognitionImg = -1;
      tv::Mat* pMatImg = new tv::Mat;
      tv::FourccBuffer* pFbImg = new tv::FourccBuffer;
      ASVLOFFSCREEN* pArcImg = new ASVLOFFSCREEN;
      system("rm -rf ./FACEID_RESULT");
      system("mkdir ./FACEID_RESULT");
      ASD_CAM_MASK camMask;
      if (SDK_MODULE_DMS == enumInput) {
            camMask = ASD_CAM_MASK_DMS;
      }
      else if (SDK_MODULE_OMS == enumInput) {
            camMask = ASD_CAM_MASK_OMS;
      }
      for(std::string imgPath : vecRegisterImg){
            idxRegisterImg++;
            CPathProcess pathPro;
            pathPro.GetFileStr(imgPath);
            std::string pathSave = "./FACEID_RESULT/" + pathPro.nosuffixname + ".txt";
            char bufRegister[1000] = { 0 };

            *(pMatImg) = tv::imread(imgPath, 1);
            tv::cvtToFourcc(*pMatImg, *pFbImg, tv::PIX_FMT_UYVY422);
            *(pArcImg) = tv::cvtToArc(*pFbImg);
            //int ret = ASD_FaceRegister(pAlgHandle, pArcImg, camType, bufRegister);
            int ret = mySdk.FaceRegister(pArcImg, camMask, bufRegister);
            if (0 != ret) {
                  LOG_ERROR("ASD_FaceRegister error! ret = %d, position:[%d,%d], path:%s\n", ret, idxRegisterImg, vecRegisterImg.size()-1, imgPath.c_str());
                  //exit(1);
            }
            LOG_INFO("register position:[%d,%d], path:%s\n",idxRegisterImg, vecRegisterImg.size()-1, imgPath.c_str());
            int idxRecognition = -1;
            for (std::string recogImgPath : vecRecogImg) {
                  idxRecognition++;
                  char bufRecognition[1000] = {0};
                  *(pMatImg) = tv::imread(recogImgPath, 1);
                  tv::cvtToFourcc(*pMatImg, *pFbImg, tv::PIX_FMT_UYVY422);
                  *(pArcImg) = tv::cvtToArc(*pFbImg);
                  int ret2 = mySdk.FaceRecognition(pArcImg, camMask, bufRecognition);
                  //int ret2 = ASD_FaceRecognition(pAlgHandle, pArcImg, camType, bufRecognition);
                  if (0 != ret2) {
                  LOG_ERROR("ASD_FaceRecognition error! ret = %d, position:[%d,%d], path:%s\n", ret2, idxRecognition, vecRecogImg.size()-1, recogImgPath.c_str());
                  //exit(1);
                  }
                  LOG_INFO("recognition position:[%d,%d], path:%s\n",idxRecognition, vecRecogImg.size()-1, recogImgPath.c_str());
                  //start match
                  double similarity = 0.0;
                  int ret3 = mySdk.FaceMatch(bufRegister, bufRecognition, &similarity);
                  //int ret3 = ASD_FaceMatch(pAlgHandle, bufRegister, bufRecognition, &similarity);
                  if (0 != ret3) {
                  LOG_ERROR("ASD_FaceMatch error! ret = %d position:[%d,%d], path:%s\n", ret3, idxRecognition, vecRecogImg.size()-1,recogImgPath.c_str());
                  //exit(1);
                  }
                  
                  LOG_INFO("similarity:%lf,match position:[%d,%d], path:%s\n", similarity, idxRecognition, vecRecogImg.size() - 1, recogImgPath.c_str());
                  //start write result
                  std::string strRet = "register_imgpath:" + imgPath + ", recognition_imgpath:" + recogImgPath + ", [similarity_score: " + std::to_string(similarity)+" ]";
                  std::ofstream ostmTxt(pathSave, std::ios::app);
                  ostmTxt << strRet << std::endl;
                  LOG_INFO("ret:%s\n",strRet.c_str());
                  ostmTxt.close();
            }
      }
      //release
      delete pMatImg;
      delete pFbImg;
      delete pArcImg;

      LOG_INFO("end Register_Recog_Match!\n");
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_run_module
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
RunModule::RunModule() {}
RunModule::~RunModule() {}
void RunModule::RunUnit() {}
void RunModule::Run(const std::string& strModule, const std::string& strType, const std::string& strPath) {
//////////////////////////////////////////////////////
////                   sdk-init
//////////////////////////////////////////////////////
      //LOG_DEBUG("start init sdk\n");
      MySdk mySdk;
      //init param
      ASD_INIT_PARAM initParam;
      initParam.camMask = ASD_CAM_MASK_ALL;
      initParam.dms_CamAngle = 0;
      // char strTemp[100] = "./arc_cfg.json";
      // initParam.cfgFile = strTemp;
      //init
      initParam.JsonPath="./arc_cfg.json";
      mySdk.Init(&initParam, CB_JSON);
      //LOG_DEBUG("end init sdk\n");
      //set 
      //mySdk.AllSet(MYSDK_SET_MASK_SIGNAL, ASD_VEHICLE_SIGNAL_SPEED, 40, 0);
//////////////////////////////////////////////////////
////                   sdk-set-param
//////////////////////////////////////////////////////
      mySdk.AllSet();
//////////////////////////////////////////////////////
////                   strmodule-cvt-enum
//////////////////////////////////////////////////////
      SDK_MODULE temEnumSdkModule;
      if (0 == strcmp(strModule.c_str(), "dms")) 
            temEnumSdkModule = SDK_MODULE_DMS;
      else if (0 == strcmp(strModule.c_str(), "oms")) 
            temEnumSdkModule = SDK_MODULE_OMS;
//////////////////////////////////////////////////////
////                   get-img-vector
//////////////////////////////////////////////////////
      //LOG_DEBUG("start get-img-vector\n");
      std::vector<std::string> vecMetiral;
      if (0 == strcmp(FLAGS_type.c_str(), "img")) {
            GetFileListRecursion(strPath, vecMetiral, { ".jpg" });
            if(0 == vecMetiral.size()){
                  LOG_ERROR("can't find any jpg, by dirpath:%s, vecMetiral.size() = %d\n", strPath.c_str(), vecMetiral.size());
                  exit(1);
            }
      }
      else if (0 == strcmp(FLAGS_type.c_str(), "video")) {
            GetFileListRecursion(strPath, vecMetiral, { ".mp4" });
            if(0 == vecMetiral.size()){
                  LOG_ERROR("can't find any mp4, by dirpath:%s, vecMetiral.size() = %d\n", strPath.c_str(), vecMetiral.size());
                  exit(1);
            }
      }
      else if(0 == strcmp(FLAGS_type.c_str(), "unit")){
            /*NO*/
      }
      //LOG_DEBUG("end get-img-vector\n");
//////////////////////////////////////////////////////
////                   check-flags
//////////////////////////////////////////////////////
      CheckStrFlags("-module", FLAGS_type.c_str(), { "img","video", "unit" }, true);
//////////////////////////////////////////////////////
////                   select-module
//////////////////////////////////////////////////////
      if (0 == strcmp(FLAGS_type.c_str(), "img")) {
            //LOG_DEBUG("start run-img\n");
            for (std::string strTemp : vecMetiral) {
                  this->RunImg(temEnumSdkModule, strTemp, mySdk);
            }
      }
      else if (0 == strcmp(FLAGS_type.c_str(), "video")) {
            //LOG_DEBUG("start run-video\n");
            for (std::string strTemp : vecMetiral) {
                  this->RunVideo(temEnumSdkModule, strTemp, mySdk);
            }
      }
      else if (0 == strcmp(FLAGS_type.c_str(), "unit")) {
            /*NO*/
      }
//////////////////////////////////////////////////////
////                   sdk-uninit
//////////////////////////////////////////////////////
      G_ReleaseDataPthread(G_threadCounter, mySdk);
      mySdk.Uninit();
}
void RunModule::RunImg(SDK_MODULE enumSdkModule, const std::string& strPath, MySdk& mySdk) {
      LOG_INFO("start run img:%s\n", strPath.c_str());
      //rise thread
      G_RiseDataPthread(RUN_MODULE_IMG, enumSdkModule, G_threadCounter, mySdk);
      LOG_INFO("end risedata\n");
      //set sdk
      //mySdk.AllSet(MYSDK_SET_MASK_SIGNAL, ASD_VEHICLE_SIGNAL_SPEED, 40, 0);
      //select dms/oms
      if (SDK_MODULE_DMS == enumSdkModule) {
            //set local
            G_SyncImg l_SyncImg;
            l_SyncImg.imgPath = strPath;
            //start init setclass
            SetGSync setMain2Putdata(g_Sync_Main2Putdata_DMS);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            //start run
            pthread_mutex_lock(&mtx_main_dmsput_data);
            setMain2Putdata.Set(l_SyncImg);
            pthread_mutex_unlock(&mtx_main_dmsput_data);
            //sleep
            sleep(4);
            //start reset
            pthread_mutex_lock(&mtx_main_dmsput_data);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            pthread_mutex_unlock(&mtx_main_dmsput_data);
      }
      else if (SDK_MODULE_OMS == enumSdkModule) {
//////////////////////////////////////////////////////
////                   Oms_Detect_Switch_Set
//////////////////////////////////////////////////////
            //set local
            G_SyncImg l_SyncImg;
            l_SyncImg.imgPath = strPath;
            //start init setclass
            SetGSync setMain2Putdata(g_Sync_Main2Putdata_OOMS);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            //start run
            pthread_mutex_lock(&mtx_main_oomsput_data);
            setMain2Putdata.Set(l_SyncImg);
            pthread_mutex_unlock(&mtx_main_oomsput_data);
            //sleep
            sleep(4);
            //start reset
            pthread_mutex_lock(&mtx_main_oomsput_data);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            pthread_mutex_unlock(&mtx_main_oomsput_data);
      }
      LOG_INFO("end run img:%s\n", strPath.c_str());
}
void RunModule::RunVideo(SDK_MODULE enumSdkModule, const std::string& strPath, MySdk& mySdk) {
      LOG_INFO("start run video:%s\n", strPath.c_str());
      //rise thread
      G_RiseDataPthread(RUN_MODULE_MP4, enumSdkModule, G_threadCounter, mySdk);
      //set sdk
      //mySdk.AllSet(MYSDK_SET_MASK_SIGNAL, ASD_VEHICLE_SIGNAL_SPEED, 40, 0);
      //select dms/oms
      if (SDK_MODULE_DMS == enumSdkModule) {
            long nbFrame = DecodeMp4ForFrameNb(strPath);
            //set local
            G_SyncMp4 l_SyncMp4;
            l_SyncMp4.nbFrame = nbFrame;
            l_SyncMp4.videoPath = strPath;
            //start init setclass
            SetGSync setMain2Putdata(g_Sync_Main2Putdata_DMS);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            //start run
            pthread_mutex_lock(&mtx_main_dmsput_data);
            setMain2Putdata.Set(l_SyncMp4);
            pthread_mutex_unlock(&mtx_main_dmsput_data);
            //sleep
            sleep(nbFrame * 0.1);
            //start reset
            pthread_mutex_lock(&mtx_main_dmsput_data);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            pthread_mutex_unlock(&mtx_main_dmsput_data);
      }
      else if (SDK_MODULE_OMS == enumSdkModule) {
//////////////////////////////////////////////////////
////                   Oms_Detect_Switch_Set
/////////////////////////////////////////////////////
            long nbFrame = DecodeMp4ForFrameNb(strPath);
            //set local
            G_SyncMp4 l_SyncMp4;
            l_SyncMp4.nbFrame = nbFrame;
            l_SyncMp4.videoPath = strPath;
            //start init setclass
            SetGSync setMain2Putdata(g_Sync_Main2Putdata_OOMS);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            //start run
            pthread_mutex_lock(&mtx_main_oomsput_data);
            setMain2Putdata.Set(l_SyncMp4);
            pthread_mutex_unlock(&mtx_main_oomsput_data);
            //sleep
            sleep(nbFrame * 0.1);
            //start reset
            pthread_mutex_lock(&mtx_main_oomsput_data);
            setMain2Putdata.Reset(enumSdkModule, SYNC_TYPE_MAIN_TO_DATAPUT);
            pthread_mutex_unlock(&mtx_main_oomsput_data);
      }
      LOG_INFO("end run video:%s\n", strPath.c_str());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////                   G_run_camera
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TestCamera::TestCamera() {}
// TestCamera::~TestCamera(){}
// void TestCamera::Run(const std::string& strModule, const std::string& strCameraOpt) {
// //////////////////////////////////////////////////////
// ////                   string-cvt-mask
// //////////////////////////////////////////////////////
//       MYSDK_SET_MASK mySdkMask = 0;
//       if (0 == strcmp(strModule.c_str(), "dms")) {
//             mySdkMask |= MYSDK_SET_MASK_DMS_DETECT_SWITCH;
//             LOG_DEBUG("sdk mask :%d\n", mySdkMask);
//       }
//       if (0 == strcmp(strModule.c_str(), "oms")) {
//             mySdkMask |= MYSDK_SET_MASK_OMS_DETECT_SWITCH;
//             LOG_DEBUG("sdk mask :%d\n", mySdkMask);
//       }
//       if (0 == strcmp(strModule.c_str(), "dms_oms")) {
//             mySdkMask |= MYSDK_SET_MASK_DMS_DETECT_SWITCH;
//             mySdkMask |= MYSDK_SET_MASK_OMS_DETECT_SWITCH;
//             LOG_DEBUG("sdk mask :%d\n", mySdkMask);
//       }
//       LOG_DEBUG("sdk mask :%d\n", mySdkMask);
// //////////////////////////////////////////////////////
// ////                   set-speed
// //////////////////////////////////////////////////////     
      
// //////////////////////////////////////////////////////
// ////                   sdk-init
// //////////////////////////////////////////////////////
//       MySdk mySdk;
//       ASD_INIT_PARAM initParam;
//       initParam.camMask = ASD_CAM_MASK_ALL;
//       initParam.dms_CamAngle = 0;
//       // char strTemp[100] = "./arc_cfg.json";
//       // initParam.cfgFile = strTemp;
//       //init
//       initParam.JsonPath="./arc_cfg.json";
//       mySdk.Init(&initParam, CB_JSON);
// //////////////////////////////////////////////////////
// ////                   sdk-set-param
// //////////////////////////////////////////////////////
//       mySdk.AllSet();
//       //mySdk.Init(ASD_CAM_MASK_ALL, 0, CB_JSON);
// //////////////////////////////////////////////////////
// ////                   camera-init
// //////////////////////////////////////////////////////
//       this->QcameraInit(mySdkMask);
//      /* qcarcam_init();
//       pthread_t this->dmsCaptureId = 0, this->omsCaptureId = 0, this->cameraCheckId = 0;
//       pthread_create(&this->dmsCaptureId, NULL, start_dms_capture, NULL);
//       //pthread_create(&this->omsCaptureId, NULL, start_oms_capture, NULL);
//       pthread_create(&this->cameraCheckId, NULL, thread_camera_check, NULL);*/
// //////////////////////////////////////////////////////
// ////                   select-run-module
// ////                   -module oms_dms/oms/dms -type camera/img/video -camera_opt alarm/faceid_register_login_match
// //////////////////////////////////////////////////////
//       if (0 == strcmp(FLAGS_camera_opt.c_str(), "alarm")) {
//             DmsOmsAlarm(mySdkMask, mySdk);
//       }
//       else if (0 == strcmp(FLAGS_camera_opt.c_str(), "faceid_register_login_match")) {
//             FaceidRegisterLoginMatch(mySdkMask, mySdk);
//       }
//       else {
//             LOG_ERROR("camera opt error! %s\n", FLAGS_camera_opt.c_str());
//             exit(1);
//       }

// //////////////////////////////////////////////////////
// ////                   sdk-uninit
// //////////////////////////////////////////////////////
//       G_ReleaseDataPthread(G_threadCounter, mySdk);
//       mySdk.Uninit();
// //////////////////////////////////////////////////////
// ////                   camera-uninit
// //////////////////////////////////////////////////////
//       this->QcameraUnInit(mySdkMask);
//       /* if (this->cameraCheckId != 0) {
// 		stop_camera_check();
// 		pthread_join(this->cameraCheckId, NULL);
// 	}
//       if(this->dmsCaptureId != 0){
// 		stop_dms_capture();
// 		pthread_join(this->dmsCaptureId, NULL);
// 	}
//       // if(this->omsCaptureId != 0){
// 	// 	stop_dms_capture();
// 	// 	pthread_join(this->omsCaptureId, NULL);
// 	// }
//       qcarcam_uninit();*/
// }
// int TestCamera::QcameraInit(MYSDK_SET_MASK inputSdkMask) {
//       LOG_DEBUG("start qcamera init\n");
//       qcarcam_init();
//       LOG_DEBUG("end qcamera init\n");
//       LOG_DEBUG("start create capture and check thread!\n");
//       if ((inputSdkMask & MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH) {
//              pthread_create(&this->dmsCaptureId, NULL, start_dms_capture, NULL);
//       }
//       if ((inputSdkMask & MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) {
//             pthread_create(&this->omsCaptureId, NULL, start_oms_capture, NULL);
//       }
//       pthread_create(&this->cameraCheckId, NULL, thread_camera_check, NULL);
//       LOG_DEBUG("end create capture and check thread!\n");
//       return 0;
// }
// int TestCamera::QcameraUnInit(MYSDK_SET_MASK inputSdkMask) {
//       LOG_DEBUG("start qcamera uninit\n");
//       if (this->cameraCheckId != 0) {
// 		stop_camera_check();
// 		pthread_join(this->cameraCheckId, NULL);
// 	}
//       if ((inputSdkMask & MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH) {
//             stop_dms_capture();
// 		pthread_join(this->dmsCaptureId, NULL);
//       }
//       if ((inputSdkMask & MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) {
//             stop_dms_capture();
// 	 	pthread_join(this->omsCaptureId, NULL);
//       }
//       LOG_DEBUG("end qcamera uninit\n");
//       return 0;
// }
// int TestCamera::GetCameraArcImg(MYSDK_SET_MASK inputSdkMask, ASVLOFFSCREEN& arcImg) {
//       //exclude all_mask
//       if (((inputSdkMask & MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH)
//            &&((inputSdkMask & MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) ) {
//             LOG_ERROR("GetCameraArcImg(), inputSdkMask only be dms or oms!\n");
//             exit(1);
//       }
//       if ((inputSdkMask & MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH) {
//             //copy image, need mutex
//             int cpRet = -1;
//             pthread_mutex_lock(&mtx_camera_pushdata_dms);
//             memcpy(arcImg.ppu8Plane[0], G_CameraImgBuff_Dms, sizeof(char)*DMS_BUFF_LENGTH);
//             pthread_mutex_unlock(&mtx_camera_pushdata_dms);
//             return 0;
//       }
//       else if ((inputSdkMask&MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) {
//             //copy image, need mutex
//             int cpRet = -1;
//             pthread_mutex_lock(&mtx_camera_pushdata_oms);
//             memcpy(arcImg.ppu8Plane[0], G_CameraImgBuff_Oms, sizeof(char)*OMS_BUFF_LENGTH);
//             pthread_mutex_unlock(&mtx_camera_pushdata_oms);
//             return 0;
//       }
// }
// int TestCamera::DmsOmsAlarm(MYSDK_SET_MASK inputSdkMask, MySdk& mySdk) {
//       if ((inputSdkMask&MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH) {
//             if(0 != pthread_create(&(mySdk.thrDmsDataPut), nullptr, G_DmsCamera_DataThread, &mySdk)){
//                   LOG_ERROR("error! G_DmsCamera_DataThread create wrong! \n");
//                   exit(1);
//             }
//             LOG_INFO("success! G_DmsCamera_DataThread create!\n");
//       }
//       if ((inputSdkMask&MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) {
//             if(0 != pthread_create(&(mySdk.thrOmsDataPut), nullptr, G_OmsCamera_DataThread, &mySdk)){
//                   LOG_ERROR("error! G_OmsCamera_DataThread create wrong! \n");
//                   exit(1);
//             }
//             LOG_INFO("success! G_OmsCamera_DataThread create!\n");
// //////////////////////////////////////////////////////
// ////                   Oms_Detect_Switch_Set
// //////////////////////////////////////////////////////
//             LOG_INFO("open all oms detect switch\n");
//             mySdk.SetOmsDetectSwitch(ASD_MASK_OMS_DEFAULT | ASD_OMS_MASK_BABY | ASD_OMS_MASK_HUMEN_PET | ASD_OMS_MASK_RESIDUE | ASD_OMS_MASK_AGE_GENDER);
//       }
//       //WAIT AND CANCEL
//        sleep(100);
//       //blocking or wait signal
//       if(0 != pthread_join(mySdk.thrDmsDataPut, NULL)){
//             LOG_ERROR("error! pthread_join wrong");
//             exit(1);
//       };
//       //blocking or wait signal
//       if(0 != pthread_join(mySdk.thrOmsDataPut, NULL)){
//             LOG_ERROR("error! pthread_join wrong");
//             exit(1);
//       };
//       return 0;
// }
// int TestCamera::FaceidRegisterLoginMatch(MYSDK_SET_MASK inputSdkMask, MySdk& mySdk) {
//       //exclude all_mask
//       if (((inputSdkMask & MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH)
//            &&((inputSdkMask & MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) ) {
//             LOG_ERROR("FaceidRegisterLoginMatch(), inputSdkMask only be dms or oms!\n");
//             exit(1);
//       }
//       if ((inputSdkMask & MYSDK_SET_MASK_DMS_DETECT_SWITCH) == MYSDK_SET_MASK_DMS_DETECT_SWITCH) {
//             //register_login_match, tip and countdown and do, next step
//             //set buff
//             unsigned char* imgTempBuff = (unsigned char*)malloc(sizeof(unsigned char*) * DMS_BUFF_LENGTH);
//             //unsigned char imgTempBuff[DMS_CAMERA_WIDTH*DMS_CAMERA_HEIGHT*2];
//             ASVLOFFSCREEN arcImg = { 0 };
//             arcImg.i32Width = DMS_CAMERA_WIDTH;
//             arcImg.i32Height = DMS_CAMERA_HEIGHT;
//             arcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
//             arcImg.pi32Pitch[0] = arcImg.i32Width * 2;//间距
//             arcImg.ppu8Plane[0] = (MUInt8 *)(imgTempBuff);
//             arcImg.ppu8Plane[1] = MNull;
//             char bufRegister[1000] = { 0 }, bufLogin[1000] = { 0 };
//             int32_t registerRet = -1, loginRet = -1, matchRet = -1;
// #define MAX_TRY_TIMES 200
//             //register-countdown
//             LOG_INFO("start faceid regster countdown! please prepare!\n");
//             int countDownTimeS = 8;
//             for (int i = 0;i < countDownTimeS;i++) {
//                   sleep(1);
//                   LOG_INFO("surplus: %d\n", i-1);
//             }
//             LOG_INFO("capture the frame push to dms register\n");
//             //register
//             for (int i = 0;i < MAX_TRY_TIMES;i++) {
//                   LOG_INFO("dms face register, try %d times\n", i);
//                   this->GetCameraArcImg(MYSDK_SET_MASK_DMS_DETECT_SWITCH, arcImg);
//                   registerRet = mySdk.FaceRegister(&arcImg, ASD_CAM_MASK_DMS, bufRegister);
//                   if (registerRet != 0) {
//                         LOG_ERROR("%dtimes, Dms FaceRegister error, ret = %d\n", i, registerRet);
//                         continue;
//                   }
//                   break;
//             }
//             if (registerRet == 0) {
//                   LOG_ERROR("Dms FaceRegister success!%d\n");
//             }
//             else {
//                   LOG_ERROR("Dms FaceRegister error, ret = %d\n", registerRet);
//                   //exit or jump do again
//                   exit(1);
//             }
//             //login-countdown
//              LOG_INFO("start faceid login countdown! please prepare!\n");
//             //int countDownTimeS = 5;
//             for (int i = 0;i < countDownTimeS;i++) {
//                   sleep(1);
//                   LOG_INFO("surplus: %d\n", i-1);
//             }
//             LOG_INFO("capture the frame push to dms login\n");
//             //login
//             for (int i = 0;i < MAX_TRY_TIMES;i++) {
//                   LOG_INFO("dms face login, try %d times\n", i);
//                   this->GetCameraArcImg(MYSDK_SET_MASK_DMS_DETECT_SWITCH, arcImg);
//                   loginRet = mySdk.FaceRecognition(&arcImg, ASD_CAM_MASK_DMS, bufRegister);
//                   if (loginRet != 0) {
//                         LOG_ERROR("%dtimes, Dms Login error, ret = %d\n", i, loginRet);
//                         continue;
//                   }
//                   break;
//             }
//             if (loginRet == 0) {
//                   LOG_ERROR("Dms Login success!%d\n");
//             }
//             else {
//                   LOG_ERROR("Dms Login error, ret = %d\n", loginRet);
//                   //exit or jump do again
//                   exit(1);
//             }
//             //match
//             double similarityRet = -1;
//             matchRet = mySdk.FaceMatch(bufRegister, bufLogin, &similarityRet);
//             if (matchRet != 0) {
//                   LOG_ERROR();
//                   //eixt jump to do again
//                   exit(1);
//             }
//             LOG_INFO("get faceid register and login match score:%lf\n", similarityRet);
//             free(imgTempBuff);
//       }
//       else if ((inputSdkMask&MYSDK_SET_MASK_OMS_DETECT_SWITCH) == MYSDK_SET_MASK_OMS_DETECT_SWITCH) {
//             //register_login_match, tip and countdown and do, next step
//             //buff set
//             unsigned char* imgTempBuff = (unsigned char*)malloc(sizeof(unsigned char*) * OMS_BUFF_LENGTH);
//             //unsigned char imgTempBuff[OMS_CAMERA_WIDTH*OMS_CAMERA_HEIGHT*2];
//             ASVLOFFSCREEN arcImg = { 0 };
//             arcImg.i32Width = OMS_CAMERA_WIDTH;
//             arcImg.i32Height = OMS_CAMERA_HEIGHT;
//             arcImg.u32PixelArrayFormat = ASVL_PAF_UYVY;
//             arcImg.pi32Pitch[0] = arcImg.i32Width * 2;//间距
//             arcImg.ppu8Plane[0] = (MUInt8 *)(imgTempBuff);
//             arcImg.ppu8Plane[1] = MNull;
//             char bufRegister[1000] = { 0 }, bufLogin[1000] = { 0 };
//             int32_t registerRet = -1, loginRet = -1, matchRet = -1;
// #define MAX_TRY_TIMES 200
            
//             //register-countdown
//             LOG_INFO("start faceid regster countdown! please prepare!\n");
//             int countDownTimeS = 8;
//             for (int i = 0;i < countDownTimeS;i++) {
//                   sleep(1);
//                   LOG_INFO("surplus: %d\n", i-1);
//             }
//             LOG_INFO("capture the frame push to oms register\n");

//             //register
//             for (int i = 0;i < MAX_TRY_TIMES;i++) {
//                   LOG_INFO("oms face register, try %d times\n", i);
//                   this->GetCameraArcImg(MYSDK_SET_MASK_OMS_DETECT_SWITCH, arcImg);
//                   registerRet = mySdk.FaceRegister(&arcImg, ASD_CAM_MASK_OMS, bufRegister);
//                   if (registerRet != 0) {
//                         LOG_ERROR("%dtimes, Oms FaceRegister error, ret = %d\n", i, registerRet);
//                         continue;
//                   }
//                   break;
//             }
//             if (registerRet == 0) {
//                   LOG_ERROR("Oms FaceRegister success!%d\n");
//             }
//             else {
//                   LOG_ERROR("Oms FaceRegister error, ret = %d\n", registerRet);

//                   //exit or jump do again
//                   exit(1);
//             }
//             //login-countdown
//              LOG_INFO("start faceid login countdown! please prepare!\n");
//             //int countDownTimeS = 5;
//             for (int i = 0;i < countDownTimeS;i++) {
//                   sleep(1);
//                   LOG_INFO("surplus: %d\n", i-1);
//             }
//             LOG_INFO("capture the frame push to oms login\n");
//             //login
//             for (int i = 0;i < MAX_TRY_TIMES;i++) {
//                   LOG_INFO("oms face login, try %d times\n", i);
//                   this->GetCameraArcImg(MYSDK_SET_MASK_OMS_DETECT_SWITCH, arcImg);
//                   loginRet = mySdk.FaceRecognition(&arcImg, ASD_CAM_MASK_OMS, bufRegister);
//                   if (loginRet != 0) {
//                         LOG_ERROR("%dtimes, Dms Login error, ret = %d\n", i, loginRet);
//                         continue;
//                   }
//                   break;
//             }
//             if (loginRet == 0) {
//                   LOG_ERROR("Oms Login success!%d\n");
//             }
//             else {
//                   LOG_ERROR("Oms Login error, ret = %d\n", loginRet);
//                   //exit or jump do again
//                   exit(1);
//             }
//             //match
//             double similarityRet = -1;
//             matchRet = mySdk.FaceMatch(bufRegister, bufLogin, &similarityRet);
//             if (matchRet != 0) {
//                   LOG_ERROR();
//                   //eixt jump to do again
//                   exit(1);
//             }
//             LOG_INFO("get faceid register and login match score:%lf\n", similarityRet);
//             free(imgTempBuff);
//       }
//       //return 
//       return 0;
// }
// //////////////////////////////////////////////////////
// ////                   camera-init
// //////////////////////////////////////////////////////
// int TestCamera::DumpImg(void) {
//       MYSDK_SET_MASK mySdkMask = MYSDK_SET_MASK_DMS_DETECT_SWITCH;
//       if (0 == strcmp(FLAGS_module.c_str(), "dms")) {
//             mySdkMask |= MYSDK_SET_MASK_DMS_DETECT_SWITCH;
//             LOG_DEBUG("sdk mask :%d\n", mySdkMask);
//       }
//       if (0 == strcmp(FLAGS_module.c_str(), "oms")) {
//             mySdkMask |= MYSDK_SET_MASK_OMS_DETECT_SWITCH;
//             LOG_DEBUG("sdk mask :%d\n", mySdkMask);
//       }
//       if (0 == strcmp(FLAGS_module.c_str(), "dms_oms")) {
//             mySdkMask |= MYSDK_SET_MASK_DMS_DETECT_SWITCH;
//             mySdkMask |= MYSDK_SET_MASK_OMS_DETECT_SWITCH;
//             LOG_DEBUG("sdk mask :%d\n", mySdkMask);
//       }
//       // struct input_event ie;
//       pthread_t thrDmsDump, thrOmsDump;
//       this->QcameraInit(mySdkMask);
//       if (mySdkMask == MYSDK_SET_MASK_DMS_DETECT_SWITCH) {
//             if (0 != pthread_create(&(thrDmsDump), nullptr, G_DmsDumpImg, NULL)) {
//                   LOG_ERROR("error! G_DmsDumpImg create wrong! \n");
//                   exit(1);
//             }
//             LOG_INFO("success! G_DmsDumpImg create!\n");
//       }
//       if (mySdkMask == MYSDK_SET_MASK_OMS_DETECT_SWITCH) {
//             if(0 != pthread_create(&(thrOmsDump), nullptr, G_OmsDumpImg, NULL)){
//                   LOG_ERROR("error! G_OmsDumpImg create wrong! \n");
//                   exit(1);
//             }
//             LOG_INFO("success! G_OmsDumpImg create!\n");
//       }
//       sleep(100);
//       this->QcameraUnInit(mySdkMask);
//       return 0;
// }

