package com.example.ty.openglndk

import android.content.res.AssetManager
import android.graphics.Bitmap

/*
 * Created by TY on 2018/5/25.
 */
class NativeOpenglControler {
    external fun init(assetManager: AssetManager?)
    external fun resetSize(width: Int, height: Int, surface: Any?)
    external fun stop()
    external fun initCube(assetManager: AssetManager?, b: Bitmap?);
    external fun rotate(x:Float,y:Float,degree:Float)
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}
