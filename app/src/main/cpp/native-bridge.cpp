#include <jni.h>
#include <android/bitmap.h>
#include <cstring>
#include "ConstDefine.h"
#include "LibyuvUtil.h"

int registerNativeMethods(JNIEnv *env, jclass cls);

extern "C"
JNIEXPORT jint JNICALL
JNI_OnLoad(JavaVM *javaVM, void *reserverd) {
    // 通过初始化方法获取 JavaVM
    JNIEnv *env;
    if (javaVM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    jclass jAudioPlayerCls = env->FindClass(NV21_UTIL_CLASS_NAME);
    jAudioPlayerCls = reinterpret_cast<jclass>(env->NewGlobalRef(jAudioPlayerCls));
    if (!jAudioPlayerCls) {
        LOGE("Fail to create global reference for %s", NV21_UTIL_CLASS_NAME);
    }
    int res = registerNativeMethods(env, jAudioPlayerCls);
    if (res != 0) {
        LOGE("Failed to register native methods for class %s ", NV21_UTIL_CLASS_NAME);
    }
    env->DeleteGlobalRef(jAudioPlayerCls);
    return JNI_VERSION_1_6;
}

namespace libyuv_util {

    void convertI420ToNV12(JNIEnv *env, jclass, jbyteArray i420_src, jbyteArray nv12_dst, int width,
                           int height) {
        jbyte *src = env->GetByteArrayElements(i420_src, NULL);
        jbyte *dst = env->GetByteArrayElements(nv12_dst, NULL);
        // 执行转换 I420 -> NV12 的转换
        LibyuvUtil::I420ToNV12(src, dst, width, height);
        // 释放资源
        env->ReleaseByteArrayElements(i420_src, src, 0);
        env->ReleaseByteArrayElements(nv12_dst, dst, 0);
    }

    void compressI420(JNIEnv *env, jclass, jbyteArray i420_src, int src_width,
                      int src_height, jbyteArray i420_dst, int dst_width,
                      int dst_height, int degree, jboolean isMirror) {
        jbyte *src = env->GetByteArrayElements(i420_src, NULL);
        const int dst_size = dst_width * dst_height * 3 / 2;
        // step1: 缩放处理
        jbyte *scaled = src;
        if (src_width != dst_width || src_height != dst_height) {
            scaled = new jbyte[dst_size];
            LibyuvUtil::I420Scale(src, src_width, src_height, scaled, dst_width,
                                  dst_height);
        }
        // step2: 旋转处理
        jbyte *rotated = scaled;
        if (degree != 0) {
            rotated = new jbyte[dst_size];
            // 若为 90/270 旋转之后会反转 width 和 height
            LibyuvUtil::I420Rotate(scaled, rotated, dst_width, dst_height, degree);
            if (scaled != src) {
                delete[]scaled;
            }
        }
        // step3: 镜像处理
        jbyte *mirrored = rotated;
        if (isMirror) {
            mirrored = new jbyte[dst_size];
            LibyuvUtil::I420Mirror(rotated, mirrored, dst_width, dst_height);
            if (rotated != src) {
                delete[]rotated;
            }
        }
        // step4: 将数据拷贝到 dst 中
        jbyte *dst = env->GetByteArrayElements(i420_dst, NULL);
        memcpy(dst, mirrored, (size_t) dst_size);
        // 释放资源
        if (mirrored != src) {
            delete[]mirrored;
        }
        env->ReleaseByteArrayElements(i420_src, src, 0);
        env->ReleaseByteArrayElements(i420_dst, dst, 0);
    }

    void convertI420ToBitmap(JNIEnv *env, jclass, jbyteArray nv21_src, jobject bitmap, int width,
                             int height) {
        jbyte *src = env->GetByteArrayElements(nv21_src, NULL);
        // 锁定画布
        void *dst_argb;
        AndroidBitmap_lockPixels(env, bitmap, &dst_argb);
        // 获取 bitmap 的信息
        AndroidBitmapInfo info;
        AndroidBitmap_getInfo(env, bitmap, &info);
        if (info.format == ANDROID_BITMAP_FORMAT_RGBA_8888) {
            // ABGR 的 stride 为 4 * width
            LibyuvUtil::I420ToABGR(src, width, height, dst_argb, info.stride);
        } else {
            // ignore.
        }
        // 解锁画布
        AndroidBitmap_unlockPixels(env, bitmap);
        // 释放通过 jbyteArray 创建的 jbyte*
        env->ReleaseByteArrayElements(nv21_src, src, 0);
    }

    void convertNV21ToI420(JNIEnv *env, jclass, jbyteArray nv21_src, jbyteArray nv12_dst, int width,
                           int height) {
        jbyte *src = env->GetByteArrayElements(nv21_src, NULL);
        jbyte *dst = env->GetByteArrayElements(nv12_dst, NULL);
        // 执行转换
        LibyuvUtil::NV21ToI420(src, dst, width, height);
        // 释放通过 jbyteArray 创建的 jbyte*
        env->ReleaseByteArrayElements(nv21_src, src, 0);
        env->ReleaseByteArrayElements(nv12_dst, dst, 0);
    }
}

JNINativeMethod libyuv_util_methods[] = {
        {"convertNV21ToI420",   "([B[BII)V",                        (void *) libyuv_util::convertNV21ToI420},
        {"convertI420ToNV12",   "([B[BII)V",                        (void *) libyuv_util::convertI420ToNV12},
        {"convertI420ToBitmap", "([BLandroid/graphics/Bitmap;II)V", (void *) libyuv_util::convertI420ToBitmap},
        {"compressI420",        "([BII[BIIIZ)V",                    (void *) libyuv_util::compressI420},
};

int registerNativeMethods(JNIEnv *env, jclass cls) {
    return env->RegisterNatives(cls, libyuv_util_methods,
                                sizeof(libyuv_util_methods) / sizeof(libyuv_util_methods[0]));
}

