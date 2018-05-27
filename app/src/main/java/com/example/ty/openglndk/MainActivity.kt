package com.example.ty.openglndk

import android.app.ActivityManager
import android.content.Context
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.SurfaceHolder
import kotlinx.android.synthetic.main.activity_main.*


class MainActivity : AppCompatActivity() {

    private var controler: NativeOpenglControler? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val am = getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        val info = am.deviceConfigurationInfo
        controler=NativeOpenglControler()
        surface.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
                Log.d("tedu","surfaceChanged ${width}, ${height}");
                controler?.resetSize(width, height)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                Log.d("tedu","surfacedestory")
//                controler?.stop()

            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
                Log.d("tedu","oncreate "+holder?.surface?.hashCode())
                Log.v("tedu","oncreate "+holder?.hashCode())
                controler?.init(assets, holder?.surface)
            }
        })



    }


}

