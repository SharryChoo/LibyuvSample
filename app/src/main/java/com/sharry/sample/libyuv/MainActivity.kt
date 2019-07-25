package com.sharry.sample.libyuv

import android.os.Bundle
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        // 测试代码
        val width = 100
        val height = 100
        val nv21Bytes = ByteArray(width * height * 3 / 2)
        val i420Bytes = ByteArray(width * height * 3 / 2)
        LibyuvUtil.convertNV21ToI420(nv21Bytes, i420Bytes, width, height)
    }
}
