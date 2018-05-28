package com.example.ty.openglndk

import android.content.res.AssetManager
import android.graphics.Bitmap
import android.view.Surface

/*
 * Created by TY on 2018/5/25.
 */
class NativeOpenglControler {
    external fun init(assetManager: AssetManager?)
    external fun resetSize(width: Int, height: Int, surface: Surface?)
    external fun stop()
    external fun initCube(assetManager: AssetManager?, b: Bitmap?);

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}
