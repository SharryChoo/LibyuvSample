//
// Created by Sharry Choo on 2019-07-24.
//

#ifndef SMEDIA_LIBYUVUTILS_H
#define SMEDIA_LIBYUVUTILS_H

#include <jni.h>

class LibyuvUtil {

public:
    static void NV21ToI420(jbyte *src, jbyte *dst, int width, int height);

    static void I420ToNV12(jbyte *src, jbyte *dst, int width, int height);

    static void I420ToNV21(jbyte *src, jbyte *dst, int width, int height);

    static void I420ToABGR(jbyte *src, int width, int height, void *dst, int dst_stride);

    static void I420Scale(jbyte *src, int src_width, int src_height, jbyte *dst,
                          int dst_width, int dst_height);

    static void I420Rotate(jbyte *src, jbyte *dst, int &width, int &height, int degree);

    static void I420Mirror(jbyte *src, jbyte *dst, int width, int height);

    static void I420Crop(jbyte *src, int src_width, int src_height, jbyte *dst,
                         int dst_width, int dst_height, int left, int top);
};


#endif //SMEDIA_LIBYUVUTILS_H
