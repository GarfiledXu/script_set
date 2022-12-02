/*******************************************************************************
Copyright(c) ArcSoft, All right reserved.

This file is ArcSoft's property. It contains ArcSoft's trade secret, proprietary
and confidential information.

The information and code contained in this file is only for authorized ArcSoft
employees to design, create, modify, or review.

DO NOT DISTRIBUTE, DO NOT DUPLICATE OR TRANSMIT IN ANY FORM WITHOUT PROPER
AUTHORIZATION.

If you are not an intended recipient of this file, you must not copy,
distribute, modify, or take any action in reliance on it.

If you have received this file in error, please immediately notify ArcSoft and
permanently delete the original and any copy of any file and any printout
thereof.
*******************************************************************************/
#ifndef LOGUTIL_H_
#define LOGUTIL_H_

#include <stdio.h>

extern int g_arclog_level;


/********************** log level definition*****************************/
enum log_level {
    ARC_TRACE=1,
	ARC_DEBUG,
	ARC_INFO,
	ARC_WARN,
	ARC_ERROR
};

char * timeString();

#define WRITE_LOG(LOG_LEVEL,format,args...)  \
    if (LOG_LEVEL >= g_arclog_level) { printf("["#LOG_LEVEL"] %s " format , timeString(), ##args);}


#endif/* LOGUTIL_H_ */
