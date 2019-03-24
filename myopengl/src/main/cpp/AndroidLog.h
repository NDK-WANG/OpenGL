//
// Created by 王强 on 2019/3/14.
//

#ifndef OPENCAR_ANDROIDLOG_H
#define OPENCAR_ANDROIDLOG_H

#endif //OPENCAR_ANDROIDLOG_H

#include "android/log.h"

#define LOG_ERROR true

#define LOGE(FORMAT,...) __android_log_print(ANDROID_LOG_ERROR,"NDK",FORMAT,##__VA_ARGS__);
