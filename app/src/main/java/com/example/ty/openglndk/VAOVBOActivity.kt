package com.example.ty.openglndk

import android.app.ActivityManager
import android.content.Context
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.util.Log
import android.view.SurfaceHolder
import kotlinx.android.synthetic.main.activity_vao_vbo.*


class VAOVBOActivity : AppCompatActivity() {

    private var controler: NativeOpenglControler? = null

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_vao_vbo)
        val am = getSystemService(Context.ACTIVITY_SERVICE) as ActivityManager
        controler = NativeOpenglControler()
        surface.holder.addCallback(object : SurfaceHolder.Callback {
            override fun surfaceChanged(holder: SurfaceHolder?, format: Int, width: Int, height: Int) {
                controler?.resetSize(width, height,holder?.surface)
            }

            override fun surfaceDestroyed(holder: SurfaceHolder?) {
                Log.d("tedu", "surfacedestory")

            }

            override fun surfaceCreated(holder: SurfaceHolder?) {
                controler?.init(assets)
            }
        })
        btn.setOnClickListener {
            surface.layoutParams.height=1000
            surface.requestLayout()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        controler?.stop()
    }


}

