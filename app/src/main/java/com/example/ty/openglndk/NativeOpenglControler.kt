package com.example.ty.openglndk

import android.content.res.AssetManager
import android.view.Surface

/*
 * Created by TY on 2018/5/25.
 */
class NativeOpenglControler{
    external fun init(assetManager: AssetManager?, surface: Surface?)
    external fun resetSize(width:Int,height:Int)
    external fun stop()
    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}
