//
// Created by Sharry Choo on 2019-06-17.
//
#ifndef SCAMERA_CONSTDEFINE_H
#define SCAMERA_CONSTDEFINE_H

#include <android/log.h>

#define TAG "SCamera-Native"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, TAG, __VA_ARGS__)
#define NV21_UTIL_CLASS_NAME "com/sharry/sample/libyuv/LibyuvUtil"
#endif //SCAMERA_CONSTDEFINE_H
