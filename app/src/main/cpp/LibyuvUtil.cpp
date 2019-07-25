//
// Created by Sharry Choo on 2019-07-24.
//

#include "LibyuvUtil.h"
#include "libyuv.h"

void LibyuvUtil::NV21ToI420(jbyte *src, jbyte *dst, int width, int height) {
    // NV21 参数
    jint src_y_size = width * height;
    jbyte *src_y = src;
    jbyte *src_vu = src + src_y_size;
    // I420 参数
    jint dst_y_size = width * height;
    jint dst_u_size = dst_y_size >> 2;
    jbyte *dst_y = dst;
    jbyte *dst_u = dst + dst_y_size;
    jbyte *dst_v = dst + dst_y_size + dst_u_size;
    /**
    * <pre>
    * int NV21ToI420(const uint8_t* src_y,
    *          int src_stride_y,
    *          const uint8_t* src_vu,
    *          int src_stride_vu,
    *          uint8_t* dst_y,
    *          int dst_stride_y,
    *          uint8_t* dst_u,
    *          int dst_stride_u,
    *          uint8_t* dst_v,
    *          int dst_stride_v,
    *          int width,
    *          int height);
    * </pre>
    * <p>
    * stride 为颜色分量的跨距: 它描述一行像素中, 该颜色分量所占的 byte 数目, YUV 每个通道均为 1byte(8bit)
    * <p>
    * stride_y: Y 是最全的, 一行中有 width 个像素, 也就有 width 个 Y
    * stride_u: YUV420 的采样为 Y:U:V = 4:1:1, 从整体的存储来看, 一个 Y 分量的数目为 U/V 的四倍
    * 但从一行上来看, width 个 Y, 它会用到 width/2 个 U
    * stride_v: 同 stride_u 的分析方式
    */
    libyuv::NV21ToI420(
            (uint8_t *) src_y, width,
            (uint8_t *) src_vu, width,
            (uint8_t *) dst_y, width,
            (uint8_t *) dst_u, width >> 1,
            (uint8_t *) dst_v, width >> 1,
            width, height
    );
}

void LibyuvUtil::I420ToNV12(jbyte *src, jbyte *dst, int width, int height) {

    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jint dst_y_size = width * height;
    jbyte *dst_y = dst;
    jbyte *dst_uv = dst + dst_y_size;

    libyuv::I420ToNV12(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst_y, width,
            (uint8_t *) dst_uv, width,
            width, height
    );
}

void LibyuvUtil::I420ToABGR(jbyte *src, int width, int height, void *dst, int dst_stride) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;
    libyuv::I420ToABGR(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst, dst_stride,
            width, height
    );
}

void LibyuvUtil::I420ToNV21(jbyte *src, jbyte *dst, int width, int height) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jint dst_y_size = width * height;
    jbyte *dst_y = dst;
    jbyte *dst_vu = dst + dst_y_size;

    libyuv::I420ToNV21(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst_y, width,
            (uint8_t *) dst_vu, width,
            width, height
    );
}

void LibyuvUtil::I420Scale(jbyte *src, int src_width, int src_height, jbyte *dst,
                           int dst_width, int dst_height) {
    jint src_y_size = src_width * src_height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jint dst_y_size = dst_width * dst_height;
    jint dst_u_size = dst_y_size >> 2;
    jbyte *dst_y = dst;
    jbyte *dst_u = dst + dst_y_size;
    jbyte *dst_v = dst + dst_y_size + dst_u_size;

    libyuv::I420Scale(
            (uint8_t *) src_y, src_width,
            (uint8_t *) src_u, src_width >> 1,
            (uint8_t *) src_v, src_width >> 1,
            src_width, src_height,
            (uint8_t *) dst_y, dst_width,
            (uint8_t *) dst_u, dst_width >> 1,
            (uint8_t *) dst_v, dst_width >> 1,
            dst_width, dst_height,
            libyuv::FilterMode::kFilterNone
    );

}

void LibyuvUtil::I420Rotate(jbyte *src, jbyte *dst, int &width, int &height,
                            int degree) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jbyte *dst_y = dst;
    jbyte *dst_u = dst + src_y_size;
    jbyte *dst_v = dst + src_y_size + src_u_size;

    libyuv::I420Rotate(
            (uint8_t *) src_y, width,
            (uint8_t *) src_u, width >> 1,
            (uint8_t *) src_v, width >> 1,
            (uint8_t *) dst_y, height,
            (uint8_t *) dst_u, height >> 1,
            (uint8_t *) dst_v, height >> 1,
            width, height, (libyuv::RotationMode) degree
    );
    // 若为 90 / 270,  则翻转宽高
    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        width += height;
        height = width - height;
        width -= height;
    }
}

void LibyuvUtil::I420Mirror(jbyte *src, jbyte *dst, int width, int height) {
    jint src_y_size = width * height;
    jint src_u_size = src_y_size >> 2;
    jbyte *src_y = src;
    jbyte *src_u = src + src_y_size;
    jbyte *src_v = src + src_y_size + src_u_size;

    jbyte *dst_y = dst;
    jbyte *dst_u = dst + src_y_size;
    jbyte *dst_v = dst + src_y_size + src_u_size;

    libyuv::I420Mirror((uint8_t *) src_y, width,
                       (uint8_t *) src_u, width >> 1,
                       (uint8_t *) src_v, width >> 1,
                       (uint8_t *) dst_y, width,
                       (uint8_t *) dst_u, width >> 1,
                       (uint8_t *) dst_v, width >> 1,
                       width, height);
}

void LibyuvUtil::I420Crop(jbyte *src, int src_width, int src_height, jbyte *dst, int dst_width,
                          int dst_height, int left, int top) {
    jint dst_y_size = dst_width * dst_height;
    jint dst_u_size = dst_y_size >> 2;
    jbyte *dst_y = dst;
    jbyte *dst_u = dst + dst_y_size;
    jbyte *dst_v = dst + dst_y_size + dst_u_size;

    libyuv::ConvertToI420(
            (uint8_t *) src, (size_t) src_width * src_height * 3 / 2,
            (uint8_t *) dst_y, dst_width,
            (uint8_t *) dst_u, dst_width >> 1,
            (uint8_t *) dst_v, dst_width >> 1,
            left, top,
            src_width, src_height,
            dst_width, dst_height,
            libyuv::kRotate0, libyuv::FOURCC_I420
    );

}
